# Makefile for jgg

#gccでコンパイル
CC = gcc

#コンパイルするファイル名
FILE = jgg
FILE2 = prot
FILE3 = bfs
LINK = -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -L/usr/lib
LINK2 = -lm -lSDL2 -L/usr/lib

jgg:$(FILE).c
	$(CC) -o $(FILE) $(FILE).c $(LINK) -g


prot:$(FILE2).c
	$(CC) -o $(FILE2) $(FILE2).c $(LINK2) -g

bfs:$(FILE3).c
	$(CC) -o $(FILE3) $(FILE3).c $(LINK) -g
