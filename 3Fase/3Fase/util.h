// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> 	// Para resolver o erro "unknown type name 'bool'"							
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <semaphore.h>							
#include <pthread.h>
#include <ctype.h>
#include <time.h>

// Tempos
#define _TIMESPEC_DEFINED

// Cores
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

// Defines
#define UNIXSTR_PATH "/tmp/s.2083121"
#define LINHAMAX 512 // Tamanho máximo da linha	

#define MAX_USERS_AQUAPARQUE 20
#define MAX_FILA_AQUAPARQUE 10
#define MAX_RECURSO1 10
#define MAX_RECURSO2 5
#define MAX_RECURSO3 3
#define MAX_RECURSO4 8
#define MAX_FILA_RECURSO1 10
#define MAX_FILA_RECURSO2 10
#define MAX_FILA_RECURSO3 10
#define MAX_FILA_RECURSO4 10

// Estruturas de Dados
struct aquaparque
{
	int qtdRecursos;
	int qtdUtilizadores;
	int maxUtilizadores;
	struct recurso *recursos;
};

struct recurso
{
	int numRecurso;
	int numUtilizadoresDentro;
	int numUtilizadoresFila;
	int numMaxUtilizadoresDoRecurso;
};

struct utilizador
{
	int idUtilizador;
	int prioridade;
	int numRecursoEncontra;
	bool entrou;
	int tempoR1;
};

// Protótipos de Funções

void readFile();
void criarRecursos(int qtdRecursosCriar);
int numRecurso(int tiraRecurso1, int tiraRecurso2);
int valorPrioridade();
int probSair();
void userSai(struct utilizador user, int num);
struct utilizador criaUser();
void * recursos(void * pointer);
int tempoRecurso();
void infoSend(int socketfd, int state, int numUtilizadores, int qtdUtilizadores1, int qtdUtilizadores2, int qtdUtilizadores3, int qtdUtilizadores4);
int criaSocket();
void semInit();
void mutexesInit();
void valueInit();
void mudaOuSai(struct utilizador user, int lugar);
void entrarRec1(struct utilizador user);
void rec1(struct utilizador user);
void entrarRec2(struct utilizador user);
void rec2(struct utilizador user);
void entrarRec3(struct utilizador user);
void rec3(struct utilizador user);
void entrarRec4(struct utilizador user);
void rec4(struct utilizador user);
void filaPrio_Aquaparque(struct utilizador user);
void filaNaoPrio_Aquaparque(struct utilizador user);
int getValue(sem_t *sem);
void * users(void * pointer);
void sim(int socketfd);
