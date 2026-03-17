CC = /mingw64/bin/gcc
CFLAGS = -I C:/cygwin64/mingw64/include/ -L C:/cygwin64/mingw64/lib/ -g -Wall -Wextra -Werror -std=c99 -pedantic
LINKFLAGS = -lSDL3.dll -lSDL3_image.dll -lSDL3_ttf.dll -lSDL3_mixer.dll

game.exe: src/window.c src/engine.c src/management.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) $^ -o $@ $(LINKFLAGS)

src/window.c: src/include/*
	touch src/window.c

src/engine.c: src/engine/*
	touch src/engine.c

src/management.c: src/management/*
	touch src/management.c
