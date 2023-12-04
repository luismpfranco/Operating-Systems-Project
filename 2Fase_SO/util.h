//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> 								
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>							
#include <pthread.h>
#include <ctype.h>

//Tempos
#define _TIMESPEC_DEFINED

#define UNIXSTR_PATH "/tmp/s.2083121"
#define LINHAMAX 512								

struct aquaparque
{
	int qtdRecursos;
	struct recurso *recursos;
	int rec1;
	int rec2;
	int rec3;
};

struct recurso
{
	int numRecurso;
	int numUtilizadoresDentro;
	int numUtilizadoresFila;
	int numMaxUtilizadoresDoRecurso;
	bool recursoOcupado;
};

struct utilizador
{
	int idUtilizador;
	int prioridade;
	int numRecursoEncontra;
	bool entrou;
	bool desistencia;
	time_t tmpEntradaFilaRecursos;
};
