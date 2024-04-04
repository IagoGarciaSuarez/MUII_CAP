FROM ubuntu:latest

WORKDIR /hartigan

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y libeigen3-dev libopencv-dev libopenmpi-dev openmpi-bin \
                    python3-pip python3-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install --upgrade pip && pip3 install mpi4py numpy Pillow matplotlib

RUN export CPLUS_INCLUDE_PATH=/usr/include/opencv4 && export LIBRARY_PATH=/usr/lib