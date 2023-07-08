OBJS = main.c
O_NAME = a
GCC = gcc
GCCFLAGS = -std=c11
PARAMS = -Wall -lmingw32 -lSDL2main -lSDL2 -o

all: $(OBJS)
	gcc -std=c11 main.c -IC:\SDL\SDL2-2.26.5\i686-w64-mingw32\include -LC:\SDL\SDL2-2.26.5\i686-w64-mingw32\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o a