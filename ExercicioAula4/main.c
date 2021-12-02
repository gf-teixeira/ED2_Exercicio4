#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int pega_registro(FILE *p_out, char *p_reg);

/* TAREFAS
- Fazer função remove (Definir estratégia da função)
- Implementar um header no arquivo mainfile
- Conferir o header antes de inserir
- Fazer alguma forma de ponteiro de um espaço disponível para o outro
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
    int valoresControles;

	if((fd = fopen("controle.bin", "rb+")) == NULL){
		printf("Nao foi possivel encontrar o arquivo de controle =(\nVamos criar um!...\n");
		fd = fopen("controle.bin", "wb+");
	}
	fseek(fd, 0, SEEK_SET);
	if(fread(&valoresControles, sizeof(int), 1, fd)){
		fseek(fd, 0, SEEK_SET);
		printf("\nArquivo Controle.bin carregado");
		fread(&valoresControles, sizeof(int), 1, fd);
		controle->qtdInserido = valoresControles;
		fread(&valoresControles, sizeof(int), 1, fd);
		controle->qtdRemovido = valoresControles;
	}else{//arquivo Controle ainda vazio
		controle->qtdInserido = 0;
		controle->qtdRemovido = 0;
	}
	fclose(fd);
	fd = fopen("insere.bin", "rb+");
	int k =0;
	while(fread(&vet_cliF[k], sizeof(ClienteFilme), 1, fd)){
		k++;
	}
	printf("\nArquivo Insere.bin carregado");
	fclose(fd);

	fd = fopen("remove.bin", "rb+");
	k = 0;
	while(fread(&vet_rem[k], sizeof(RemoveReg), 1, fd)){
		k++;
	}
	printf("\nArquivo remove.bin carregado");
	fclose(fd);

}

void inserir(ClienteFilme *vet_cliF, FILE *file, Controle *controle){
	char registro[160];
	char validade = 'V';
	int header;

	//V : válido, I: inválido
	sprintf(registro, "V#%d#%d#%s#%s#%s",
	vet_cliF[controle->qtdInserido].CodCli,
	vet_cliF[controle->qtdInserido].CodF,
	vet_cliF[controle->qtdInserido].NomeCli,
	vet_cliF[controle->qtdInserido].NomeF,
	vet_cliF[controle->qtdInserido].Genero);

	int tam_reg = strlen(registro); 
	printf("%s",registro);

	fseek(file,0,SEEK_SET);
	if(fread(&header, sizeof(int), 1, file)){
		printf("\nLeu header\n");
		printf("%d", header);
		if(header != -1){
			// fazer a busca para inserir em lugar que estava invalido
        }else{
            fseek(file, 0, SEEK_END);
            fwrite(&tam_reg, sizeof(int), 1, file);
        }
	}else{
		header = -1;
		fwrite(&header, sizeof(int), 1, file);
		fseek(file, 0, SEEK_END);
        fwrite(&tam_reg, sizeof(int), 1, file);
       
	}
	fwrite(registro, sizeof(char), tam_reg, file);
    controle->qtdInserido++;
    printf("\nInserido com sucesso.");
}

void remover(RemoveReg *vet_rem, Controle *controle, FILE *file){
	int removeCodCliente, removeCodFilme;
	int tamanho;
	char registro[156];
	char *pch;
	char invalido = 'I';
	int codCliente, codFilme;
	int tam_reg;
	int offset;
	int header;
	
	printf("\ncontrole->qtdRemovido: %d", controle->qtdRemovido);

	removeCodCliente = vet_rem[controle->qtdRemovido].CodCli;
	removeCodFilme = vet_rem[controle->qtdRemovido].CodF;

	printf("\nremoveCodCliente: %d", removeCodCliente);
	printf("\nremoveCodFilme: %d\n", removeCodFilme);

	fseek(file, 4, SEEK_SET);
	offset = 4;
	tam_reg = pega_registro(file,registro);
	printf("depois do tam_reg");

  	while(tam_reg > 0){
		printf("\nTESTANDO");
        pch = strtok(registro, "#");
		printf("%c", *pch);
		if(*pch == 'V'){
			pch = strtok(NULL, "#");
        	codCliente = (*pch) - '0';
        	pch = strtok(NULL, "#");
        	codFilme = (*pch) - '0';
        	if((codCliente == removeCodCliente) && (codFilme == removeCodFilme)){
            	printf("Excluido\n");
            	printf("codiguin do cliente excluido: %d\n", codCliente);
            	printf("codiguin do filme excluido: %d\n", codFilme);
            	fseek(file, -(tam_reg), SEEK_CUR);
            	fwrite(&invalido, sizeof(char), 1, file);
				// -------------------------------------//
				fseek(file, 0, SEEK_SET);
				fread(&header, sizeof(int), 1, file); 
				fseek(file, 0, SEEK_SET);
				fwrite(&offset, sizeof(int), 1, file);
				//fseek(file, offset+5, SEEK_SET);
				fseek(file, offset+1, SEEK_CUR);
				fwrite(&header, sizeof(int), 1, file);
				printf("offset: %d", offset);
				// --------------------------------------- //
            	controle->qtdRemovido++;
            	break;
			}else{
				offset = offset + tam_reg + 4;
				tam_reg = pega_registro(file, registro);
			}
		}else{
			offset = offset + tam_reg + 4;
			tam_reg = pega_registro(file, registro);
		}
        
}

}

int main(){
	int opcao = 0;
	FILE *file;

	if((file = fopen("mainfile.bin", "rb+")) == NULL){
		printf("Nao foi possivel encontrar o arquivo de dados =(\nVamos criar um!...\n");
		file = fopen("mainfile.bin", "wb+");
	}

	ClienteFilme vet_cliF[4];
	RemoveReg vet_rem[3];
	Controle *controle = (Controle*) malloc(sizeof(Controle));

	while(opcao != 5){
		printf("\n1. Insercao");
		printf("\n2. Remocao");
		printf("\n3. Compactacao");
		printf("\n4. Carrega Arquivos");
		printf("\n5. Sair\n");
		scanf("%d", &opcao);

		switch(opcao){
			case 1:
				inserir(vet_cliF, file, controle);
				break;
			case 2:
				remover(vet_rem, controle, file);
				break;
			case 3:
				break;
			case 4:
				carrega_arquivos(vet_cliF, vet_rem, controle);
				for(int i=0; i<3; i++){
					printf("\n");
					printf("CodCliente: %d", vet_rem[i].CodCli);
					printf("CodFilme: %d", vet_rem[i].CodF);
				}
				break;
			case 5:
				break;
		}
	}
	fclose(file);

	file = fopen("controle.bin", "rb+");
	fseek(file, 0, SEEK_SET);
	fwrite(controle, sizeof(Controle), 1, file);  
	fclose(file);                                 
	return 0;                                     
}

int pega_registro(FILE *p_out, char *p_reg){
     int bytes;

    if (!fread(&bytes, sizeof(int), 1, p_out)){
       return 0;
    }else{
        fread(p_reg, bytes, 1, p_out);
        return bytes;
    }
}
