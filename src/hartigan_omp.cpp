#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <Eigen/Dense>
// #include <opencv2/opencv.hpp>
// #include <omp.h>

const std::string FILE_PATH = "./pavia.txt";
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
float distanciaCuadrada(const std::vector<float>& v1, const std::vector<float>& v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Los vectores deben tener el mismo tamaño");
    }

    float distancia = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        distancia += std::pow(v1[i] - v2[i], 2);
    }
    return distancia;
}

int main() {
    try {
        Eigen::MatrixXf data = load_data(FILE_PATH);
        std::cout << "Datos cargados con éxito.\n";
        // Imprimir las dimensiones de la matriz
        std::cout << "Dimensiones de la matriz: " << data.rows() << "x" << data.cols() << std::endl;

        // double start = omp_get_wtime();

        // // Inicialización de asignaciones y centroides
        // std::mt19937 gen(static_cast<unsigned int>(time(0))); // Semilla con el tiempo actual
        // std::uniform_int_distribution<> dist(0, K);
        // std::vector<int> asignaciones(data.rows() * data.cols());
        // for(auto& val : asignaciones) {
        //     val = dist(gen);
        // }

        // cv::Mat cluster_map(715,1096, CV_8U);
        // cv::Mat matAsignaciones(data.rows(), data.cols(), CV_32S);
        // for(int i = 0; i < data.rows(); ++i) {
        //     for(int j = 0; j < data.cols(); ++j) {
        //         matAsignaciones.at<int>(i, j) = asignaciones[i * data.cols() + j];
        //     }
        // }

        // int index = 0;
        // int prev_label = 0;

        // for (int i = 0; i < cluster_map.rows; ++i) {
        //     for (int j = 0; j < cluster_map.cols; ++j) {  
        //         int l = cluster_map.at<uchar>(i, j) = matAsignaciones.at<int>(i,j);
        //         if (prev_label != cluster_map.at<unsigned char>(i, j)){
        //             prev_label = cluster_map.at<uchar>(i, j);
        //         }
        //     }   
        // }
        
        // std::cout << "Reshaped matrix size: " << cluster_map.size() << std::endl;

        // cv::Mat result_image(715,1096, CV_8UC3);
        // cv::Vec3b colors[] = {
        //     cv::Vec3b(0, 0, 255),   // Rojo
        //     cv::Vec3b(0, 255, 0),   // Verde
        //     cv::Vec3b(255, 0, 0),   // Azul
        //     cv::Vec3b(255, 255, 0), // Amarillo
        //     cv::Vec3b(255, 0, 255), // Magenta
        //     cv::Vec3b(0, 255, 255), // Cian
        //     cv::Vec3b(255, 255, 255),// Blanco
        //     cv::Vec3b(0, 0, 0),     // Negro
        //     cv::Vec3b(128, 128, 128),// Gris
        //     cv::Vec3b(64, 64, 64)   // Gris oscuro
        // };
        // index = 0;
        // for (int i = 0; i < result_image.rows; ++i) {
        //     for (int j = 0; j < result_image.cols; ++j) {
        //         uchar cluster_index = cluster_map.at<unsigned char>(index++);
        //         result_image.at<cv::Vec3b>(i, j) = colors[cluster_index];
        //     }
        // }

        // cv::Mat resultT;
        // transpose(result_image,resultT); 
        // cv::imwrite("../results/hartigan_omp_" + std::to_string(K) + ".jpeg",resultT);

        // // Mat imagen(height, width, CV_8UC3);
        // // for(int i = 0; i < height; ++i) {
        // //     for(int j = 0; j < width; ++j) {
        // //         int index = i * width + j;
        // //         imagen.at<Vec3b>(i, j) = colors[asignaciones[index]];
        // //     }
        // // }
        // // cv::imwrite("results/hartigan_omp_" + std::to_string(K) + ".png", imagen);

        // // return 0;


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
