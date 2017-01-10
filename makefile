# Makefile for jgg

#gccでコンパイル
CC = gcc
CPP = g++

#コンパイルするファイル名
FILE = jgg
FILE2 = prot
FILE3 = bfs
FILE4 = coansv2
FILE5 = coansv3
FILE6 = coansv2.1
LINK = -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -L/usr/lib
LINK2 = -lm -lSDL2 -L/usr/lib

coans:$(FILE6).cpp
	$(CPP) -o $(FILE6) $(FILE6).cpp $(LINK) -g

v3:$(FILE5).c
	$(CC) -o $(FILE5) $(FILE5).c $(LINK) -g

jgg:$(FILE).c
	$(CC) -o $(FILE) $(FILE).c $(LINK) -g


prot:$(FILE2).c
	$(CC) -o $(FILE2) $(FILE2).c $(LINK2) -g

bfs:$(FILE3).c
	$(CC) -o $(FILE3) $(FILE3).c $(LINK) -g
