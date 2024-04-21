# Hartigan Dockerizado

## Pull o Build

Para hacer pull de la imagen:

- Para Windows/Linux/MacOS Intel

```bash
docker pull meninodockerino/hartigan-kmeans
```

- Para MacOS Silicon

```bash
docker pull iagogarsu/hartigan-kmeans
```

O en caso de que se prefiera hacer la build:

```bash
docker build -t hartigan-kmeans .
```

## Run

Para ejecutar el contenedor desde el directorio del repositorio:

```bash
docker run --rm -it -v $(pwd):/hartigan [iagogarsu/hartigan-kmeans | meninodockerino/hartigan-kmeans]
```

o si se hizo build:

```bash
docker run --rm -it -v $(pwd):/hartigan hartigan-kmeans
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
