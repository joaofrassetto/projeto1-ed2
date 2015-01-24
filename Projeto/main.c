#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct
{
	int codigo;
	char raca[30], nome[100];
} registro;

typedef struct
{
	int codigo, offset;
} indiceprim;

void AbreArquivos(FILE *AP1, FILE *AP2, FILE *IndPrim, FILE *IndSec1, FILE *IndSec2);
int ExisteCachorro(int codigo, FILE *AP2);
void CadastraCachorro(FILE *AP2);
int ExigeRecriaIndice(FILE *arq);
void AtualizaInfoIndice(char status, FILE *arq);
void RecriaIndices();

int main()
{
	FILE *AP1, *AP2, *IndPrim, *IndSec1, *IndSec2;
    
    
    AbreArquivos(AP1, AP2, IndPrim, IndSec1, IndSec2);
    getch();
    return 0;
}

/*DESCRI��O: Verifica se os arquivos j� foram criados.
             Se n�o, cria-os.
  PAR�METROS: AP1 - Arquivo Principal 1
              AP2 - Arquivo Principal 2
			  IndPrim - Arquivo de �ndice (busca por chave prim�ria)
			  IndSec1 - Arquivo de �ndice 1 (busca por chave secund�ria)
			  IndSec2 - Arquivo de �ndice 2 (busca por chave secund�ria) */
void AbreArquivos(FILE *AP1, FILE *AP2, FILE *IndPrim, FILE *IndSec1, FILE *IndSec2)
{
    if ((AP1 = fopen("AP1.bin", "r+b")) == NULL)
    {
    	AP1 = fopen("AP1.bin", "w+b");
    	fprintf(AP1, "%d", -1);
    	//AtualizaInfoIndice('*', IndPrim);
    	//AtualizaInfoIndice('*', IndSec1);
    	//AtualizaInfoIndice('*', IndSec2);			
	}
	/*else if (ExigeRecriaIndice(IndPrim))
	{
		
	}*/
    	
    if ((AP2 = fopen("AP2.bin", "r+b")) == NULL)
        AP2 = fopen("AP2.bin", "w+b");
}

/*DESCRI��O: Verifica se o c�digo j� existe no arquivo.
  PAR�METROS: codigo - C�digo a ser verificado
              AP2 - Arquivo Principal 2
  RETORNOS: 0 - N�o existe um cachorro com o c�digo passado por par�metro
  			1 - Existe um cachorro com o c�digo passado por par�metro*/
int ExisteCachorro(int codigo, FILE *AP2)
{
	int i = 0;
	registro reg;
	
	return 0;
	rewind(AP2);
	while (fread(&reg, sizeof(reg)*i, 1, AP2) >= 1)
	{
		if (reg.codigo == codigo)
		{
			return 1;
			break;
		}
		i++;		
	}	
}

/*DESCRI��O: Cadastra informa��es de um cachorro no Arquivo Principal 2
  PAR�METROS: AP2 - Arquivo Principal 2*/
void CadastraCachorro(FILE *AP2)
{
	registro reg;
	
	printf("\nCodigo: ");
	scanf("%d", &reg.codigo);
	while (ExisteCachorro(reg.codigo, AP2))
	{
		system("CLS");
		printf("\nCodigo ja cadastrado. Digite novamente!");
		getch();
		system("CLS");
		printf("\nCodigo: ");
		scanf("%d", &reg.codigo);
	}
	printf("Raca: ");
	gets(reg.raca);
	printf("Nome do Cachorro: ");
	gets(reg.nome);
	
	fseek(AP2, 0, SEEK_END); //seta a posi��o para o fim do arquivo.
	fwrite(&reg, sizeof(reg), 1, AP2); //escreve no fim do arquivo.
}

/*DESCRI��O: Verifica se � necess�rio recriar um �ndice a partir de um arquivo
  PAR�METRO: ind - �ndice a ser verificado
  RETORNOS: 0 - N�o � necess�rio recriar o �ndice
            1 - � necess�rio recriar o �ndice*/
int ExigeRecriaIndice(FILE *arq)
{
	char ch;
	
	fseek(arq, 0, 0);
	ch = fgetc(arq);
	if (ch == '!')
	  return 1;
	else
	  return 0;
}

/*DESCRI��O: Atualiza o header do �ndice com o status de atualiza��o
  PAR�METROS: status - '*' - �ndice atualizado
                       '!' - �ndice desatualizado
			  arq - �ndice a ser atualizado*/
void AtualizaInfoIndice(char status, FILE *arq)
{
	fseek(arq, 0, 0);
	fputc(status, arq);	
}















