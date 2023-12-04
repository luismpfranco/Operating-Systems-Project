projeto1: monitor.o main.o simulador.o
	gcc monitor.o main.o simulador.o -o projeto1

monitor: monitor.c monitor.h
	gcc -c monitor.c

main: monitor.h simulador.h main.c
	gcc -c main.c

simulador: simulador.h simulador.c
	gcc -c simulador.c