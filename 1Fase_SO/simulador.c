#include "simulador.h"

//para ler o file de configbase
void lerFileConfig(char* nomeFile)
{
	FILE* file;
	if ((file = fopen(nomeFile, "r")) == NULL) //tenta abrir o file em modo de leitura
	{
		printf("O ficheiro não existe. Tente de novo.");
	}
	else //extrai as configuracoes
	{
		char line [120]; //para armazenar cada linha lida
		int vtemporaria; //para armazenar os valores convertidos de strings para inteiros
		char* configuracao; //apontador para strings usado para armazenar as palavras de cada linha

		while(fgets(line, sizeof line, file) != NULL) //ler cada linha e continua até não haver mais
		{
			configuracao = strtok(line, " "); //divide a line em substrings usando " " ou seja vai encontrar cada palavra e guardar em substrings tirando o " " sequencialmente

			if(strcmp(configuracao, "Utilizadores") == 0) //compara ambas, se for 0 sao iguais
			{
				configuracao = strtok(NULL, "-"); //procura a proxima palavra e guarda em configuracao
				vtemporaria = atoi(configuracao); //converte a string em int e guarda em vtemporaria
				printf("%s%d%s\n", S_UTILIZADORES, vtemporaria, " "); //imprime na tela os valores...
				numUtilizadores = vtemporaria;
			}
			else if(strcmp(configuracao, "TempMedio") == 0)
			{
				configuracao = strtok(NULL, "-");
				vtemporaria = atoi(configuracao);
				printf("%s%d%s\n", S_TEMPMEDIO, vtemporaria, "s");
				numTempMedio = vtemporaria;
			}
			else if(strcmp(configuracao, "Recursos") == 0)
			{
				configuracao = strtok(NULL, "-");
				vtemporaria = atoi(configuracao);
				printf("%s%d%s\n", S_RECURSOS, vtemporaria, " ");
				numRecursos = vtemporaria;
			}
			else if(strcmp(configuracao, "ProbDes") == 0)
			{
				configuracao = strtok(NULL, "-");
				vtemporaria = atoi(configuracao);
				printf("%s%d%s\n", S_PROBDES, vtemporaria, "%");
				numProbDes = vtemporaria;
			}
			else if(strcmp(configuracao, "Inicio") == 0)
			{
				configuracao = strtok(NULL, "-");
				vtemporaria = atoi(configuracao);
				printf("%s%d%s\n", S_INICIO, vtemporaria, "s");
				numInicio = vtemporaria;
			}
			else if(strcmp(configuracao, "TempSim") == 0)
			{
				configuracao = strtok(NULL, "-");
				vtemporaria = atoi(configuracao);
				printf("%s%d%s\n", S_TEMPSIM, vtemporaria, "s");
				numTempSim = vtemporaria;
			}
		}
		fclose(file);
	}
}
