CXX=g++
CXXFLAGS=-std=c++11 -I /usr/include/eigen3 -fopenmp $(shell pkg-config --cflags --libs opencv4)
SRC=src/hartigan_omp.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=hartigan_omp

all: clean $(EXEC) run

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run:
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)
