CC = g++
CC_FLAGS = -std=c++11 -g
LD_FLAGS = -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image
INCLUDE_DIR = include/

all: md_build
	$(CC) src/*.cpp -o build/HadakaVM -I$(INCLUDE_DIR) $(SDL_INCLUDE_DIRS) $(SDL_FLAGS) $(SDL_LIBRARY_DIR) $(CC_FLAGS) $(SDL_LIBRARY_DIR) $(LD_FLAGS)
md_build:
	-@mkdir build
clean:
	rm build/HadakaVM.exe
