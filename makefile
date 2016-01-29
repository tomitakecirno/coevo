# Makefile for jgg

#gccでコンパイル
CC = gcc

#コンパイルするファイル名
FILE = jgg
FILE2 = prot
LINK = -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -L/usr/lib

JGG:$(FILE).c
	$(CC) -o $(FILE) $(FILE).c $(LINK) -g


prot:$(FILE2).c
	$(CC) -o $(FILE2) $(FILE2).c $(LINK) -g

