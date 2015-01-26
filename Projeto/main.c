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
} indice1;

typedef struct
{
	char vacina[30];
	int offset;
} indice2;

indice1 INDEX1[50];
indice2 INDEX2[50];
indice1 INDEX3[50];
int tam1 = 0, tam2 = 0, tam3 = 0; 

void AbreArquivos(FILE *AP1, FILE *AP2, FILE *IndPrim, FILE *IndSec1, FILE *IndSec2);
int ExisteCachorro(int codigo, FILE *AP2);
void CadastraCachorro(FILE *AP2);
int ExigeRecriaIndice(FILE *arq);
void AtualizaInfoIndice(char status, FILE *arq);
void RecriaIndicePrim(FILE *AP1);
void QuickSortInd1(indice1 aux[], int left, int right);
int ParticionarQSInd1(indice1 aux[], int left, int right, int pivo);
void TrocaQSInd1(indice1 aux[], int i, int j);

int main()
{
	FILE *AP1, *AP2, *IndPrim, *IndSec1, *IndSec2;
    
    
    AbreArquivos(AP1, AP2, IndPrim, IndSec1, IndSec2);
    getch();
    return 0;
}

/*
DESCRI��O: Verifica se os arquivos j� foram criados.
           Se n�o, cria-os.
PAR�METROS: AP1 - Arquivo Principal 1
            AP2 - Arquivo Principal 2
	        IndPrim - Arquivo de �ndice (busca por chave prim�ria)
			IndSec1 - Arquivo de �ndice 1 (busca por chave secund�ria)
			IndSec2 - Arquivo de �ndice 2 (busca por chave secund�ria)
*/
void AbreArquivos(FILE *AP1, FILE *AP2, FILE *IndPrim, FILE *IndSec1, FILE *IndSec2)
{
    if ((AP1 = fopen("AP1.bin", "r+b")) == NULL)
    {
    	AP1 = fopen("AP1.bin", "w+b");
    	fprintf(AP1, "%d", -1);
    	IndPrim = fopen("IndPrim.bin", "w+b");
    	AtualizaInfoIndice('!', IndPrim);
    	IndSec1 = fopen("IndSec1.bin", "w+b");
    	AtualizaInfoIndice('!', IndSec1);
    	IndSec2 = fopen("IndSec2.bin", "w+b");
    	AtualizaInfoIndice('!', IndSec2);			
	}
	else if (ExigeRecriaIndice(IndPrim))
	{
		RecriaIndicePrim(AP1);
		QuickSortInd1(INDEX1, 0, tam1);
	}
	else
	{
		IndPrim = fopen("IndPrim.bin", "r+b");
    	IndSec1 = fopen("IndSec1.bin", "r+b");
    	IndSec2 = fopen("IndSec2.bin", "r+b");
	}
    	
    if ((AP2 = fopen("AP2.bin", "r+b")) == NULL)
        AP2 = fopen("AP2.bin", "w+b");
}

/*
DESCRI��O: Verifica se o c�digo j� existe no arquivo.
PAR�METROS: codigo - C�digo a ser verificado
            AP2 - Arquivo Principal 2
RETORNOS: 0 - N�o existe um cachorro com o c�digo passado por par�metro
          1 - Existe um cachorro com o c�digo passado por par�metro
*/
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

/*
DESCRI��O: Cadastra informa��es de um cachorro no Arquivo Principal 2
PAR�METROS: AP2 - Arquivo Principal 2
*/
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

/*
DESCRI��O: Verifica se � necess�rio recriar um �ndice a partir de um arquivo
PAR�METRO: ind - �ndice a ser verificado
RETORNOS: 0 - N�o � necess�rio recriar o �ndice
          1 - � necess�rio recriar o �ndice
*/
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

/*
DESCRI��O: Atualiza o header do �ndice com o status de atualiza��o
PAR�METROS: status - '*' - �ndice atualizado
                     '!' - �ndice desatualizado
            arq - �ndice a ser atualizado
*/
void AtualizaInfoIndice(char status, FILE *arq)
{
	fseek(arq, 0, 0);
	fputc(status, arq);	
}

/*
DESCRI��O: Recria o �ndice prim�rio a partir do arquivo e carrega em mem�ria principal
PAR�METRO: AP1 - Arquivo Principal 1
*/
void RecriaIndicePrim(FILE *AP1)
{
	int deslocamento, aux;
	char ch;
	
	fseek(AP1, sizeof(int), SEEK_SET); //header
	ch = fgetc(AP1);
	if (ch == EOF)
		return;
	else
	{
		aux = ftell(AP1);
		fseek(AP1, 2*sizeof(int), SEEK_SET); //primeiro registro
		ch = fgetc(AP1);
		while (ch != EOF)
		{
			if (ch != '!')
			{
				INDEX1[tam1].offset = ftell(AP1) + 1;
				fseek(AP1, INDEX1[tam1].offset, SEEK_SET);
				fscanf(AP1, "%d", INDEX1[tam1].codigo);
				tam1++;
			}
			fseek(AP1, 0, aux);
			fscanf(AP1, "%d", deslocamento);
			aux += deslocamento;
			fseek(AP1, sizeof(int), aux);
			ch = fgetc(AP1);
		}	
	}
}

//QuickSort
void TrocaQSInd1(indice1 aux[], int i, int j) 
{
    indice1 t = aux[i];
    aux[i] = aux[j];
    aux[j] = t;
}
 
int ParticionarQSInd1(indice1 aux[], int left,int right,int pivo)
{
    int pos, i;
    TrocaQSInd1(aux, pivo, right);
    pos = left;
    for(i = left; i < right; i++)
    {
        if (aux[i].codigo < aux[right].codigo)
        {
            TrocaQSInd1(aux, i, pos);
            pos++;
        }
    }
    TrocaQSInd1(aux, right, pos);
    return pos;
}
 
void QuickSortInd1(indice1 aux[], int left, int right)
{
	int pivo, pos;
	
    if (left < right)
    {
        pivo = (left + right) / 2;
        pos = ParticionarQSInd1(aux,left,right,pivo);
        QuickSortInd1(aux, left, pos - 1);
        QuickSortInd1(aux, pos + 1, right);
    }
}

// Comentario de teste para o pull request!











