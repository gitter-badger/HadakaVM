# Licensed under the GNU GPL v3+, see LICENSE

CXX_FLAGS = -std=c++11 -g -Iinclude -Wall -Wextra -pedantic
LD_FLAGS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image

CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix build/obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all: build build/obj build/HadakaVM
build/HadakaVM: $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) -o $@ $^
build/obj/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $^
build:
	-@mkdir build/
build/obj:
	-@mkdir build/obj
clean:
	-@rm -r build/obj/*.o
	-@rm -r build/HadakaVM*
