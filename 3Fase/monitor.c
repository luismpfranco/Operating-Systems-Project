#include "util.h"

bool simEnd = false;

//Recebe mensagens do cliente através do socket, decodifica e escreve informações formatadas no ficheiro de output e na consola
void decodMessage (int newsocketfd)
{
	int messageArrived = 0;
	int state = 0;
	int numUtilizadores = 0;
	int tmp = 0;
	int qtdUtilizadores1 = 0;
	int qtdUtilizadores2 = 0;
	int qtdUtilizadores3 = 0;
	
	FILE* ficheiroSaida = fopen("saida.txt", "a");
	
	if(ficheiroSaida == NULL)
	{
		printf("Ocorreu um erro ao criar um ficheiro.");
	}
	else
	{
		while(state != 20)
		{
			char lineReceive[LINHAMAX + 1];
			messageArrived = recv(newsocketfd, lineReceive, LINHAMAX, 0);
			
			sscanf(lineReceive, "%d %d %d %d %d", &state, &numUtilizadores, &qtdUtilizadores1, &qtdUtilizadores2, &qtdUtilizadores3);
			
			switch(state)
			{
				case 1:
				{
					fprintf(ficheiroSaida, "Início de simulação\n");
					printf("Início de simulação\n");
					break;
				}
				case 2:
				{
					fprintf(ficheiroSaida, "Quantidade de utilizadores: %d\n", numUtilizadores);
					printf("Quantidade de utilizadores: %d\n", numUtilizadores);
					break;
				}
				case 3:
				{
					fprintf(ficheiroSaida, "Recurso 1: %d utilizador(es)\n", qtdUtilizadores1);
					printf("Recurso 1: %d utilizador(es)\n", qtdUtilizadores1);
					break;
				}
				case 4:
				{
					fprintf(ficheiroSaida, "Recurso 2: %d utilizador(es)\n", qtdUtilizadores2);
					printf("Recurso 2: %d utilizador(es)\n", qtdUtilizadores2);
					break;
				}
				case 5:
				{
					fprintf(ficheiroSaida, "Recurso 3: %d utilizador(es)\n", qtdUtilizadores3);
					printf("Recurso 3: %d utilizador(es)\n", qtdUtilizadores3);
					break;
				}
			}
		}
		if(state == 20)
		{
			fprintf(ficheiroSaida, "Término da simulação.\n");
			printf("Término da simulação.\n");
			fclose(ficheiroSaida);
		}
	}
}

//Inicia o servidor, aguardando conexões do cliente e criando uma thread para decodificar mensagens do cliente
void simInit()
{

	int socketfd, newsocketfd, lenClient, childpid, lenServer;
	struct sockaddr_un addrClient, addrServer;

	if ((socketfd = socket(AF_UNIX,SOCK_STREAM, 0)) < 0)
	{
		perror ("Erro ao abrir o socket do server.");
	}

	bzero((char*)&addrServer, sizeof(addrServer));
	addrServer.sun_family = AF_UNIX;
	strcpy(addrServer.sun_path, UNIXSTR_PATH);
	lenServer = strlen(addrServer.sun_path) + sizeof(addrServer.sun_family);
	unlink(UNIXSTR_PATH);

	if(bind(socketfd, (struct sockaddr *) &addrServer, lenServer) < 0)
	{
		perror("Não é possível a conexão do socket ao endereço específico.");
	}

	printf("Esperando pelo simulador\n");
	listen (socketfd, 1);

	lenClient = sizeof(addrClient);
	newsocketfd = accept (socketfd, (struct sockaddr *) &addrClient, &lenClient);
	if(newsocketfd < 0)
	{
		perror ("Erro ao criar o socket.");
	}

	if((childpid = fork()) < 0)
	{
		perror("Erro ao criar o processo filho.");
	}
	else if(childpid == 0)
	{
		close(socketfd);
		decodMessage(newsocketfd);
		exit(0);
	}
	close(newsocketfd);
}

//Apresenta um menu para iniciar a simulação e chama simInit para iniciar o servidor
int main(int argc, char const * argv[])
{
	printf ("Selecione a seguinte opção:\n");
	printf ("1. Começar a simulação.\n");

	int opt = 0;
	while(!simEnd)
	{
		if(opt == 1)
		{
			simInit();
		}
		while (opt != 1)
		{
			printf("Seleciona a opção anterior!\n");
			scanf("%d", &opt);
		}
	}
	return 0;
}
