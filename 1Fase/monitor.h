//libs
#include <stdio.h> // Para funções de entrada/saída padrão como printf
#include <stdlib.h> // Para funções de manipulação de strings como atoi
#include <fcntl.h> // Para as constantes O_CREAT, O_TRUNC, O_RDWR
#include <unistd.h> // Para a função dup2
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


//funções
void writeLog(char* nomeFile);
