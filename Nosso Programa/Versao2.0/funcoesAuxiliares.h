#include "global.h"
void estampas(char tipo);

/* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal */
int resolversistema(void);

/* Rotina que conta os nos e atribui numeros a eles */
int numero(char *nome);

void zeraSistema (void);

double heaviside(double);

void salvarResultadoEmArquivo (vector <double>);

int leNetlist (void);

void atualizarMemoriasCapacitorIndutor(char tipo);

void analisePontoOperacao(char tipo);

//essa funcao printa uma barrinha de status da simulacao, ela printa a cada
//int porcento, e com o caracter char passado para a funcao (da pra escolher)
//exemplo: int = 1, char = *. printa uma barrina ******* a cada 1%
//exemplo: int = 2, char = \. printa uma barrina \\\\\\ a cada 2%
//etc...
void printProgresso(int,char);

/*void salvarResultadoEmArquivo(vector < vector<double> > tabela)*/

/* Monta estampas
using namespace std;

class estampas
{
public:
    this* R(int i);
    this* G(int i);
    this* I(int i);
    this* V(int i);
    this* E(int i);
    this* F(int i);
    this* H(int i);
    this* O(int i);

    this* MostrarEstampas();
}
*/




/*TIRADO DO MNA1*/
/*
for (i=1; i<=ne; i++)
{
  tipo=netlist[i].nome[0];
  if (tipo=='R') {
    g=1/netlist[i].valor;
    Yn[netlist[i].a][netlist[i].a]+=g;
    Yn[netlist[i].b][netlist[i].b]+=g;
    Yn[netlist[i].a][netlist[i].b]-=g;
    Yn[netlist[i].b][netlist[i].a]-=g;
  }
  else if (tipo=='G') {
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].c]+=g;
    Yn[netlist[i].b][netlist[i].d]+=g;
    Yn[netlist[i].a][netlist[i].d]-=g;
    Yn[netlist[i].b][netlist[i].c]-=g;
  }
  else if (tipo=='I') {
    g=netlist[i].valor;
    Yn[netlist[i].a][nv+1]-=g;
    Yn[netlist[i].b][nv+1]+=g;
  }
  else if (tipo=='V') {
    Yn[netlist[i].a][netlist[i].x]+=1;
    Yn[netlist[i].b][netlist[i].x]-=1;
    Yn[netlist[i].x][netlist[i].a]-=1;
    Yn[netlist[i].x][netlist[i].b]+=1;
    Yn[netlist[i].x][nv+1]-=netlist[i].valor;
  }
  else if (tipo=='E') {
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].x]+=1;
    Yn[netlist[i].b][netlist[i].x]-=1;
    Yn[netlist[i].x][netlist[i].a]-=1;
    Yn[netlist[i].x][netlist[i].b]+=1;
    Yn[netlist[i].x][netlist[i].c]+=g;
    Yn[netlist[i].x][netlist[i].d]-=g;
  }
  else if (tipo=='F') {
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].x]+=g;
    Yn[netlist[i].b][netlist[i].x]-=g;
    Yn[netlist[i].c][netlist[i].x]+=1;
    Yn[netlist[i].d][netlist[i].x]-=1;
    Yn[netlist[i].x][netlist[i].c]-=1;
    Yn[netlist[i].x][netlist[i].d]+=1;
  }
  else if (tipo=='H') {
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].y]+=1;
    Yn[netlist[i].b][netlist[i].y]-=1;
    Yn[netlist[i].c][netlist[i].x]+=1;
    Yn[netlist[i].d][netlist[i].x]-=1;
    Yn[netlist[i].y][netlist[i].a]-=1;
    Yn[netlist[i].y][netlist[i].b]+=1;
    Yn[netlist[i].x][netlist[i].c]-=1;
    Yn[netlist[i].x][netlist[i].d]+=1;
    Yn[netlist[i].y][netlist[i].x]+=g;
  }
  else if (tipo=='O') {
    Yn[netlist[i].a][netlist[i].x]+=1;
    Yn[netlist[i].b][netlist[i].x]-=1;
    Yn[netlist[i].x][netlist[i].c]+=1;
    Yn[netlist[i].x][netlist[i].d]-=1;
  }
#ifdef DEBUG
  / Opcional: Mostra o sistema apos a montagem da estampa /
  printf("Sistema apos a estampa de %s\n",netlist[i].nome);
  for (k=1; k<=nv; k++) {
    for (j=1; j<=nv+1; j++)
      if (Yn[k][j]!=0) printf("%+3.1f ",Yn[k][j]);
      else printf(" ... ");
    printf("\n");
  }
  getch();
#endif
}
*/
