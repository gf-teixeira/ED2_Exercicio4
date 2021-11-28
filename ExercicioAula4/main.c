#include <stdio.h>
#include <string.h>

typedef struct {
	int CodCli;
	int CodF;
	char NomeCli[50];
	char NomeF[50];
	char Genero[50];
	
} ClienteFilme;

typedef struct {
	int CodCli;
	int CodF;
} RemoveReg;

void carrega_arquivos(ClienteFilme *vet_cliF, RemoveReg *vet_rem){
	FILE *fd;
	fd = fopen("insere.bin", "ab+");
	int k = 0;
	while(fread(&vet_cliF[k], sizeof(ClienteFilme), 1, fd)){
		k++;		
	}
	fclose(fd);

	fd = fopen("remove.bin", "ab+");
	k = 0;
	while(fread(&vet_rem[k], sizeof(RemoveReg), 1, fd)){
		k++;		
	}
	fclose(fd);
}

int main(){
	int opcao = 0;
	FILE *file;

	if((file = fopen("MAIN_FILE.dad", "ab+") == NULL)){
		printf("Nao foi possivel abrir o arquivo");
		return 0;
	}

	ClienteFilme vet_cliF[4];
	RemoveReg vet_rem[3];
	while(opcao != 5){
		printf("\n1. Insercao");
		printf("\n2. Remocao");
		printf("\n3. Compactacao");
		printf("\n4. Carrega Arquivos");
		printf("\n5. Sair");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				carrega_arquivos(vet_cliF, vet_rem);
				/*for(int i=0; i<4; i++){
					printf("\n");
					printf(" %d", vet_cliF[i].CodCli);
					printf(" %d", vet_cliF[i].CodF);
					printf(" %s", vet_cliF[i].NomeCli);
					printf(" %s", vet_cliF[i].NomeF);
				}
				for(int i=0; i<3; i++){
					printf("\n");
					printf(" %d", vet_rem[i].CodCli);
					printf(" %d", vet_rem[i].CodF);
				
				}*/
				break;
		}
	}
	return 0;
}


