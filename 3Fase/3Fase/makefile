all: simulador monitor

monitor: monitor.o
	gcc monitor.c -lpthread -o monitor

simulador: simulador.o
	gcc simulador.c -lpthread -o simulador

clean:
	rm -f *.o *~ monitor simulador
	rm saida.txt
