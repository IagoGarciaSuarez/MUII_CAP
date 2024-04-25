import matplotlib.pyplot as plt

def plot_performance():
    # Tiempos para K = 3, 6 y 9
    # exec_times_hartigan_seq = [15.56712818145752, 23.76786470413208, 31.83247709274292]
    # exec_times_hartigan_mp = [4.559414625167847, 6.4686057567596436, 8.601719617843628]
    exec_times_hartigan_omp_2 = [9.44953, 14.8172, 19.9216]
    exec_times_hartigan_omp_4 = [6.43763, 7.93107, 9.6626]
    exec_times_hartigan_omp_6 = [4.4546, 6.24518, 8.01287]
    n_clusters = [3, 6, 9]

    plt.figure(figsize=(10, 6))

    # plt.plot(n_clusters, exec_times_hartigan_seq, label='Secuencial', marker='o')
    # plt.plot(n_clusters, exec_times_hartigan_mp, label='MPI (4 CPUs)', marker='s')
    plt.plot(n_clusters, exec_times_hartigan_omp_2, label='OMP (2 CPUs)', marker='s')
    plt.plot(n_clusters, exec_times_hartigan_omp_4, label='OMP (4 CPUs)', marker='s')
    plt.plot(n_clusters, exec_times_hartigan_omp_6, label='OMP (6 CPUs)', marker='s')

    plt.title('Rendimiento de Harigan OMP 2v4v6 hilos')
    plt.xlabel('Nº de clusters')
    plt.ylabel('Tiempo de ejecución (s)')

    plt.legend()
    plt.grid(True)

    plt.savefig('benchmark/performance_comparison_threads.png', dpi=300)

if __name__ == "__main__":
    plot_performance()
