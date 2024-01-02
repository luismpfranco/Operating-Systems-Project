#include "util.h"

// Socket
int socketfd = 0;

// Inicialização da struct global
struct aquaparque meuAquaparque;

// Declaração de semáforos para controlar o acesso a recursos compartilhados.
sem_t infoSendT;                  		// Controlar o número de mensagens enviadas por vez

sem_t semFilaNP_Aquaparque;       		// Controlar o número de pessoas não prioritárias na fila do aquaparque
sem_t semFilaP_Aquaparque;        		// Controlar o número de pessoas prioritárias na fila do aquaparque
sem_t semEsperaNP_Aquaparque;     		// Bloquear os processos não prioritários quando houver processos prioritários na fila prioritária

sem_t semNumUsersAquaparque;      		// Controlar o número de pessoas que entram no aquaparque

sem_t semNumUsersRec1;            		// Controlar o número de pessoas que entram no Recurso 1
sem_t semNumUsersRec2; 			  		// Controlar o número de pessoas que entram no Recurso 2
sem_t semNumUsersRec3; 			  		// Controlar o número de pessoas que entram no Recurso 3
sem_t semNumUsersRec4; 			  		// Controlar o número de pessoas que entram no Recurso 4

sem_t semFilaRec1; 				  		// Controlar o número de pessoas na fila do Recurso 1
sem_t semEsperaFilaRec1; 		  		// Bloquear os utilizadores quando houver utilizadores na fila do Recurso 1

sem_t semFilaRec2; 				  		// Controlar o número de pessoas na fila do Recurso 1
sem_t semEsperaFilaRec2; 		  		// Bloquear os utilizadores quando houver utilizadores na fila do Recurso 2

sem_t semFilaRec3; 				  		// Controlar o número de pessoas na fila do Recurso 3
sem_t semEsperaFilaRec3; 		  		// Bloquear os utilizadores quando houver utilizadores na fila do Recurso 3

sem_t semFilaRec4; 				  		// Controlar o número de pessoas na fila do Recurso 4
sem_t semEsperaFilaRec4; 		  		// Bloquear os utilizadores quando houver utilizadores na fila do Recurso 4


// Declaração de trincos (mutexes) para garantir acesso exclusivo a seções críticas do código.
pthread_mutex_t trincoUser;				// Controlar a criação da pessoa
pthread_mutex_t order;					// Controlar a entrada de cada pessoa no aquaparque
pthread_mutex_t orderP;					// Controlar a entrada de cada pessoa na fila prioritária do aquaparque
pthread_mutex_t orderNP;				// Controlar a entrada de cada pessoa na fila NÃO prioritária do aquaparque
pthread_mutex_t trincoNumUsers;			// Controlar o número de utilizadores no aquaparque
pthread_mutex_t trincoNumUsersEntrada;	// Controlar a entrada de utilizadores no aquaparque

pthread_mutex_t trincoUserRec1; 		// Controlar a colocação da pessoa no Recurso 1
pthread_mutex_t trincoUserFilaRec1; 	// Controlar a colocação da pessoa na Fila do Recurso 1

pthread_mutex_t trincoUserRec2; 		// Controlar a colocação da pessoa no Recurso 1
pthread_mutex_t trincoUserFilaRec2; 	// Controlar a colocação da pessoa na Fila do Recurso 1

pthread_mutex_t trincoUserRec3; 		// Controlar a colocação da pessoa no Recurso 3
pthread_mutex_t trincoUserFilaRec3; 	// Controlar a colocação da pessoa na Fila do Recurso 3

pthread_mutex_t trincoUserRec4; 		// Controlar a colocação da pessoa no Recurso 4
pthread_mutex_t trincoUserFilaRec4; 	// Controlar a colocação da pessoa na Fila do Recurso 4

pthread_mutex_t trincoDuracaoRec1;		// Controlar a duração da pessoa no Recurso 1
pthread_mutex_t trincoDuracaoRec2;		// Controlar a duração da pessoa no Recurso 2
pthread_mutex_t trincoDuracaoRec3;		// Controlar a duração da pessoa no Recurso 3
pthread_mutex_t trincoDuracaoRec4;		// Controlar a duração da pessoa no Recurso 4


// Declaração de identificadores de thread para recursos e utilizadores.
pthread_t tIdRecursos[4];
pthread_t tIdUtilizadores[200];

// Declaração de variáveis que serão lidas a partir do arquivo de configuração.
int numMaxUtilizadoresCriar = 0; 		// 50
int numMaxUtilizadoresRec1 = 0; 		// 10
int numMaxFilaRecurso1 = 0; 			// 10
int numMaxUtilizadoresRec2 = 0; 		// 5
int numMaxFilaRecurso2 = 0; 			// 10
int numMaxUtilizadoresRec3 = 0; 		// 3
int numMaxFilaRecurso3 = 0; 			// 10
int numMaxUtilizadoresRec4 = 0; 		// 8
int numMaxFilaRecurso4 = 0; 			// 10
int quantidadeRecursos = 0; 			// 4
int maximoFila = 0; 					// 10


// Declaração de variáveis globais utilizadas na simulação.
int idUser = 0; 						// Inicia a 0
int tempoR1 = 0;						// Vai ser aleatório para cada utilizador
int tempoR2 = 5;
int tempoR3 = 10;
int tempoR4 = 5;

int vagasAquaparque = 0; 				// = numMaxUtilizadores = 20

int vagasRec1 = 0; 						// = numMaxUtilizadoresRec1 = 10
int vagasFilaRec1 = 0; 					// = numMaxFilaRecurso1 = 10

int vagasRec2 = 0;						// = numMaxUsersRec2 = 5
int vagasFilaRec2 = 0;					// = numMaxFilaRec2 = 10

int vagasRec3 = 0; 						// = numMaxUtilizadoresRec3 = 3
int vagasFilaRec3 = 0; 					// = numMaxFilaRecurso3 = 10

int vagasRec4 = 0; 						// = numMaxUtilizadoresRec4 = 8
int vagasFilaRec4 = 0; 					// = numMaxFilaRecurso4 = 10

int filaP_Aquaparque = 0; 				// = maximoFila = 10
int numUsersP = 0;

int filaNP_Aquaparque = 0;  			// = maximoFila = 10
int numUsersNP = 0;

int j = 0;

// Variáveis que serão enviadas para o monitor
int numUsers = 0; 						// Inicia a 0
int numUsers1 = 0; 						// Inicia a 0
int numUsers2 = 0; 						// Inicia a 0
int numUsers3 = 0; 						// Inicia a 0
int numUsers4 = 0; 						// Inicia a 0


// Função para ler as configurações do arquivo "configbase.txt" e inicializar as variáveis globais.
void readFile()
{
	FILE* configuracao = fopen("configbase.txt", "r");

	if(configuracao != NULL)
	{
		char line[50];
		int value;
		char param[50];

		while(fgets(line, sizeof(line), configuracao) != NULL)
		{
			sscanf(line, "%s : %d", param , &value);
			
			if(strcmp(param, "numMaxUtilizadores") == 0)
			{
				vagasAquaparque = value;
			}
			if(strcmp(param, "numMaxUtilizadoresCriar") == 0)
			{
				numMaxUtilizadoresCriar = value;
			}
			if(strcmp(param, "numMaxUsersRec1") == 0)
			{
				numMaxUtilizadoresRec1 = value;
				vagasRec1 = numMaxUtilizadoresRec1;
			}
			if(strcmp(param, "numMaxFilaRec1") == 0)
			{
				numMaxFilaRecurso1 = value;
				vagasFilaRec1 = numMaxFilaRecurso1;
			}
			if(strcmp(param, "numMaxUsersRec2") == 0)
			{
				numMaxUtilizadoresRec2 = value;
				vagasRec2 = numMaxUtilizadoresRec2;
			}
			if(strcmp(param, "numMaxFilaRec2") == 0)
			{
				numMaxFilaRecurso2 = value;
				vagasFilaRec2 = numMaxFilaRecurso2;
			}
			if(strcmp(param, "numMaxUsersRec3") == 0)
			{
				numMaxUtilizadoresRec3 = value;
				vagasRec3 = numMaxUtilizadoresRec3;
			}
			if(strcmp(param, "numMaxFilaRec3") == 0)
			{
				numMaxFilaRecurso3 = value;
				vagasFilaRec3 = numMaxFilaRecurso3;
			}
			if(strcmp(param, "numMaxUsersRec4") == 0)
			{
				numMaxUtilizadoresRec4 = value;
				vagasRec4 = numMaxUtilizadoresRec4;
			}
			if(strcmp(param, "numMaxFilaRec4") == 0)
			{
				numMaxFilaRecurso4 = value;
				vagasFilaRec4 = numMaxFilaRecurso4;
			}
			if(strcmp(param, "numRecursos") == 0)
			{
				quantidadeRecursos = value;
			}
			if(strcmp(param, "maxFilaAquaparque") == 0)
			{
				maximoFila = value;
				filaP_Aquaparque = maximoFila;
				filaNP_Aquaparque = maximoFila;
			}
		}
		fclose(configuracao);
	}
	else 
	{
		perror("Erro ao abrir o ficheiro de configuração");
    	exit(EXIT_FAILURE);
	}
}

// Função para inicializar a estrutura do aquaparque e seus recursos com base na quantidade fornecida.
void criarRecursos(int qtdRecursosCriar)
{
	meuAquaparque.qtdRecursos = qtdRecursosCriar;
	meuAquaparque.recursos = malloc(meuAquaparque.qtdRecursos * sizeof(struct recurso));
	
	for(int i = 0; i < qtdRecursosCriar; i++)
	{
		meuAquaparque.recursos[i].numRecurso = i + 1;
		meuAquaparque.recursos[i].numUtilizadoresDentro = 0;

		if(i == 0)
		{
			meuAquaparque.recursos[i].numMaxUtilizadoresDoRecurso = numMaxUtilizadoresRec1;
			meuAquaparque.recursos[i].numUtilizadoresFila = numMaxFilaRecurso1;
		}
		else if(i == 1)
		{
			meuAquaparque.recursos[i].numMaxUtilizadoresDoRecurso = numMaxUtilizadoresRec2;
			meuAquaparque.recursos[i].numUtilizadoresFila = numMaxFilaRecurso2;
		}
		else if(i == 2)
		{
			meuAquaparque.recursos[i].numMaxUtilizadoresDoRecurso = numMaxUtilizadoresRec3;
			meuAquaparque.recursos[i].numUtilizadoresFila = numMaxFilaRecurso3;
		}
		else if(i == 3)
		{
			meuAquaparque.recursos[i].numMaxUtilizadoresDoRecurso = numMaxUtilizadoresRec4;
			meuAquaparque.recursos[i].numUtilizadoresFila = numMaxFilaRecurso4;
		}
	}
}

// Função para determinar um número de recurso aleatório, removendo os valores especificados por tiraRecurso1 e tiraRecurso2.
int numRecurso(int tiraRecurso1, int tiraRecurso2)
{
    int valRecurso[5] = {1, 2, 3, 4, 5};

    // Retira o valor de tiraRecurso1 do array valRecurso
    for (int i = 0; i < 5; ++i)
    {
        if (valRecurso[i] == tiraRecurso1)
        {
            for (int j = i; j < 4; ++j)
            {
                valRecurso[j] = valRecurso[j + 1];
            }
            break;
        }
    }

    // Retira o valor de tiraRecurso2 do array valRecurso
    for (int i = 0; i < 4; ++i)
    {
        if (valRecurso[i] == tiraRecurso2)
        {
            for (int j = i; j < 3; ++j)
            {
                valRecurso[j] = valRecurso[j + 1];
            }
            break;
        }
    }

    int numRemocoes = (tiraRecurso1 != 0) + (tiraRecurso2 != 0); // Conta o número de retiradas
    int valRecRandom = valRecurso[rand() % (5 - numRemocoes)];
    return valRecRandom;
}

// Função para determinar o valor de prioridade de um recurso com base no número do recurso.
int valorPrioridade()
{
	int valorPrioridade[2] = {0, 1};
	int valorPrioRandom = valorPrioridade[rand() % 2];
	return valorPrioRandom;
}

// Função para simular a probabilidade de um utilizador sair.
int probSair()
{
	return rand() % 3;
}

// Função para que um utilizador saia do aquaparque ou escolha um novo recurso.
void userSai(struct utilizador user, int num)
{
	int numProbSair = probSair();

	if(numProbSair == 0)
	{
		user.numRecursoEncontra = 5;
		mudaOuSai(user, 5);
	}
	else
	{
		user.numRecursoEncontra = numRecurso(num, 5);
		int valor = user.numRecursoEncontra;
		mudaOuSai(user, valor);
	}
}

// Cria um novo utilizador com id, prioridade, número de recurso, estado de se já entrou no recurso ou não.
struct utilizador criaUser()
{
	pthread_mutex_lock(&trincoUser);
	
	struct utilizador user;
	user.idUtilizador = idUser;
	
	user.prioridade = valorPrioridade();

	user.entrou = false;
	user.tempoR1 = tempoRecurso();

	user.numRecursoEncontra = numRecurso(0, 5);

	idUser++;
	
	pthread_mutex_unlock(&trincoUser);
	return user;
}

// Imprime informações sobre os recursos na consola.
void * recursos(void * pointer)
{
	int *numeroRecurso = (int*) pointer;
	struct recurso *recursosNovos = &(meuAquaparque.recursos[*numeroRecurso]);
	
	printf(WHT "Recurso %d: Pode conter, no máximo, %2d pessoas dentro e %2d pessoas na fila.\n" RESET, recursosNovos->numRecurso, recursosNovos->numMaxUtilizadoresDoRecurso, recursosNovos->numUtilizadoresFila);
}

// Retorna um valor de tempo aleatório para simular o tempo de uso do recurso.
int tempoRecurso()
{
	return rand() % 16 + 5;
}

// Envia informações formatadas sobre o estado da simulação, o número de utilizadores totais, dos recursos 1, 2, 3 e 4 para o servidor através de sockets
void infoSend(int socketfd, int state, int numUtilizadores, int qtdUtilizadores1, int qtdUtilizadores2, int qtdUtilizadores3, int qtdUtilizadores4)
{
	sem_wait(&infoSendT);

    char buffer[LINHAMAX];
	int lenMsg = 0;
	
	sprintf(buffer, "%d %d %d %d %d %d", state, numUtilizadores, qtdUtilizadores1, qtdUtilizadores2, qtdUtilizadores3, qtdUtilizadores4);
	lenMsg = strlen(buffer) + 1;

	if(send(socketfd, buffer, lenMsg, 0) != lenMsg)
	{
		perror("Ocorreu um erro ao enviar os dados\n");
	}
	sleep(1);
	sem_post(&infoSendT);
}

// Cria um socket Unix e conecta ao servidor, retornando o descritor do socket.
int criaSocket()
{
     struct sockaddr_un addrServer;
     int lenServer;

     if ((socketfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	 {
		 perror(" Ocorreu um erro ao criar um socket\n");
	 }

    bzero((char*)&addrServer, sizeof(addrServer));

    addrServer.sun_family = AF_UNIX;
	strcpy(addrServer.sun_path, UNIXSTR_PATH);
   	lenServer = strlen(addrServer.sun_path) + sizeof(addrServer.sun_family);

   	bool simWorking = false;
   	while (connect(socketfd, (struct sockaddr *) &addrServer, lenServer) < 0)
	{
   		if(simWorking == false)
		{
			printf(WHT "Esperando pelo monitor\n" RESET);
			simWorking = true;
		}
	}

	if(socketfd < 0)
	{
		perror ("Erro aceite");
	}
	return socketfd;
}

// Inicializa os semáforos.
void semInit()
{
	sem_init(&infoSendT, 0, 1);
	sem_init(&semFilaNP_Aquaparque, 0, MAX_FILA_AQUAPARQUE);
	sem_init(&semFilaP_Aquaparque, 0, MAX_FILA_AQUAPARQUE);
	sem_init(&semEsperaNP_Aquaparque, 0, 1);
	
	sem_init(&semNumUsersAquaparque, 0, MAX_USERS_AQUAPARQUE);
	
	sem_init(&semNumUsersRec1, 0, MAX_RECURSO1);
	sem_init(&semNumUsersRec2, 0, MAX_RECURSO2);
	sem_init(&semNumUsersRec3, 0, MAX_RECURSO3);
	sem_init(&semNumUsersRec4, 0, MAX_RECURSO4);

	sem_init(&semEsperaFilaRec1, 0, 1);
	sem_init(&semFilaRec1, 0, MAX_FILA_RECURSO1);

	sem_init(&semEsperaFilaRec2, 0, 1);
	sem_init(&semFilaRec2, 0, MAX_FILA_RECURSO2);

	sem_init(&semEsperaFilaRec3, 0, 1);
	sem_init(&semFilaRec3, 0, MAX_FILA_RECURSO3);

	sem_init(&semEsperaFilaRec4, 0, 1);
	sem_init(&semFilaRec4, 0, MAX_FILA_RECURSO4);
}

// Inicializa os mutexes.
void mutexesInit()
{
	pthread_mutex_init(&trincoUser, NULL);
	pthread_mutex_init(&order, NULL);
	pthread_mutex_init(&orderP, NULL);
	pthread_mutex_init(&orderNP, NULL);
	pthread_mutex_init(&trincoNumUsers, NULL);
	pthread_mutex_init(&trincoNumUsersEntrada, NULL);
	pthread_mutex_init(&trincoUserRec1, NULL);
	pthread_mutex_init(&trincoUserFilaRec1, NULL);
	pthread_mutex_init(&trincoUserRec2, NULL);
	pthread_mutex_init(&trincoUserFilaRec2, NULL);
	pthread_mutex_init(&trincoUserRec3, NULL);
	pthread_mutex_init(&trincoUserFilaRec3, NULL);
	pthread_mutex_init(&trincoUserRec4, NULL);
	pthread_mutex_init(&trincoUserFilaRec4, NULL);
	pthread_mutex_init(&trincoDuracaoRec1, NULL);
	pthread_mutex_init(&trincoDuracaoRec2, NULL);
	pthread_mutex_init(&trincoDuracaoRec3, NULL);
	pthread_mutex_init(&trincoDuracaoRec4, NULL);
}

// Inicializa valores dos semáforos e faz a leitura das configurações base.
void valueInit()
{
	srand((unsigned)time(NULL));
	
	semInit();

	mutexesInit();
    
	readFile();
    printf(WHT "\nLeitura efetuada.\n" RESET);
	criarRecursos(4);
}

// Função principal para mudar ou sair do recurso atual do utilizador.
void mudaOuSai(struct utilizador user, int lugar)
{
	user.numRecursoEncontra = lugar;
	if(user.numRecursoEncontra == 1)
	{
		rec1(user);
	}
	else if(user.numRecursoEncontra == 2)
	{
		rec2(user);
	}
	else if(user.numRecursoEncontra == 3)
	{
		rec3(user);
	}
	else if(user.numRecursoEncontra == 4)
	{
		rec4(user);
	}
	else if(user.numRecursoEncontra == 5)
	{
		pthread_mutex_lock(&trincoNumUsers);
		sem_post(&semNumUsersAquaparque);
		numUsers--;
		vagasAquaparque++;
		int semValuer = getValue(&semNumUsersAquaparque);
		printf(MAG "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tO utilizador %2d saiu do Aquaparque\n" RESET, user.idUtilizador);
		infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
		pthread_mutex_unlock(&trincoNumUsers);

		pthread_exit(NULL);
	}
	else
	{
		printf(RED "Erro: O recurso do user %2d está a %2d\n" RESET, user.idUtilizador, user.numRecursoEncontra);
	}
}

// Função para entrada no recurso 1, levando em consideração prioridade e condições do recurso 1.
void entrarRec1(struct utilizador user)
{
	pthread_mutex_lock(&trincoUserRec1);

	numUsers1++;
	vagasRec1--;
	printf(CYN "\t\t\t\t\tO utilizador %2d entrou na piscina\n" RESET, user.idUtilizador);
	infoSend(socketfd, 3, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec1);

	pthread_mutex_lock(&trincoDuracaoRec1);
	sleep(user.tempoR1);
	pthread_mutex_unlock(&trincoDuracaoRec1);

	pthread_mutex_lock(&trincoUserRec1);
	numUsers1--;
	vagasRec1++;
	printf(MAG "\t\t\t\t\t\t\t\t\t\tO utilizador %2d vai sair da piscina\n" RESET, user.idUtilizador);
	infoSend(socketfd, 3, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec1);

	sem_post(&semNumUsersRec1);
}

// Função principal que simula a entrada ou não do utilizador no recurso 1.
void rec1(struct utilizador user)
{
	if (vagasRec1 > 0 && numUsers1 < numMaxUtilizadoresRec1 && vagasFilaRec1 == numMaxFilaRecurso1) 
	{
		sem_wait(&semNumUsersRec1);
		entrarRec1(user);
		userSai(user, 0);
	}
	else if(vagasRec1 == 0 || numUsers1 >= numMaxUtilizadoresRec1 || vagasFilaRec1 <= numMaxFilaRecurso1)
	{
		if(vagasFilaRec1 != 0)
		{
			sem_wait(&semFilaRec1);
			vagasFilaRec1--;
			
			printf(YEL "\t\t\t\t\tEntrada do utilizador %2d na fila da piscina\n" RESET, user.idUtilizador);

			sem_wait(&semEsperaFilaRec1);
			sem_wait(&semNumUsersRec1);

			sem_post(&semFilaRec1);
			vagasFilaRec1++;

			sem_post(&semEsperaFilaRec1);
			entrarRec1(user);

			userSai(user, 0);
		}
		else if(vagasFilaRec1 == 0)
		{
			printf(BLU "A piscina e a sua fila estão cheios\n" RESET);
			userSai(user, 1);
		}
		else
		{
			printf(RED "Erro: As vagas da fila da piscina estão a %2d\n" RESET, vagasFilaRec1);
		}
	}
	else
	{
		printf(RED "Erro: As vagas da piscina está a %2d e a quantidade de users na piscina está a %2d\n" RESET, vagasRec1, numUsers1);
	}
}

// Função para entrada no recurso 2, levando em consideração prioridade e condições do recurso 2.
void entrarRec2(struct utilizador user)
{
	pthread_mutex_lock(&trincoUserRec2);

	numUsers2++;
	vagasRec2--;
	printf(CYN "\t\t\t\t\tO utilizador %2d entrou no tobogã\n" RESET, user.idUtilizador);
	infoSend(socketfd, 4, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec2);

	pthread_mutex_lock(&trincoDuracaoRec2);
	sleep(tempoR2);
	pthread_mutex_unlock(&trincoDuracaoRec2);

	pthread_mutex_lock(&trincoUserRec2);
	numUsers2--;
	vagasRec2++;
	printf(MAG "\t\t\t\t\t\t\t\t\t\tO utilizador %2d vai sair do tobogã\n" RESET, user.idUtilizador);
	infoSend(socketfd, 4, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec2);

	sem_post(&semNumUsersRec2);
}

// Função principal que simula a entrada ou não do utilizador no recurso 2.
void rec2(struct utilizador user)
{
	if (vagasRec2 > 0 && numUsers2 < numMaxUtilizadoresRec2 && vagasFilaRec2 == numMaxFilaRecurso2) 
	{
		sem_wait(&semNumUsersRec2);
		entrarRec2(user);
		userSai(user, 0);
	}
	else if(vagasRec2 == 0 || numUsers2 >= numMaxUtilizadoresRec2 || vagasFilaRec2 <= numMaxFilaRecurso2)
	{
		if(vagasFilaRec2 != 0)
		{
			sem_wait(&semFilaRec2);
			vagasFilaRec2--;
			
			printf(YEL "\t\t\t\t\tEntrada do utilizador %2d na fila do tobogã\n" RESET, user.idUtilizador);
			
			sem_wait(&semEsperaFilaRec2);
			sem_wait(&semNumUsersRec2);

			sem_post(&semFilaRec2);
			vagasFilaRec2++;

			sem_post(&semEsperaFilaRec2);
			
			entrarRec2(user);

			userSai(user, 0);
		}
		else if(vagasFilaRec2 == 0)
		{
			printf(BLU "O tobogã e a sua fila estão cheios\n" RESET);
			userSai(user, 2);
		}
		else
		{
			printf(RED "Erro: As vagas da fila do tobogã estão a %2d\n" RESET, vagasFilaRec2);
		}
	}
	else
	{
		printf(RED "Erro: As vagas do tobogã está a %2d e a quantidade de users no tobogã está a %2d\n" RESET, vagasRec2, numUsers2);
	}
}

// Função para entrada no recurso 3, levando em consideração prioridade e condições do recurso 3.
void entrarRec3(struct utilizador user)
{
	pthread_mutex_lock(&trincoUserRec3);

	numUsers3++;
	vagasRec3--;
	printf(CYN "\t\t\t\t\tO utilizador %2d entrou no escorrega\n" RESET, user.idUtilizador);
	infoSend(socketfd, 5, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec3);

	pthread_mutex_lock(&trincoDuracaoRec3);
	sleep(tempoR3);
	pthread_mutex_unlock(&trincoDuracaoRec3);

	pthread_mutex_lock(&trincoUserRec3);
	numUsers3--;
	vagasRec3++;
	printf(MAG "\t\t\t\t\t\t\t\t\t\tO utilizador %2d vai sair do escorrega\n" RESET, user.idUtilizador);
	infoSend(socketfd, 5, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec3);

	sem_post(&semNumUsersRec3);
}

// Função principal que simula a entrada ou não do utilizador no recurso 3.
void rec3(struct utilizador user)
{
	if (vagasRec3 > 0 && numUsers3 < numMaxUtilizadoresRec3 && vagasFilaRec3 == numMaxFilaRecurso3) 
	{
		sem_wait(&semNumUsersRec3);
		entrarRec3(user);
		userSai(user, 0);
	}
	else if(vagasRec3 == 0 || numUsers3 >= numMaxUtilizadoresRec3 || vagasFilaRec3 <= numMaxFilaRecurso3)
	{
		if(vagasFilaRec3 != 0)
		{
			sem_wait(&semFilaRec3);
			vagasFilaRec3--;
			
			printf(YEL "\t\t\t\t\tEntrada do utilizador %2d na fila do escorrega\n" RESET, user.idUtilizador);
			
			sem_wait(&semEsperaFilaRec3);
			sem_wait(&semNumUsersRec3);

			sem_post(&semFilaRec3);
			vagasFilaRec3++;

			sem_post(&semEsperaFilaRec3);
			
			entrarRec3(user);

			userSai(user, 0);
		}
		else if(vagasFilaRec3 == 0)
		{
			printf(BLU "O escorrega e a sua fila estão cheios\n" RESET);
			userSai(user, 3);
		}
		else
		{
			printf(RED "Erro: As vagas da fila do escorrega estão a %2d\n" RESET, vagasFilaRec3);
		}
	}
	else
	{
		printf(RED "Erro: As vagas do escorrega está a %2d e a quantidade de users no escorrega está a %2d\n" RESET, vagasRec3, numUsers3);
	}
}

// Função para entrada no recurso 4, levando em consideração prioridade e condições do recurso 4.
void entrarRec4(struct utilizador user)
{
	pthread_mutex_lock(&trincoUserRec4);

	numUsers4++;
	vagasRec4--;
	printf(CYN "\t\t\t\t\tO utilizador %2d entrou no snack-bar\n" RESET, user.idUtilizador);
	infoSend(socketfd, 6, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec4);

	pthread_mutex_lock(&trincoDuracaoRec4);
	sleep(tempoR4);
	pthread_mutex_unlock(&trincoDuracaoRec4);

	pthread_mutex_lock(&trincoUserRec4);
	numUsers4--;
	vagasRec4++;
	printf(MAG "\t\t\t\t\t\t\t\t\t\tO utilizador %2d vai sair do snack-bar\n" RESET, user.idUtilizador);
	infoSend(socketfd, 6, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
	pthread_mutex_unlock(&trincoUserRec4);

	sem_post(&semNumUsersRec4);
}

// Função principal que simula a entrada ou não do utilizador no recurso 4.
void rec4(struct utilizador user)
{
	if (vagasRec4 > 0 && numUsers4 < numMaxUtilizadoresRec4 && vagasFilaRec4 == numMaxFilaRecurso4) 
	{ // O recurso tem vagas e o user entra
		sem_wait(&semNumUsersRec4);
		entrarRec4(user);

		userSai(user, 0);
	}
	else if(vagasRec4 == 0 || numUsers4 >= numMaxUtilizadoresRec4 || vagasFilaRec4 <= numMaxFilaRecurso4)
	{ // O recurso não tem vagas
		if(vagasFilaRec4 != 0) // A fila do recurso tem vagas
		{
			sem_wait(&semFilaRec4);
			vagasFilaRec4--;
			
			printf(YEL "\t\t\t\t\tEntrada do utilizador %2d na fila do snack-bar\n" RESET, user.idUtilizador);
			
			sem_wait(&semEsperaFilaRec4);
			sem_wait(&semNumUsersRec4);

			sem_post(&semFilaRec4);
			vagasFilaRec4++;

			sem_post(&semEsperaFilaRec4);
			
			entrarRec4(user);

			userSai(user, 0);
		}
		else if(vagasFilaRec4 == 0)
		{
			printf(BLU "O snack-bar e a sua fila estão cheios\n" RESET);
			userSai(user, 4);
		}
		else
		{
			printf(RED "Erro: As vagas da fila do snack-bar estão a %2d\n" RESET, vagasFilaRec4);
		}
	}
	else
	{
		printf(RED "Erro: As vagas do snack-bar está a %2d e a quantidade de users no snack-bar está a %2d\n" RESET, vagasRec4, numUsers4);
	}
}

// Simula a entrada na fila do parque aquático para utilizadores prioritários.
void filaPrio_Aquaparque(struct utilizador user)
{
	pthread_mutex_unlock(&orderP);
	printf(YEL "O utilizador %2d chegou à fila prioritária do Aquaparque\n" RESET, user.idUtilizador);
	sem_wait(&semFilaP_Aquaparque);
	numUsersP++;

	if(j == 0)
	{
		sem_wait(&semEsperaNP_Aquaparque);
		j++;
	}

	sem_wait(&semNumUsersAquaparque);
	sem_post(&semNumUsersAquaparque);
	sem_post(&semFilaP_Aquaparque);
	numUsersP--;
}

// Simula a entrada na fila do parque aquático para utilizadores não prioritários.
void filaNaoPrio_Aquaparque(struct utilizador user)
{
	pthread_mutex_unlock(&orderNP);
	printf(YEL "O utilizador %2d chegou à fila NÃO prioritária do Aquaparque\n" RESET, user.idUtilizador);
	sem_wait(&semFilaNP_Aquaparque);
	numUsersNP++;

	sem_wait(&semNumUsersAquaparque);
	printf(YEL "O utilizador NÃO prioritário %2d cede a entrada no Aquaparque aos prioritários\n" RESET, user.idUtilizador);
	sem_post(&semNumUsersAquaparque);
	sem_wait(&semEsperaNP_Aquaparque);

	sem_post(&semFilaNP_Aquaparque);
	numUsersNP--;
}

// Obtém o valor atual de um semáforo.
int getValue(sem_t *sem)
{
	int valor;
	sem_getvalue(sem, &valor);
	return valor;
}

// Função principal executada por cada thread de utilizador.
void * users(void * pointer)
{
	struct utilizador user = criaUser();

	pthread_mutex_lock(&order);

	if(vagasAquaparque == 0)
	{
		if(user.prioridade == 1)
		{
			if(numUsersP >= maximoFila)
			{
				printf(RED "O user %2d é prioritário, mas a fila prioritária do Aquaparque está cheia, logo ele desistiu\n" RESET, user.idUtilizador);
				
				pthread_mutex_unlock(&order);

				pthread_exit(NULL);
			}
			else if(numUsersP < maximoFila && numUsersP >= 0)
			{
				pthread_mutex_unlock(&order);
				pthread_mutex_lock(&orderP);
				filaPrio_Aquaparque(user);
				pthread_mutex_lock(&order);
			}
			else
			{
				printf(RED "Erro: A fila prioritária do Aquaparque está com %2d vagas\n" RESET, maximoFila - numUsersP);
			}
		}
		else if(user.prioridade == 0)
		{
			if(numUsersNP >= maximoFila)
			{
				printf(RED "O user %2d é NÃO prioritário, mas a fila NÃO prioritária do Aquaparque está cheia, logo ele desistiu\n" RESET, user.idUtilizador);

				pthread_mutex_unlock(&order);

				pthread_exit(NULL);
			}
			else if (numUsersNP < maximoFila && numUsersNP >= 0)
			{
				pthread_mutex_unlock(&order);
				pthread_mutex_lock(&orderNP);
				filaNaoPrio_Aquaparque(user);
				pthread_mutex_lock(&order);
			}
			else
			{
				printf(RED "Erro: A fila NÃO prioritária do Aquaparque está com %2d vagas\n" RESET, maximoFila - numUsersNP);
			}
		}
		else
		{
			printf(RED "Erro: O user %2d tem a prioridade a %d\n" RESET, user.idUtilizador, user.prioridade);
		}
	}

	pthread_mutex_lock(&trincoUser);

	if (!user.entrou) 
	{
		pthread_mutex_lock(&trincoNumUsersEntrada);
		sem_wait(&semNumUsersAquaparque);
		vagasAquaparque--;
		numUsers++;
		int semValue = getValue(&semNumUsersAquaparque);
		printf(GRN "O utilizador %2d entrou no Aquaparque\n" RESET, user.idUtilizador);
		infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
		user.entrou = true;
		pthread_mutex_unlock(&order);
		pthread_mutex_unlock(&trincoNumUsersEntrada);
	}

	pthread_mutex_unlock(&trincoUser);
	if(numUsersP == 0 && j == 1)
	{
		sem_post(&semEsperaNP_Aquaparque);
	}
	usleep(50000);  // Evita espera ativa
    
	mudaOuSai(user, user.numRecursoEncontra);
}

// Inicia a simulação, cria threads para os recursos e utilizadores, e controla o fluxo.
void sim(int socketfd)
{
    valueInit();
    infoSend(socketfd, 1, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);

	int zero = 0;
    int um = 1;
    int dois = 2;
	int tres = 3;
    
	printf(WHT "\nAquaparque:\n" RESET);
    pthread_create(&tIdRecursos[0], NULL, recursos, &zero);
	pthread_create(&tIdRecursos[1], NULL, recursos, &um);
	pthread_create(&tIdRecursos[2], NULL, recursos, &dois);
	pthread_create(&tIdRecursos[3], NULL, recursos, &tres);

    for(int i = 0; i < quantidadeRecursos; i++)
	{
		pthread_join(tIdRecursos[i], NULL);
    }

    for(int i = 0; i < numMaxUtilizadoresCriar; i++)
    {
    	pthread_create(&tIdUtilizadores[i], NULL, users, NULL);
    }
	
	for(int i = 0; i < numMaxUtilizadoresCriar; i++)
	{
		pthread_join(tIdUtilizadores[i], NULL);
	}
	
	infoSend(socketfd, 20, numUsers, numUsers1, numUsers2, numUsers3, numUsers4);
}

// Função principal para inicializar as threads e recursos.
int main(int argc, char*argv[])
{
	srand(time(NULL));
    socketfd = criaSocket();

    sim(socketfd);

	close(socketfd);
	printf(WHT "Simulação terminada\n" RESET);
		
	free(meuAquaparque.recursos);
		
	exit(0);
}
