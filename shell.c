#include <stdio.h>
#include <stdlib.h>

#define TAM_CMD 256
#define SUCESSO 0  // mesma convenção de exit: 0 é sucesso e !=0 é falha
#define FALHA 1

int recebe_digitacao(char *entr)
{
	fflush(stdin);
	if(fgets(entr, TAM_CMD, stdin) == NULL)
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



int
main() {
    char entrada[TAM_CMD];
    int recebe_status;

    while(1) {
        printf("shellzin> ");
        recebe_status = recebe_digitacao(entrada);
        if (recebe_status == SUCESSO)
            printf("O texto digitado foi %s", entrada);
    }
}
