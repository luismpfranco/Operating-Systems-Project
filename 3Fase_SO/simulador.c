#include "util.h"

//socket
int socketfd = 0;

//Inicialização da struct global
struct aquaparque meuAquaparque;

//Semáforos
sem_t infoSendT;
sem_t semFila1;
sem_t semFila2;
sem_t semFila3;
sem_t semNumUsersRec1;
sem_t semNumUsersRec2;
sem_t semNumUsersRec3;

//Trincos
pthread_mutex_t trincoUser;
pthread_mutex_t trincoNumUsers;	
pthread_mutex_t trincoUserEntrouRec1;
pthread_mutex_t trincoUserEntrouRec2;
pthread_mutex_t trincoUserEntrouRec3;

//Tarefas
pthread_t tIdRecursos[3];
pthread_t tIdUtilizadores[300];

//Variáveis do ficheiro
int numMaxUtilizadoresCriar = 0;
int probDesistencia = 0;
int tmpMedioChegadaUsers = 0;
int quantidadeRecursos = 0;
int maximoFila = 0;
int comecoSimulacao = 0;
int fimSimulacao = 0;
int vagasRec1 = 0;
int vagasFilaRec1 = 0;
int vagasRec2 = 0;
int vagasFilaRec2 = 0;
int vagasRec3 = 0;
int vagasFilaRec3 = 0;

//Variáveis globais
int idUser = 0;

int tempoFilaR1 = 0;
int tempoFilaR2 = 0;
int tempoFilaR3 = 0;
int tempoR1 = 0;
int tempoR2 = 0;
int tempoR3 = 0;

int desistencia = 0;
int desistirFila = 0;
int desisteR1 = 0;
int desisteR2 = 0;
int desisteR3 = 0;

//Variáveis que serão enviadas para o monitor
int numUsers = 0;
int numUsers1 = 0;
int numUsers2 = 0;
int numUsers3 = 0;


//Lê as configurações do ficheiro "configbase.txt" e inicializa as variáveis globais.
void readFile()
{
	FILE* configuracao;

	configuracao = fopen("configbase.txt", "r");

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
				numMaxUtilizadoresCriar = value;
			}
			if(strcmp(param, "probDesistir") == 0)
			{
				probDesistencia = value;
			}
			if(strcmp(param, "tmpMedioChegadaUtilizadores") == 0)
			{
				tmpMedioChegadaUsers = value;
			}
			if(strcmp(param, "numRecursos") == 0)
			{
				quantidadeRecursos = value;
			}
			if(strcmp(param, "maxFila") == 0)
			{
				maximoFila = value;
			}
			if(strcmp(param, "inicioSimulacao") == 0)
			{
				comecoSimulacao = value;
			}
			if(strcmp(param, "tempoSimulacao") == 0)
			{
				fimSimulacao = value;
			}
		}
		fclose(configuracao);
	}
	else {
		printf("Ocorreu um erro ao abrir o ficheiro. Try Again.\n");
	}
}

//Inicializa a estrutura do aquaparque e os seus recursos com base na quantidade fornecida
void criarRecursos(int qtdRecursosCriar)
{
	meuAquaparque.qtdRecursos = qtdRecursosCriar;
	meuAquaparque.recursos = malloc(meuAquaparque.qtdRecursos * sizeof(struct recurso));
	
	for(int i = 0; i < qtdRecursosCriar; i++)
	{
		meuAquaparque.recursos[i].numRecurso = i + 1;
		meuAquaparque.recursos[i].numUtilizadoresDentro = 0;
		meuAquaparque.recursos[i].recursoOcupado = false;
		meuAquaparque.recursos[i].numUtilizadoresFila = 0;
		meuAquaparque.recursos[i].numMaxUtilizadoresDoRecurso = 10;
	}
}

//Cria um novo utilizador com id, prioridade, número de recurso, estado de se já entrou no recurso ou não e desistência
struct utilizador criaUser()
{
	pthread_mutex_lock(&trincoUser);
	
	struct utilizador user;
	user.idUtilizador = idUser;
	
	int valorPrioridade[2] = {0, 1};
	
	int valorPrioRandom = valorPrioridade[rand() % 2];
	user.prioridade = valorPrioRandom;
	
	int valRecurso[3] = {1, 2, 3};
	int valRecRandom = valRecurso[rand() % 3];
	user.numRecursoEncontra = valRecRandom;
	user.entrou = false;
	user.desistencia = 0;
	
	idUser++;
	
	pthread_mutex_unlock(&trincoUser);
	return user;
}

//Imprime informações sobre os recursos na consola
void * recursos(void * pointer)
{
	int * numeroRecurso = (int *) pointer;
	struct recurso *recursosNovos = &(meuAquaparque.recursos[*numeroRecurso]);
	
	printf("Recurso %d:\n", recursosNovos->numRecurso);
	printf("   Com %d pessoas dentro.\n", recursosNovos->numUtilizadoresDentro);
	printf("   E com %d pessoas na fila.\n", recursosNovos->numUtilizadoresFila);
	sleep(2);
}

//Retorna um valor de tempo aleatório para simular o tempo de espera na fila do recurso
int tempoFilaRecurso()
{
	return rand() % 11 + 5;
}

//Retorna um valor de tempo aleatório para simular o tempo de uso do recurso
int tempoRecurso()
{
	return rand() % 16 + 5;
}

//Simula a probabilidade de um utilizador desistir da fila do recurso - APAGAR
int probDesistirRec()
{
    int random = rand() % 99;

    int max_value = desistencia;

    if(random < max_value)
	{
        desistirFila = 1;
    }
    else
	{
        desistirFila = 0;
    }
    return desistirFila;
}

//Envia informações formatadas sobre o estado da simulação, o número de utilizadores totais, dos recursos 1, 2 e 3 para o servidor através de sockets
void infoSend(int socketfd, int state, int numUtilizadores, int qtdUtilizadores1, int qtdUtilizadores2, int qtdUtilizadores3)
{
	sem_wait(&infoSendT);

    char buffer[LINHAMAX];
	int lenMsg = 0;
	
	sprintf(buffer, "%d %d %d %d %d", state, numUtilizadores, qtdUtilizadores1, qtdUtilizadores2, qtdUtilizadores3);
	lenMsg = strlen(buffer) + 1;

	if(send(socketfd, buffer, lenMsg, 0) != lenMsg)
	{
		perror("Ocorreu um erro ao enviar os dados\n");
	}

	sleep(1);
	sem_post(&infoSendT);
}

//Cria um socket Unix e coneta ao servidor, retornando o descritor do socket
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
			printf("Esperando pelo monitor\n");
			simWorking = true;
		}
	}

	if(socketfd < 0)
	{
		perror ("Erro aceite");
	}

	printf ("Simulação terminada \n");
	return socketfd;
}

//Inicializa valores dos semáforos e faz a leitura das configurações base
void valueInit()
{
	srand((unsigned)time(NULL));
	//RESOLVER
    sem_init(&infoSendT, 0, 1);
	sem_init(&semFila1, 0, 10);
	sem_init(&semFila2, 0, 10);
	sem_init(&semFila3, 0, 10);
	sem_init(&semNumUsersRec1, 0, 10);
	sem_init(&semNumUsersRec2, 0, 10);
	sem_init(&semNumUsersRec3, 0, 10);
    
	criarRecursos(3);
	
	readFile();
    printf("Leitura efetuada.\n");
	
	tempoFilaR1 = tempoFilaRecurso();
	printf("O tempo de espera na fila 1 é %d\n", tempoFilaR1);
	tempoFilaR2 = tempoFilaRecurso();
	printf("O tempo de espera na fila 2 é %d\n", tempoFilaR2);
	tempoFilaR3 = tempoFilaRecurso();
	printf("O tempo de espera na fila 3 é %d\n", tempoFilaR3);
	
	tempoR1 = tempoRecurso();
	printf("O tempo de duração do 1 é %d\n", tempoR1);
	tempoR2 = tempoRecurso();
	printf("O tempo de duração do 2 é %d\n", tempoR2);
	tempoR3 = tempoRecurso();
	printf("O tempo de duração do 3 é %d\n", tempoR3);
}

//Simula o comportamento dos utilizadores no aquaparque
void * users(void * pointer)
{
	struct utilizador user = criaUser();
	
	if(user.numRecursoEncontra == 1)
	{
		sem_wait(&semFila1);

		if(vagasFilaRec1 > 0)
		{
			printf("Entrada do user %d na fila do R%d\n", user.idUtilizador, user.numRecursoEncontra);
			vagasFilaRec1--;
		
			pthread_mutex_lock(&trincoNumUsers);
			numUsers++;
			infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
			pthread_mutex_unlock(&trincoNumUsers);
		
			user.tmpEntradaFilaRecursos = time(NULL);
			
			if(vagasRec1 == 0){
				printf("O user %d não conseguiu entrar no R%d pois já está cheio e saiu\n", user.idUtilizador, user.numRecursoEncontra);
				
				pthread_mutex_lock(&trincoNumUsers);
				numUsers++;
				vagasFilaRec1++;
				infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
				pthread_mutex_unlock(&trincoNumUsers);
				pthread_exit(NULL);
			}
			else{
				pthread_mutex_lock(&trincoUserEntrouRec1);
				sem_wait(&semNumUsersRec1); //decrementa
		
				vagasRec1--;

				user.entrou = true;
				numUsers1++;
		
				int tmpEsperaFilaRec1 = 0;
				tmpEsperaFilaRec1 = (int)difftime(time(NULL), user.tmpEntradaFilaRecursos);
				printf("tmpEsperaFilaRec1: %d\n", tmpEsperaFilaRec1);
		
				printf("Entrada do user %d no R%d. Esteve na fila por %d\n", user.idUtilizador, user.numRecursoEncontra, tmpEsperaFilaRec1);
				vagasFilaRec1++;
				infoSend(socketfd, 3, numUsers, numUsers1, numUsers2, numUsers3);

				pthread_mutex_unlock(&trincoUserEntrouRec1);
				
				sem_post(&semNumUsersRec1); //incrementa
				
				sleep(tempoR1);
				
				printf("Saída do user %d do R%d. Esteve lá por: %d\n", user.idUtilizador, user.numRecursoEncontra, tempoR1);
				numUsers1--;
				infoSend(socketfd, 3, numUsers, numUsers1, numUsers2, numUsers3);
				pthread_exit(NULL);
			}
		}
		else if(vagasFilaRec1 == 0){
			
			sleep(5);
			
			printf("Desistência do user %d da fila do R%d e saiu\n", user.idUtilizador, user.numRecursoEncontra);
			sem_post(&semFila1);
			
			pthread_mutex_lock(&trincoNumUsers);
			numUsers--;
			infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
			infoSend(socketfd, 3, numUsers, numUsers1, numUsers2, numUsers3);
			pthread_mutex_unlock(&trincoNumUsers);
			
			pthread_exit(NULL);
			
		}
	}
	else if(user.numRecursoEncontra == 2)
	{
		sem_wait(&semFila2);
		vagasFilaRec2--;
		
		printf("Entrada do user %d na fila do R%d\n", user.idUtilizador, user.numRecursoEncontra);
		
		pthread_mutex_lock(&trincoNumUsers);
		numUsers++;
		infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
		pthread_mutex_unlock(&trincoNumUsers);
		
		user.tmpEntradaFilaRecursos = time(NULL);
		
		if(vagasFilaRec2 == 0)
		{
			desisteR2 = probDesistirRec();
			
			if(desisteR2)
			{
				sleep(tempoFilaRecurso());
				
				printf("O utilizador %d desistiu da fila do recurso %d\n", user.idUtilizador, user.numRecursoEncontra);
				vagasFilaRec2++;
				sem_post(&semFila2);
				
				pthread_mutex_lock(&trincoNumUsers);
				numUsers--;
				infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
				infoSend(socketfd, 4, numUsers, numUsers1, numUsers2, numUsers3);
				pthread_mutex_unlock(&trincoNumUsers);
				
				pthread_exit(NULL);
			}
		}
		
		sleep(tempoFilaRecurso());
		
		pthread_mutex_lock(&trincoUserEntrouRec2);
		sem_wait(&semNumUsersRec2);
		
		vagasRec2--;

		user.entrou = true;
		numUsers2++;
		
		int tmpEsperaFilaRec2 = 0;
		tmpEsperaFilaRec2 = (int)difftime(time(NULL), user.tmpEntradaFilaRecursos);
		
		printf("Entrada do user %d no R%d. Esteve na fila por %d\n", user.idUtilizador, user.numRecursoEncontra, tmpEsperaFilaRec2);
		vagasFilaRec2++;
		infoSend(socketfd, 4, numUsers, numUsers1, numUsers2, numUsers3);
		
		pthread_mutex_unlock(&trincoUserEntrouRec2);
		
		int tmpRecurso = tempoRecurso();
		sleep(tmpRecurso);
		sem_post(&semNumUsersRec2);
		printf("O utilizador %d saiu do Recurso %d. Esteve lá por: %d\n", user.idUtilizador, user.numRecursoEncontra, tmpRecurso);
		numUsers2--;
		infoSend(socketfd, 4, numUsers, numUsers1, numUsers2, numUsers3);	
	}
	else if(user.numRecursoEncontra == 3)
	{
		sem_wait(&semFila3);
		
		
		printf("Entrada do user %d na fila do R%d\n", user.idUtilizador, user.numRecursoEncontra);
		
		pthread_mutex_lock(&trincoNumUsers);
		numUsers++;
		infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
		pthread_mutex_unlock(&trincoNumUsers);
		
		user.tmpEntradaFilaRecursos = time(NULL);
		
		if(vagasFilaRec3 == 0)
		{
			desisteR3 = probDesistirRec();
			
			if(desisteR3)
			{
				sleep(tempoFilaRecurso());
				
				printf("O utilizador %d desistiu da fila do recurso %d\n", user.idUtilizador, user.numRecursoEncontra);
				vagasFilaRec3++;
				sem_post(&semFila3);
				
				pthread_mutex_lock(&trincoNumUsers);
				numUsers--;
				infoSend(socketfd, 2, numUsers, numUsers1, numUsers2, numUsers3);
				infoSend(socketfd, 5, numUsers, numUsers1, numUsers2, numUsers3);
				pthread_mutex_unlock(&trincoNumUsers);
				
				pthread_exit(NULL);
			}
		}
		
		sleep(tempoFilaRecurso());
		
		pthread_mutex_lock(&trincoUserEntrouRec3);
		sem_wait(&semNumUsersRec3);
		
		vagasRec3--;

		user.entrou = true;
		numUsers3++;

		
		int tmpEsperaFilaRec3 = 0;
		tmpEsperaFilaRec3 = (int)difftime(time(NULL), user.tmpEntradaFilaRecursos);
		
		printf("O utilizador %d entrou no Recurso %d. Esteve na fila por %d\n", user.idUtilizador, user.numRecursoEncontra, tmpEsperaFilaRec3);
		vagasFilaRec3++;
		infoSend(socketfd, 5, numUsers, numUsers1, numUsers2, numUsers3);
		
		pthread_mutex_unlock(&trincoUserEntrouRec3);
		
		int tmpRecurso = tempoRecurso();
		sleep(tmpRecurso);
		sem_post(&semNumUsersRec3);
		printf("O utilizador %d saiu do Recurso %d. Esteve lá por: %d\n", user.idUtilizador, user.numRecursoEncontra, tmpRecurso);
		numUsers3--;
		infoSend(socketfd, 5, numUsers, numUsers1, numUsers2, numUsers3);
	}
	else
	{
		printf("Erro: o recurso do utilizador %d está a %d", user.idUtilizador, user.numRecursoEncontra);
	}
}

//Inicia a simulação, cria threads para os recursos e utilizadores, e controla o fluxo.
void sim(int socketfd)
{
    valueInit();
    infoSend(socketfd, 1, numUsers, numUsers1, numUsers2, numUsers3);

	int zero = 0;
    int um = 1;
    int dois = 2;
    
	
	printf("Aquaparque:\n");
    pthread_create (&tIdRecursos[0], NULL, recursos, &zero);
	pthread_create (&tIdRecursos[1], NULL, recursos, &um);
	pthread_create (&tIdRecursos[2], NULL, recursos, &dois);

	sleep(5);

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
	
    infoSend(socketfd, 20, numUsers, numUsers1, numUsers2, numUsers3);
}

//Chama as funções de inicialização, inicia a simulação e limpa os recursos no fim
int main(int argc, char*argv[])
{
    socketfd = criaSocket();

    sim(socketfd);
	
    close(socketfd);
	
	free(meuAquaparque.recursos);
	
    return 0;

}

