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
	int qtdRecursos; //3 - configbase
	int qtdUtilizadores;
	int maxUtilizadores; //100 - configbase
	struct recurso *recursos;
	struct utilizador *utilizadores;
	struct utilizador *filaUsersAq; //20
};

struct recurso
{
	int numRecurso;
	int numUtilizadoresDentro;
	int numUtilizadoresFila;
	int numMaxUtilizadoresDoRecurso;
	bool recursoOcupado;
	struct utilizador *userFila;
	struct utilizador *userRec;
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
