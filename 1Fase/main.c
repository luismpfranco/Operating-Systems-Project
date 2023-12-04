#include "monitor.h"
#include "simulador.h"
	
int main (int argc, char *argv[]) {
	
	printf("\n");

	lerFileConfig("configbase.config");
	
	printf("\n------------------------------\n");
	printf("\n");
	printf("Utilizadores que já entraram: %d \n", numUtilizadores);
	printf("Tempo médio de chegada: %d%s \n", numTempMedio, "s");
	printf("Dimensão de recursos: %d \n", numRecursos);
	printf("Probabilidade de desistência nas filas: %d%s \n", numProbDes, "%");
	printf("Início da simulação: %d%s \n", numInicio, "s");
	printf("Tempo da Simulação: %d%s \n", numTempSim, "s");
	printf("\n");
	
	writeLog("registo.txt");
}
