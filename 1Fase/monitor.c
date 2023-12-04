#include "monitor.h"
#include "simulador.h"

void writeLog(char* nomeFile){

	int fileDescriptor;
	fileDescriptor = open(nomeFile, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU); //abre o nomeFile ou cria se nao existir com permissoes de ler, escrever e executar para o admin
	
	//duplica o fileDescriptor para a saída padrão '1', faz com que os printf's abaixo seja escrito no file em vez de impresso na consola
	dup2(fileDescriptor,1);

	printf("Utilizadores que já entraram: %d \n", numUtilizadores);
	printf("Tempo médio de chegada: %d%s \n", numTempMedio, "s");
	printf("Dimensão de recursos: %d \n", numRecursos);
	printf("Probabilidade de desistência nas filas: %d%s \n", numProbDes, "%");
	printf("Início da simulação: %d%s \n", numInicio, "s");
	printf("Tempo da Simulação: %d%s \n", numTempSim, "s");
	printf("\n");

	//restaura para o valor original, ou seja, qq saida do programa será impresso na consola	
	dup2(1, fileDescriptor);
	close(fileDescriptor);
}