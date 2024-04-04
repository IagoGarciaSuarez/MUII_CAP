# CXX=g++
# CXXFLAGS=-std=c++20 -O2 $(pkg-config --cflags --libs opencv4) -fopenmp -I /usr/include/eigen3
# LDFLAGS=
# SOURCES=src/hartigan_omp.cpp
# OBJECTS=$(SOURCES:.cpp=.o)
# EXECUTABLE=hartigan_omp

# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

# .cpp.o:
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJECTS) $(EXECUTABLE)

# run:
# 	./$(EXECUTABLE) && rm $(OBJECTS)

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
	./$(EXEC)
