//bibliotecas
#include <stdio.h> // Para funções de entrada/saída padrão como printf
#include <stdlib.h> // Para funções de manipulação de strings como atoi
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

//simulador/constantes
#define S_UTILIZADORES "Número de utilizadores no Parque Aquático - "
#define S_TEMPMEDIO "Tempo médio de chegada dos utilizadores - "
#define S_RECURSOS "Dimensão dos recursos - "
#define S_PROBDES "Probabilidade de desistência nas filas - "
#define S_INICIO "Início da simulação - "
#define S_TEMPSIM "Tempo da simulação - "

//funções
void lerFileConfig(char* nomeFile);

//variáveis
	int numUtilizadores;
	int numTempMedio;
	int numRecursos;
	int numProbDes;
	int numInicio;
	int numTempSim;
