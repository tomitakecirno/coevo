# Makefile for jgg

#gccでコンパイル
CC = gcc

#コンパイルするファイル名
FILE = jgg
FILE2 = sdltest
FILE3 = jgg_backup
LINK = -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -L/usr/lib

JGG:$(FILE).c
	$(CC) -o $(FILE) $(FILE).c $(LINK)


sdl:$(FILE2).c
	$(CC) -o $(FILE2) $(FILE2).c $(LINK)

backup:$(FILE3).c
	$(CC) -o $(FILE3) $(FILE3).c $(LINK)

