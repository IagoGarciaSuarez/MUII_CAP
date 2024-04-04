# Hartigan Dockerizado

## Build

Para construir la imagen:

```bash
docker build -t hartigan .
```

## Run

Para ejecutar el contenedor desde el directorio del repositorio:

```bash
docker run --rm -it -v $(pwd):/hartigan hartigan
```

## Ejecución del código

En el código, modificar la ruta del archivo para apuntar a pavia.txt, o situar el archivo en el directorio del repositorio. 

Es necesario modificar también K en la constante global según se desee.

### Todos
```bash
make
```

### Secuencial

```bash
make seq
```

### MPI

```bash
make mpi
```

### OMP

```bash
make omp
```

## Graficar rendimiento

En el archivo plot_performance.py modificar las listas con los tiempos obtenidos y ejecutarlo con

```bash
python3 plot_performance.py
```
