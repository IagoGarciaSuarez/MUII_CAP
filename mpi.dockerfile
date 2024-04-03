FROM ubuntu:latest

WORKDIR /app

RUN apt update && apt install -y python3-pip python3-dev libopenmpi-dev openmpi-bin && apt clean && rm -rf /var/lib/apt/lists/*

RUN pip3 install --upgrade pip 

RUN pip3 install mpi4py numpy Pillow