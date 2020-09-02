#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAIOR_DIGITACAO 256 // Tamanho máximo da string de comando, em caracteres
#define MAIOR_COMANDO 32 // Tamanho máximo de parâmetros, opções, chaves, controles, etc. por comando
#define MAIOR_PIPELINE 16 // Tamanho máximo de comandos encadeados em um pipeline
#define MAIOR_PALAVRA 64
#define SUCESSO 0  // mesma convenção de exit: 0 é sucesso e !=0 é falha
#define FALHA 1

char* comando[MAIOR_PIPELINE][MAIOR_COMANDO];
int quantos_comandos;

int recebe_digitacao(char *entr)
{
	fflush(stdin);
	if(fgets(entr, MAIOR_DIGITACAO, stdin) == NULL)
	{
		if(feof(stdin))
		{
			printf("fim\n");
			exit(SUCESSO);
		}
		fflush(stdin);
		return FALHA;
	}
	fflush(stdin);

	if(entr[0] == '\n') return FALHA;
	return SUCESSO;
}

void analise(char* entr) {
    char analisando[MAIOR_PIPELINE][MAIOR_DIGITACAO];

  /* A construção da árvore de comandos começa com o seccionamento dos trechos entre as barras verticais (pipes) */
    char* trch = strtok(entr, "|\n"); // Busca por barras verticais (pipes) ou o fim da string
    int i = 0; // Navegação no array
    do {
        strcpy(analisando[i], trch); // Armazaena o trecho(token) na posição específica do array
        i++;
    } while ((trch = strtok(NULL, "|\n")) != NULL); // strtok(NULL <~~~> continua busca do strtok anterior
    quantos_comandos = i;

  /* Agora, cada trecho é seccionado levando em conta espaços em branco. */
    int j; // Navegação no array
    for (i = 0; i < quantos_comandos; i++) {
        trch = strtok(analisando[i], " ");
        j = 0;
        do {
            if (comando[i][j] == NULL)
                comando[i][j] = (char*) malloc( MAIOR_PALAVRA * sizeof(char) );
            strcpy(comando[i][j], trch); // Armazaena o token na posição específica do array
            j++;
        } while ( (trch = strtok(NULL, " ")) != NULL );
        if (comando[i][j] != NULL) {
            free(comando[i][j]);
            comando[i][j] = NULL;
        }
    }
}

int
main() {
    char entrada[MAIOR_DIGITACAO];
    int recebe_status;
    int i, j;
    int pid;
    int status;
    int fd[2];

    for (i=0; i < MAIOR_PIPELINE; i++)
        for (j=0; j < MAIOR_COMANDO; j++)
            comando[i][j] = NULL;

    while(1) {
        printf("SHZN> ");
        recebe_status = recebe_digitacao(entrada);
        if (recebe_status == FALHA){
            exit(FALHA);
        }

        analise(entrada);

        if (quantos_comandos != 2) {
            puts("Esta versão só funciona com 1 pipe, nem mais, nem menos.");
            continue;
        }

        pipe(fd);
        pid = fork();
        if (pid == 0) {
            dup2(fd[1],1);
            close(fd[0]);
            close(fd[1]);
            execvp(comando[0][0], comando[0]);
        } else {
            pid = fork();
            if (pid == 0) {
                dup2(fd[0], 0);
                close(fd[0]);
                close(fd[1]);
                execvp(comando[1][0], comando[1]);
            } else {
                close(fd[0]);
                close(fd[1]);
                waitpid(-1, &status, 0);
                waitpid(-1, &status, 0);
            }
        }
    }
}
