# Hartigan Dockerizado

## MPI

### Build

Para construir la imagen:

```bash
docker build -f mpi.dockerfile -t hartigan-mpi .
```

### Run

Para ejecutar el contenedor desde el directorio del repositorio:

```bash
docker run --rm -it -v $(pwd):/app hartigan-mpi
```

### Ejecución del código

```bash
mpiexec --allow-run-as-root -n 8 python3 hartigan_mpi.py
```
