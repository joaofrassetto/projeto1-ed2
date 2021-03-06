#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

//Cria��o da estrutura para AP2
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

int Menu();
void AbreArquivos(FILE **AP1, FILE **AP2, FILE **IndPrim, FILE **IndSec1, FILE **IndSec2);
void CadastraVacina(FILE **AP1, FILE **AP2);
void CadastraCachorro(FILE **AP2);
void AtualizaInfoIndice(char status, FILE **arq);
int ExisteCachorro(int codigo, FILE **AP2);
int ProcuraEspacoVazio(FILE **AP1, int tam_reg);

int main() 
{
    int opcao;
	FILE *AP1, *AP2, *IndPrim, *IndSec1, *IndSec2;
	
	AbreArquivos(&AP1, &AP2, &IndPrim, &IndSec1, &IndSec2);
	opcao = Menu();
	while (1)
	{
	    switch(opcao)
	    {
	        case 1: CadastraCachorro(&AP2); break;
	        case 0: printf("\nSaindo do Programa..."); 
        	        fclose(AP1);
                    fclose(AP2);
                    fclose(IndPrim);
                    fclose(IndSec1);
                    fclose(IndSec2);
                    getch(); return 0;
	        default: printf("\nOpcao invalida!"); getch(); break;
        }
        opcao = Menu();
    }
}

/*
DESCRI��O: Exibe as op��es do menu.
RETORNO: O n�mero da op��o escolhida pelo usu�rio
*/
int Menu()
{
	int opcao;
	
	system("CLS");
    printf("\n1 - Cadastra Cachorro");
	printf("\n0 - Sair");
	printf("\n\nEscolha a opcao: ");
    scanf("%d", &opcao);
	
	return opcao;
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
void AbreArquivos(FILE **AP1, FILE **AP2, FILE **IndPrim, FILE **IndSec1, FILE **IndSec2)
{
    if ((*AP1 = fopen("AP1.bin", "r+b")) == NULL)
    {
    	*AP1 = fopen("AP1.bin", "w+b");
    	fprintf(*AP1, "%d", -1);
    	*IndPrim = fopen("IndPrim.bin", "w+b");
    	AtualizaInfoIndice('!', IndPrim);
    	*IndSec1 = fopen("IndSec1.bin", "w+b");
    	AtualizaInfoIndice('!', IndSec1);
    	*IndSec2 = fopen("IndSec2.bin", "w+b");
    	AtualizaInfoIndice('!', IndSec2);
	}
	/*else if (ExigeRecriaIndice(IndPrim))
	{
		RecriaIndicePrim(AP1);
		QuickSortInd1(INDEX1, 0, tam1);
	}
	else
	{
		IndPrim = fopen("IndPrim.bin", "r+b");
		CarregaIndice(IndPrim, 1);
    	IndSec1 = fopen("IndSec1.bin", "r+b");
    	CarregaIndice(IndSec1, 2);
    	IndSec2 = fopen("IndSec2.bin", "r+b");
    	CarregaIndice(IndSec2, 1);
	}*/
    	
    if ((*AP2 = fopen("AP2.bin", "r+b")) == NULL)
        *AP2 = fopen("AP2.bin", "w+b");
    
}

/*
DESCRI��O: Cadastra informa��es de um cachorro no Arquivo Principal 2
PAR�METROS: AP2 - Arquivo Principal 2
*/
void CadastraCachorro(FILE **AP2)
{
	registro reg;
	
	system("CLS");
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
	fflush(stdin);
	printf("Raca: ");
	gets(reg.raca);
	printf("Nome do Cachorro: ");
	gets(reg.nome);

	
	fseek(*AP2, 0, SEEK_END); //seta a posi��o para o fim do arquivo.
	fwrite(&reg, sizeof(reg), 1, *AP2); //escreve no fim do arquivo.
}

/*
DESCRI��O: Atualiza o header do �ndice com o status de atualiza��o
PAR�METROS: status - '*' - �ndice atualizado
                     '!' - �ndice desatualizado
            arq - �ndice a ser atualizado
*/
void AtualizaInfoIndice(char status, FILE **arq)
{
	fseek(*arq, 0, SEEK_SET);
	fputc(status, *arq);	
}

/*
DESCRI��O: Verifica se o c�digo j� existe no arquivo.
PAR�METROS: codigo - C�digo a ser verificado
            AP2 - Arquivo Principal 2
RETORNOS: 0 - N�o existe um cachorro com o c�digo passado por par�metro
          1 - Existe um cachorro com o c�digo passado por par�metro
*/
int ExisteCachorro(int codigo, FILE **AP2)
{
	registro reg;
	
	rewind(*AP2);
	while (fread(&reg, sizeof(registro), 1, *AP2))
	{
		if (reg.codigo == codigo)
		{
			return 1;
			break;
		}	
	}
    return 0;	
}

/*
DESCRI��O: Realiza o cadastro de vacinas
PAR�METROS: AP1 - Arquivo principal 1
            AP2 - Arquivo principal 2
*/
void CadastraVacina(FILE **AP1, FILE **AP2)
{
    int cod_controle, cod_cachorro, tam_reg, posicao;
    char verificador = '*', vacina[30], data[6], respo[100], registro[255];
    
    system("CLS");
    printf("\n\nDigite o c�digo do cachorro <-1 para cadastrar um cachorro>: ");
    scanf("%d", &cod_cachorro);
    if (cod_cachorro == -1)
      CadastraCachorro(AP2);
    else
    {
        while (!ExisteCachorro(cod_cachorro, AP2))
        {   
            printf("\n\nCachorro inexistente. Digite novamente!");
            system("CLS");  
            printf("\n\nDigite o c�digo do cachorro <-1 para cadastrar um cachorro>: ");
            scanf("%d", &cod_cachorro);
            if (cod_cachorro == -1)
              CadastraCachorro(AP2);    
        }
        system("CLS");
        cod_controle = 1;//cod_controle = pegar do INDEX1 ordenado;
        printf("Codigo do cachorro: %d", cod_cachorro);
        fflush(stdin);
        printf("\nNome da vacina: ");
        gets(vacina);
        printf("\nData de vacinacao <MM/AA>: ");
        gets(data);
        printf("\nResponsavel pela aplicacao: ");
        gets(respo);
        sprintf(registro, "%d|%d|%s|%s|%s|", cod_controle, cod_cachorro, vacina, data, respo);
        tam_reg = strlen(registro);
        posicao = ProcuraEspacoVazio(AP1, tam_reg);
        if (posicao != -1)
          fseek(*AP1, posicao, SEEK_SET);
        else
          fseek(*AP1, 0, SEEK_END);
        fwrite(&tam_reg, sizeof(int), 1, *AP1);
        fwrite(&verificador, sizeof(char), 1, *AP1);
        fwrite(registro, sizeof(char), tam_reg, *AP1);
    }
}

/*
DESCRI��O: Retorna o espa�o vazio encontrado no arquivo
PAR�METROS: AP1 - Arquivo principal 1
            tam_reg - tamanho do registro a ser inserido
RETORNO: posi��o livre para ser escrita
*/
int ProcuraEspacoVazio(FILE **AP1, int tam_reg)
{
    int offset, tam, pos;
    char ch;
    
    rewind(*AP1);
    fread(&offset, sizeof(int), 1, *AP1);
    if (offset == -1)
        return -1;
    else
    {
        fseek(*AP1, offset, SEEK_SET);
        pos = ftell(*AP1);
        while (fread(&tam, sizeof(int), 1, *AP1))
        {
            if (tam == -1)
            {
                return -1;
                break;
            }
            else
            {
                ch = fgetc(*AP1);
                if ((tam > tam_reg) && (ch == '!'))
                {
                   return pos;
                   break;                 
                }
                else
                {
                    fread(&offset, sizeof(int), 1, *AP1);
                    fseek(*AP1, offset, SEEK_SET);
                }   
            }     
        }   
    }
}

