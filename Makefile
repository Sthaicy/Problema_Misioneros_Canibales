CXX      = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Iinclude
SRC      = $(wildcard src/*.cpp src/*/*.cpp)
OBJ      = $(SRC:src/%.cpp=build/%.o)

build/misioneros: $(OBJ)
	$(CXX) $(OBJ) -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

.PHONY: clean
