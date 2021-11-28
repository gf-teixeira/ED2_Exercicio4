#include <stdio.h>
#include <string.h>

//TODO fazer controle de inserção diretamente no arquivo
//sem o uso de variável global
int global=0;

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

void inserir(ClienteFilme *vet_cliF, FILE *file){
	char registro[160];
	sprintf(registro, "#%d#%d#%s#%s#%s", 
	vet_cliF[global].CodCli, 
	vet_cliF[global].CodF, 
	vet_cliF[global].NomeCli, 
	vet_cliF[global].NomeF, 
	vet_cliF[global].Genero);
	int tam_reg = strlen(registro);
	//printf("%s", registro);
	fwrite(&tam_reg, sizeof(int), 1, file);
	fwrite(registro, sizeof(char), tam_reg, file);
	global++;
}
int main(){
	int opcao = 0;
	FILE *file;

	if((file = fopen("mainfile.bin", "ab+")) == NULL){
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
				inserir(vet_cliF, file);
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				carrega_arquivos(vet_cliF, vet_rem);
				break;
		}
	}
	fclose(file);
	return 0;
}


