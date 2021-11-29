#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//criar arquivo controle.bin para armazenar
//quandos foram inseridos e removidos
//int#int
/*
typedef struct {
    int qtdInseridos;
    int qtdRemovidos;
}Controle;
*/
/* TAREFAS
- Fazer função remove (Definir estratégia da função)
- Implementar um header no arquivo mainfile
- Conferir o header antes de inserir
- Consumir o arquivo controle ao carregas arquivos e Alterá-lo antes de fechar o programa
- Fazer função de compactação.
*/

typedef struct {
	int qtdInserido;
	int qtdRemovido;
} Controle;

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

void carrega_arquivos(ClienteFilme *vet_cliF, RemoveReg *vet_rem, Controle *controle){
	FILE *fd;
	fd = fopen("controle.bin", "ab+");
	if(fread(controle, sizeof(Controle), 1, fd)){ 
		printf("\nArquivo Controle.bin carregado");
	}else{//arquivo ainda vazio
		controle->qtdInserido = 0;
		controle->qtdRemovido = 0;
	}
	fclose(fd);

	fd = fopen("insere.bin", "ab+");
	//int k = controle->qtdInserido;
	int k =0;
	while(fread(&vet_cliF[k], sizeof(ClienteFilme), 1, fd)){
		k++;
	}
	printf("\nArquivo Insere.bin carregado");
	fclose(fd);

	fd = fopen("remove.bin", "ab+");
	k = 0;
	printf("%d", k);
	while(fread(&vet_rem[k], sizeof(RemoveReg), 1, fd)){
		k++;		
	}
	printf("\nArquivo remove.bin carregado");
	fclose(fd);

}

void inserir(ClienteFilme *vet_cliF, FILE *file, Controle *controle){
	char registro[160];
	sprintf(registro, "#%d#%d#%s#%s#%s", 
	vet_cliF[controle->qtdInserido].CodCli, 
	vet_cliF[controle->qtdInserido].CodF, 
	vet_cliF[controle->qtdInserido].NomeCli, 
	vet_cliF[controle->qtdInserido].NomeF, 
	vet_cliF[controle->qtdInserido].Genero);
	int tam_reg = strlen(registro);
	fwrite(&tam_reg, sizeof(int), 1, file);
	fwrite(registro, sizeof(char), tam_reg, file);
	controle->qtdInserido++;
	
}

void remover(RemoveReg *vet_rem, FILE *file){
	//percorrer arquivo
	//se chave primaria igual à chave primaria requerida
	//então .... colocar marcador 
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
	Controle *controle = (Controle*) malloc(sizeof(Controle));;

	while(opcao != 5){
		printf("\n1. Insercao");
		printf("\n2. Remocao");
		printf("\n3. Compactacao");
		printf("\n4. Carrega Arquivos");
		printf("\n5. Sair");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				inserir(vet_cliF, file, controle);
				break;
			case 2:
				remover(vet_rem, file);
				break;
			case 3:
				break;
			case 4:
				carrega_arquivos(vet_cliF, vet_rem, controle);
				break;
			case 5:
				break;
		}
	}
	fclose(file);

	file = fopen("controle.bin", "wb+");
	fwrite(controle, sizeof(controle), 1, file);
	fclose(file);
	return 0;
}


