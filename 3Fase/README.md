# **Projeto SO -> 3.ª Fase**

## 1. Para começar
As instruções seguintes irão fornecer uma forma de testar o projeto em qualquer máquina local.


## 2. Requisitos
Para executar este projeto, é necessário ter os packages GCC e Make instalados no seu sistema operativo.
###**Nota**: Este projeto deve ser executado num sistema Unix.


## 3. Executando o projeto

1. Abra duas janelas de terminais.
2. Em cada um dos terminais, vá até o diretório do projeto e digite make clean. (Clean para limpar qualquer build efetuada anteriormente, isto é, limpa os executáveis, "*.o" e o ficheiro de saída "saida.txt")
3. No mesmo terminal, digite 'make' para compilar o projeto e digite './monitor' para executar o monitor.
4. No outro terminal, vá até o mesmo diretório e digite './simulador' para executar o simulador.
5. No terminal do monitor deve clicar 1 e dar enter para dar início à simulação.
6. Neste momento já deve estar a visualizar a simulação do projeto.
7. No fim deve fazer "Control + C" na janela do monitor para terminar o programa.


## 4. Alterações
Se quiser efetuar alguma alteração no número de utilizadores a entrar no Aquaparque, em cada recurso ou em cada fila de recurso basta:
- Abrir o configbase.txt ('nano configbase.txt' ou abrir o ficheiro no seu sistema operativo).
- Efetuar as alterações.
- Guardar e fechar o ficheiro:
	- No terminal deve-se fazer "Control + X";
	- Depois "Y" para confirmar as mudanças, "N" caso contrário;
	- E por fim "Enter" para fechar o ficheiro.
- Não esquecer de efetuar os passos para compilação e execução do programa. (Ver no ponto 3.)

## 5. Autores

- André Filipe Pereira Vieira 2035617
- Luís Miguel Pestana Franco 2083121
- Maria Catarina Vieira Freitas 2081621