from mpi4py import MPI
import numpy as np
import time
from PIL import Image


K = 9
HARTIGAN_THRESHOLD = 50.0
ITERACIONES_THRESHOLD = 10000000
FILE_PATH = "./pavia.txt"
ROWS = 1096
COLS = 715

# mpiexec --allow-run-as-root -n 8 python3 hartigan_mpi.py
# con 8 procesos da a 97955 elementos por proceso

# Función para cargar los datos desde un archivo
def load_data(file_path):
    data = np.loadtxt(file_path, delimiter=",")
    data_array = np.array(data)
    matrix = np.reshape(data_array, (ROWS * COLS, 102)) # 783640
    pixels = matrix.astype(np.float32)
    return pixels

# Función para calcular la distancia cuadrada entre dos vectores
def distancia_cuadrada(v1, v2):
    return np.sum((np.array(v1) - np.array(v2)) ** 2)

def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()
    size = comm.Get_size()

    if size not in [4, 8]:
        print("Error: Es necesario poner 4 u 8 cores.")
        return
    
    if rank == 0:
        t_init = None
        t_end = None
        t_total = None

        print("---- LOADING DATA ----")
        pixels = load_data(FILE_PATH)
        print("---- DATA LOADED ----")
        
        t_init = time.time()

        segment_size = pixels.shape[0] // size
        segmentos_pixels = [pixels[i*segment_size:(i+1)*segment_size] for i in range(size)]
        centroides = []
        asignaciones = np.random.randint(0, K, size=(ROWS * COLS))
        segmentos_asignaciones = [asignaciones[i*segment_size:(i+1)*segment_size] for i in range(size)]
        for i in range(K):
            centroides.append(np.mean(pixels[asignaciones == i], axis = 0))
    else:
        segmentos_pixels = None
        centroides = None
        segmentos_asignaciones = None
        asignaciones_recibidas = None

    pixeles_recibidos = comm.scatter(segmentos_pixels, root=0)
    asignaciones_recibidas = comm.scatter(segmentos_asignaciones, root=0)
    centroides = comm.bcast(centroides, root=0)
    
    while 1:
        centroides_anterior = centroides.copy()
        
        # Calculamos para cada pixel la distancia a cada centroide y obtenemos
        # el índice de la menor distancia. Este índice corresponde al número del
        # cluster al que deben ser asignados, así que se asigna a él.
        for p, i in zip(pixeles_recibidos, range(len(pixeles_recibidos))):
            distancias = []
            for c in centroides:
                distancias.append(distancia_cuadrada(c, p))
            min_dist_idx = np.argmin(distancias)
            asignaciones_recibidas[i] = min_dist_idx

        # Recalculamos los centroides
        for i in range(len(centroides)):
            centroides[i] = np.mean(pixeles_recibidos[asignaciones_recibidas == i], axis=0)

        # Calculamos la convergencia
        inercia_anterior = np.sum([distancia_cuadrada(pixeles_recibidos[i], centroides_anterior[asignaciones_recibidas[i]]) for i in range(len(pixeles_recibidos))])
        inercia_actual = np.sum([distancia_cuadrada(pixeles_recibidos[i], centroides[asignaciones_recibidas[i]]) for i in range(len(pixeles_recibidos))])
        inercia_anterior_total = comm.allreduce(inercia_anterior, op=MPI.SUM)
        inercia_actual_total = comm.allreduce(inercia_actual, op=MPI.SUM)
        maxDelta = abs(inercia_anterior_total - inercia_actual_total)
        centroides_suma = comm.reduce(centroides, op=MPI.SUM, root=0)
        centroides = None
        if rank == 0:
            centroides = [c/size for c in centroides_suma]
        if maxDelta > HARTIGAN_THRESHOLD:
            break
        else:
            MPI.Barrier()
            centroides = comm.bcast(centroides, root=0)

    asignaciones_completas = comm.gather(asignaciones_recibidas, root=0)

    if rank == 0:
        asignaciones_flatten = np.concatenate(asignaciones_completas)
        t_end = time.time()
        t_total = t_end - t_init
        print(f"K = {K}\tTiempo de ejecución = {t_total} segs ")

        cluster_map = np.reshape(asignaciones_flatten, (715,1096)).astype(np.uint8)
        colors = [
            (0, 0, 255),   # Red
            (0, 255, 0),   # Green
            (255, 0, 0),   # Blue
            (255, 255, 0), # Yellow
            (255, 0, 255), # Magenta
            (0, 255, 255), # Cyan
            (255, 255, 255), # White
            (0, 0, 0),     # Black
            (128, 128, 128), # Gray
            (64, 64, 64)   # Gray
        ]
        color_map_array = np.array(colors)

        # Asignar a cada pixel de la imagen de salida el color en finción de la etiqueta
        result_image = np.zeros((1096,715, 3), dtype=np.uint8)

        for i in range(result_image.shape[0]):
            for j in range(result_image.shape[1]):
                result_image[i, j] = color_map_array[cluster_map[j, i]]
        
        image = Image.fromarray(result_image)

        # Guardar la imagen en un archivo
        image.save(f'results/hartigan_mpi_k_{K}.jpeg')

    MPI.Finalize()

if __name__ == '__main__':
    main()