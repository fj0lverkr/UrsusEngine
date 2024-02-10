#OBJS specifies which files to compile as part of the project
OBJS = main.cpp src/Game.cpp src/TextureManager.cpp src/TiledMap.cpp src/Vector2D.cpp src/Collision.cpp src/Camera2D.cpp src/ECS/ECS.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I C:/src/SDL2/include -I C:/src/boost -I C:/src/tmxlite/include -I src

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L C:/src/SDL2/lib -L C:/src/tmxlite/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window, but we need the console to see our couts  -Wl,-subsystem,windows
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lbcrypt -ltmxlite

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
