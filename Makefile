main:
	clang++ -std=c++17 $(pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf lua5.4) src/*.cpp lib/lua/*.c lib/box2d/src/**/*.cpp -Wno-deprecated -I./ -I./lib/ -I./lib/boost/ -I./SDL2/ -I./SDL2_image/ -I./SDL2_mixer/ -I./SDL2_ttf/ -I./src/  -I./lib/rapidjson/ -I./lib/glm/ -I./lib/glm/gtx/ -I./lib/sol/ -I./lib/lua/ -I./lib/box2d/src/ -I./lib/box2d/include/ -I./lib/box2d/include/box2d/ -L./ -llua5.4 -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -O3 -o game_engine_linux
clean:
	rm -f $(OBJECTS) game_engine_linux
