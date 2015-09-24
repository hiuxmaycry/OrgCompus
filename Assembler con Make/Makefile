all:
	gcc -Wall -c parametros.c -o parametros.o
	gcc -Wall -std=c99 -c automata.c -o automata.o
	gcc -Wall -c tp1.c -o tp1.o
	gcc -c -o0 proximo.S -o proximo.o
	gcc -Wall tp1.o parametros.o automata.o proximo.o -o autcel
