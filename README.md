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

### Secuencial

```bash
python3 hartigan_seq.py
```

### MPI

```bash
mpiexec --allow-run-as-root -n 4 python3 hartigan_mpi.py
```

### OMP

```bash
make
```
