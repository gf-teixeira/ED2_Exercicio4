#include<stdio.h>

int main() {
    FILE *fd;

    //////////////////////////////
    struct CliF {
        int CodCli;
        int CodF;
        char NomeCli[50];
        char NomeF[50];
        char Genero[50];
    } vet[5] = {{1, 1, "Joao da Silva", "Indiana Jones e Os Cacadores da Arca Perdida", "Aventura"},
                {1, 2, "Joao da Silva", "Matrix", "Aventura"},
				{1, 3, "Joao da Silva", "Titanic", "Aventura"},
				{2, 3, "Pedro Silva e Silva", "Genio indomavel", "Aventura"},
                {2, 4, "Jao Da Silva Pedro Neto", "O Filme 20304 Segundo", "Aventura e Acao esporte"},
				};

    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);

    //////////////////////////////
	struct remove {
        int CodCli;
        int CodF;
    } vet_r[3] = {{1,1},
                  {1,3},
                  {2,3}};

    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

