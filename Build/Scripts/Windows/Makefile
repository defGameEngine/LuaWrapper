CC=g++

DGE_PATH=..\..\..\..\defGameEngine
SOL2_INCLUDE=C:\SDK\sol2\include
GLFW_INCLUDE=C:\SDK\glfw\include
GLFW_LIB=C:\SDK\glfw\x64\lib-mingw-w64
STB_INCLUDE=C:\SDK\stb
LUA_LIB=C:\SDK\lua542
LUA_INCLUDE=C:\SDK\lua542\include
LIBS=-lgdi32 -luser32 -lkernel32 -lopengl32 -lglfw3dll -lglu32 -llua54

FLAGS=-Wall -pedantic -std=c++20 -fms-extensions
DEPENDENCIES=-I"$(LUA_INCLUDE)" -I"$(DGE_PATH)\Include" -I"$(SOL2_INCLUDE)" -I"$(STB_INCLUDE)" -I"$(GLFW_INCLUDE)" -L"$(GLFW_LIB)" -L"$(LUA_LIB)" $(LIBS)

ENGINE_SOURCES=$(DGE_PATH)\Sources\defGameEngine.cpp $(DGE_PATH)\Sources\Graphic.cpp $(DGE_PATH)\Sources\KeyState.cpp \
				$(DGE_PATH)\Sources\Pixel.cpp $(DGE_PATH)\Sources\PlatformGL.cpp $(DGE_PATH)\Sources\PlatformGLFW3.cpp \
				$(DGE_PATH)\Sources\Sprite.cpp $(DGE_PATH)\Sources\StbImage.cpp $(DGE_PATH)\Sources\Texture.cpp

SOURCES=..\..\..\Sources\Main.cpp ..\..\..\Sources\Constants.cpp ..\..\..\Sources\Types.cpp ..\..\..\Sources\Utils.cpp

.PHONY: all
all:
	$(CC) -std=c++20 $(FLAGS) $(DEPENDENCIES) -o ..\..\Target\defGameEngine $(SOURCES) $(ENGINE_SOURCES)
