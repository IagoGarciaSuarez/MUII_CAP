CXX=g++
CXXFLAGS=-std=c++11 -I /usr/include/eigen3
SRCDIR=src/
SRC=hartigan_omp.cpp
OBJ=$(SRCDIR)$(SRC:.cpp=.o)
EXEC=hartigan_omp

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	export CPLUS_INCLUDE_PATH=/usr/include/opencv4 && export LIBRARY_PATH=/usr/lib && ./$(EXEC)

