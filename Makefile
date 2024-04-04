CXX=g++
CXXFLAGS=-std=c++11 -I /usr/include/eigen3 -fopenmp $(shell pkg-config --cflags --libs opencv4)
SRC=src/hartigan_omp.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=hartigan_omp

all: clean seq mpi $(EXEC) run

seq:
	python3 src/hartigan_seq.py

mpi:
	mpiexec --allow-run-as-root -n 4 python3 src/hartigan_mpi.py

omp: 
	make $(EXEC) && make run

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run:
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
