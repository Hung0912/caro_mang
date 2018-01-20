CC=gcc

all: caro

caro: game.o caroAlgo.o
	gcc `pkg-config --cflags gtk+-3.0` -o caro game.o caroAlgo.o `pkg-config --libs gtk+-3.0`

game.o: game.c
	gcc `pkg-config --cflags gtk+-3.0` -c game.c `pkg-config --libs gtk+-3.0`

caroAlgo.o: caroAlgo.c
	gcc -c caroAlgo.c
