import numpy as np
import time
from PIL import Image

K = 3
HARTIGAN_THRESHOLD = 50.0
ITERACIONES_THRESHOLD = 10000000
FILE_PATH = "pavia.txt"
ROWS = 1096
COLS = 715

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
    print("========== SECUENCIAL ==========")

    print(f"---- LOADING DATA FROM {FILE_PATH} ----")
    pixels = load_data(FILE_PATH)
    print(f"---- {FILE_PATH} LOADED ----")
    print("Calculando...")
    
    t_init = time.time()
    asignaciones = np.random.randint(0, K, size=(ROWS * COLS))
    centroides = []
    for i in range(K):
        centroides.append(np.mean(pixels[asignaciones == i], axis = 0))

    while 1:
        centroides_anterior = centroides.copy()

        # Calculamos para cada pixel la distancia a cada centroide y obtenemos
        # el índice de la menor distancia. Este índice corresponde al número del
        # cluster al que deben ser asignados, así que se asigna a él.
        for p, i in zip(pixels, range(len(pixels))):
          distancias = []
          for c in centroides:
            distancias.append(distancia_cuadrada(c, p))
          min_dist_idx = np.argmin(distancias)
          asignaciones[i] = min_dist_idx

        # Recalculamos los centroides
        for i in range(len(centroides)):
            centroides[i] = np.mean(pixels[asignaciones == i], axis=0)

        # Calculamos la convergencia
        inercia_anterior = np.sum([distancia_cuadrada(pixels[i], centroides_anterior[asignaciones[i]]) for i in range(len(pixels))])
        inercia_actual = np.sum([distancia_cuadrada(pixels[i], centroides[asignaciones[i]]) for i in range(len(pixels))])
        if abs(inercia_anterior - inercia_actual) > HARTIGAN_THRESHOLD:
            break

    t_end = time.time()
    t_total = t_end - t_init
    print(f"K = {K}\tTiempo de ejecución = {t_total} segs ")

    cluster_map = np.reshape(asignaciones, (715,1096)).astype(np.uint8)
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
    image.save(f'results/hartigan_seq_{K}.jpeg')


    return t_total

if __name__ == '__main__':
    main()