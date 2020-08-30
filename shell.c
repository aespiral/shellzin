#include <stdio.h>
#include <stdlib.h>

#define TAM_CMD 256

int recebe_digitacao(char *entr)
{
	fflush(stdin);
	if(fgets(entr, TAM_CMD, stdin) == NULL)
	{
		if(feof(stdin))
		{
			printf("fim\n");
			exit(0);
		}
		fflush(stdin);
		return 0;
	}
	fflush(stdin);

	if(entr[0] == '\n') return 0;
	return 1;
}

int
main() {
    char entrada[TAM_CMD];
    int recebe_status;

    while(1) {
        printf("shellzin> ");
        recebe_status = recebe_digitacao(entrada);
        if (recebe_status == 1)
            printf("O texto digitado foi %s", entrada);
    }
}
