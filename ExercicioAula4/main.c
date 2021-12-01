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
	char Genero[50]; //1 + + 1+ 1  + 1+ 50 + 1 + 50 + 1 + 50 = 156
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
	}else{//arquivo Controle ainda vazio
		controle->qtdInserido = 0;
		controle->qtdRemovido = 0;
	}
	fclose(fd);
	fd = fopen("insere.bin", "ab+");
	int k =0;
	while(fread(&vet_cliF[k], sizeof(ClienteFilme), 1, fd)){
		k++;
	}
	printf("\nArquivo Insere.bin carregado");
	fclose(fd);

	fd = fopen("remove.bin", "ab+");
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
	sprintf(registro, "#%d#%d#%s#%s#%s", 
	vet_cliF[controle->qtdInserido].CodCli, 
	vet_cliF[controle->qtdInserido].CodF, 
	vet_cliF[controle->qtdInserido].NomeCli, 
	vet_cliF[controle->qtdInserido].NomeF, 
	vet_cliF[controle->qtdInserido].Genero);
	int tam_reg = strlen(registro);
	
	/*int header;
	if(fread(header, sizeof(int), 1, file)){ //se existir algo no arquivo
		if(header != -1){ //se for diferente de -1 é necessário mudar a posição do arquivo
			fseek(file, header, SEEK_SET);
			//Cada registro disponível irá apontar para outro?
		}
	}*/
	fseek(file, 0 , SEEK_SET); 
	if(fread(&header, sizeof(int), 1, file)){
		printf("Leu algo");
		printf("%d", header);
		if(header != -1){
			//fseek(header)
		}
	}else{
		header = -1;
		fwrite(&header, sizeof(int), 1, file);
	}
	fwrite(&tam_reg, sizeof(int), 1, file);
	fwrite(&validade, sizeof(char), 1, file);
	fwrite(registro, sizeof(char), tam_reg, file);
	controle->qtdInserido++;
	printf("\nInserido com sucesso.");
	
	
}
void remover(RemoveReg *vet_rem, Controle *controle, FILE *file){
	int removeCodCliente, removeCodFilme;
	int tamanho;
	char registro[156];
	char *pch;
	int flag1 = 0;
	int codCliente, codFilme;
	int tam_reg;
	printf("\ncontrole->qtdRemovido: %d", controle->qtdRemovido);
	removeCodCliente = vet_rem[controle->qtdRemovido].CodCli;
	removeCodFilme = vet_rem[controle->qtdRemovido].CodF;
	printf("\nremoveCodCliente: %d", removeCodCliente);
	printf("\nremoveCodFilme: %d", removeCodFilme);

	fseek(file, 4, SEEK_SET);  

	tam_reg = pega_registro(file,registro);
	printf("BBB");
  	while (tam_reg > 0)  
    {	
		// registro - > V#1#1#.......
		// V#1#1 
		pch = strtok(registro,"#");  // -> 1#1
		for(int i=0; i<3; i++){
			if(i==1){
				codCliente = (*pch) - '0';
			}else if(i==2){
				codFilme = (*pch) - '0';
			}
			printf("%s\n",pch);
			if(i <=2){
			pch = strtok(NULL,"#");
			}
			printf("CCCC");

		}
		printf("cliente: %d, filme: %d", codCliente, codFilme);
		if((codCliente == removeCodCliente) && (codFilme == removeCodFilme)){
			printf("Excluido");
			break;
		}
		printf("AAAA");
		 tam_reg = pega_registro(file,registro); 
		 printf("%d", tam_reg);
		
	} 
	controle->qtdRemovido++;
	
	
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

	file = fopen("controle.bin", "wb+");
	fwrite(controle, sizeof(controle), 1, file);
	fclose(file);

	return 0;
}



/*
fseek(out,0,0);
  tam_reg = pega_registro(file,registro);
  while (tam_reg > 0)  
   {
    pch = strtok(registro,"|");  
    while (pch != NULL)
      {
        printf("%s\n",pch);
        pch = strtok(NULL,"|");
      }
    printf("\n");
    tam_reg = pega_registro(file,registro);  
   } 

  fclose(out);

}*/

int pega_registro(FILE *p_out, char *p_reg)
{
     int bytes;
     
     if (!fread(&bytes, sizeof(int), 1, p_out))
       return 0;
     else {
            fread(p_reg, bytes, 1, p_out);   
            return bytes;
          }  
}     

//*/



/*while(fread(&tamanho, sizeof(int), 1, file)){
		fseek(file, 1, SEEK_CUR);
		fread(codigos, sizeof(char), 4, file);
		//75V#1#1..
		pch = strtok(codigos,"#");  
		for(int i=0; i<2; i++){
			if(i==0){
				codCliente = (int) (*pch);
				if(codCliente == removeCodCliente){
					flag1 = 1;
				}
			}else{
				codFilme = (int) (*pch);
				if(codFilme == removeCodFilme){
					flag1 = 1;
				}else{
					flag1 = 0;
				}
			}
			
			pch = strtok(NULL,"#");
		}
		if(flag1 == 1){
			printf("Achou");
		}
	}*/