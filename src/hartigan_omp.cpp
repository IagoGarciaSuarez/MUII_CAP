#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <random>
#include <Eigen/Dense>
#include <omp.h> 
#include <opencv2/opencv.hpp>

const std::string FILE_PATH = "pavia.txt";
const float HARTIGAN_THRESHOLD = 50.0;
const int K = 3;

Eigen::MatrixXf load_data(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    std::vector<float> values;
    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        while (getline(ss, value, ',')) {
            values.push_back(std::stof(value));
        }
    }

    Eigen::MatrixXf matrix(values.size() / 102, 102);
    for (size_t i = 0; i < values.size() / 102; ++i) {
        for (size_t j = 0; j < 102; ++j) {
            matrix(i, j) = values[i * 102 + j];
        }
    }

    return matrix;
}

float distancia_cuadrada(const Eigen::VectorXf& p, const Eigen::VectorXf& c) {
    return (p - c).squaredNorm();
}

int main() {
    std::cout << "========== OMP ==========\n";
    omp_set_num_threads(4);
    try {
        Eigen::MatrixXf data = load_data(FILE_PATH);
        std::cout << "----- Datos cargados -----\n";
        std::cout << "Calculando...\n";

        double start = omp_get_wtime();

        // Inicialización de asignaciones y centroides
        std::mt19937 gen(static_cast<unsigned int>(time(0))); // Semilla con el tiempo actual
        std::uniform_int_distribution<> dist(0, K);
        std::vector<int> asignaciones(data.rows() * data.cols());

        #pragma omp parallel for
        for(auto& val : asignaciones) {
            val = dist(gen);
        }
        std::vector<Eigen::VectorXf> centroides(K, Eigen::VectorXf::Zero(data.cols()));

        for (int i = 0; i < K; ++i) {
            centroides.push_back(Eigen::VectorXf::Zero(data.cols()));
        }
        std::vector<int> count(K, 0);

        #pragma omp parallel for
        for (int i = 0; i < data.rows(); ++i) {
            int asignacion = asignaciones[i];
            if (asignacion >= 0 && asignacion < K) {
                Eigen::VectorXf tempRow = data.row(i);
                #pragma omp critical
                {
                    centroides[asignacion] += tempRow;
                    count[asignacion]++;
                }
            }
        }
        #pragma omp parallel for
        for (int i = 0; i < K; ++i) {
            if (count[i] > 0) { 
                centroides[i] /= count[i];
            }
        }

        while (true) {
            auto centroides_anterior = centroides;

            for (int i = 0; i < data.rows(); ++i) {
                float min_dist = std::numeric_limits<float>::max();
                int min_dist_idx = -1;
                for (size_t j = 0; j < centroides.size(); ++j) {
                    float dist = distancia_cuadrada(data.row(i), centroides[j]);
                    if (dist < min_dist) {
                        min_dist = dist;
                        min_dist_idx = j;
                    }
                }
                asignaciones[i] = min_dist_idx;
            }

            float inercia_anterior = 0.0;
            float inercia_actual = 0.0;
            for (int i = 0; i < data.rows(); ++i) {
                inercia_anterior += distancia_cuadrada(data.row(i), centroides_anterior[asignaciones[i]]);
                inercia_actual += distancia_cuadrada(data.row(i), centroides[asignaciones[i]]);
            }

            if (std::abs(inercia_anterior - inercia_actual) < HARTIGAN_THRESHOLD) {
                break;
            }
        }

        double end = omp_get_wtime();

        double tiempo_transcurrido = end - start;
        std::cout << "K = " << K << ". Tiempo de ejecución = " << tiempo_transcurrido << " segs" << std::endl;


        cv::Mat cluster_map(715,1096, CV_8U);
        cv::Mat matAsignaciones(data.rows(), data.cols(), CV_32S);
        for(int i = 0; i < data.rows(); ++i) {
            for(int j = 0; j < data.cols(); ++j) {
                matAsignaciones.at<int>(i, j) = asignaciones[i * data.cols() + j];
            }
        }

        int index = 0;
        int prev_label = 0;

        for (int i = 0; i < cluster_map.rows; ++i) {
            for (int j = 0; j < cluster_map.cols; ++j) {  
                int l = cluster_map.at<uchar>(i, j) = matAsignaciones.at<int>(i,j);
                if (prev_label != cluster_map.at<unsigned char>(i, j)){
                    prev_label = cluster_map.at<uchar>(i, j);
                }
            }   
        }
        
        cv::Mat result_image(1096,715, CV_8UC3);
        cv::Vec3b colors[] = {
            cv::Vec3b(0, 0, 255),   // Rojo
            cv::Vec3b(0, 255, 0),   // Verde
            cv::Vec3b(255, 0, 0),   // Azul
            cv::Vec3b(255, 255, 0), // Amarillo
            cv::Vec3b(255, 0, 255), // Magenta
            cv::Vec3b(0, 255, 255), // Cian
            cv::Vec3b(255, 255, 255),// Blanco
            cv::Vec3b(0, 0, 0),     // Negro
            cv::Vec3b(128, 128, 128),// Gris
            cv::Vec3b(64, 64, 64)   // Gris oscuro
        };

        const int height = 715; // Altura
        const int width = 1096; // Anchura
        cv::Mat imagen(height, width, CV_8UC3);
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                int index = i * width + j;
                imagen.at<cv::Vec3b>(i, j) = colors[asignaciones[index]];
            }
        }
        cv::Mat resultT;
        cv::transpose(imagen, resultT);

        cv::imwrite("results/hartigan_omp_" + std::to_string(K) + ".jpeg", resultT);

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
