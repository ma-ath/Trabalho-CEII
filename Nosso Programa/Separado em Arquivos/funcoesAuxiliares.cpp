/* Monta estampas */
#include "funcoesAuxiliares.h"
#include "global.h"

using namespace std;

void estampas(char tipo)
{
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
  else if (tipo=='V')   /*A estampa de V deve mudar de acordo com o tempoAtual*/
  {
    if (strcmp(netlist[i].fonte, "DC") == 0)
    /*
    Fonte DC - Nao muda com o tempoAtual
    */
    {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=netlist[i].valor;
    }
    else
    if (strcmp(netlist[i].fonte, "SIN") == 0)
    /*
    Fonte SIN - Muda de acordo com:
    off+Vp*exp(-dec*(tempoAtual-atraso))*sin(2*pi*f*(tempoAtual-atraso)+(pi/180)*fase)
    */
    {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=
        (
          netlist[i].valor +
          (
            netlist[i].amplitude*exp(-1*netlist[i].amortecimento*(tempoAtual-netlist[i].atraso)) *
            sin(2*PI*netlist[i].freq*(tempoAtual-netlist[i].atraso) + (PI/180)*netlist[i].defasagem)
          )*( heaviside(tempoAtual-netlist[i].atraso) -
              heaviside((tempoAtual-netlist[i].atraso) - (2*PI/netlist[i].freq)*netlist[i].ciclo) )
        );
    }
    if (strcmp(netlist[i].fonte, "PULSE") == 0)
    /*
    Fonte PULSE - Muda de acordo com:
    add expressao aqui
    */
    {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=netlist[i].valor;
    }
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
  else if (tipo=='K') {     /*Transformador Ideal*/
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].x]-=g;
    Yn[netlist[i].b][netlist[i].x]+=g;
    Yn[netlist[i].c][netlist[i].x]+=1;
    Yn[netlist[i].d][netlist[i].x]-=1;

    Yn[netlist[i].x][netlist[i].a]+=g;
    Yn[netlist[i].x][netlist[i].b]-=g;
    Yn[netlist[i].x][netlist[i].c]-=1;
    Yn[netlist[i].x][netlist[i].d]+=1;
  }
}

int resolversistema(void)
{
  int i,j,l,a;
  double t, p;

  for (i=1; i<=nv; i++)
  {
    t=0.0;
    a=i;
    for (l=i; l<=nv; l++)
    {
      if (fabs(Yn[l][i])>fabs(t))
      {
	       a=l;
	       t=Yn[l][i];
      }
    }
    if (i!=a)
    {
      for (l=1; l<=nv+1; l++)
      {
	       p=Yn[i][l];
	       Yn[i][l]=Yn[a][l];
	       Yn[a][l]=p;
      }
    }
    if (fabs(t)<TOLG)
    {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=nv+1; j>0; j--)
    {  /* Basta j>i em vez de j>0 */
      Yn[i][j]/= t;
      p=Yn[i][j];
      if (p!=0)  /* Evita operacoes com zero */
        for (l=1; l<=nv; l++)
        {
	         if (l!=i)
	         Yn[l][j]-=Yn[l][i]*p;
        }
    }
  }
  return 0;
}

int numero(char *nome)
{
  int i,achou;
  i=0; achou=0;
  while (!achou && i<=nv)
    if (!(achou=!strcmp(nome,lista[i]))) i++;
  if (!achou) {
    if (nv==MAX_NOS)
    {
      printf("O programa so aceita ate %d nos\n",nv);
      exit(1);
    }
    nv++;
    strcpy(lista[nv],nome);
    return nv; /* novo no */
  }
  else
  {
    return i; /* no ja conhecido */
  }
}

void zeraSistema (void) {
  for (i=0; i<=nv; i++)
    for (j=0; j<=nv+1; j++)
      Yn[i][j]=0;
}

double heaviside(double t)
{
  if (t >= 0)
    return 1;
  else
    return 0;
}

void salvarResultadoEmArquivo (vector <double> resultadoUmTempo)
{
  //  ofstream arquivo;
     //arquivo.open ("analise no tempo.txt", ios::app);
     for (i=0; i < static_cast <int> (resultadoUmTempo.size()); i++){
       fprintf (arquivoSolucao, "%g ",resultadoUmTempo.at(i) );
     }
     fprintf (arquivoSolucao, "\n");
    // arquivo.close();
}
/*
void salvarResultadoEmArquivo(vector < vector<double> > tabela)
{
  unsigned aux = 0;
  unsigned aux2 = 0;
  ofstream arquivo;

  arquivo.open (srtcat(nomearquivo,".tab"));
  if (arquivo.isopen())
  {
    while(aux <= tabela.size())
    {
      aux2 = 0;
      while(aux2 <= tabela[0].size())
      {
        arquivo << static_cast <char*> ( casttabela[aux2].at(aux) );
        aux2++;
      }
      aux++;
    }
  }
  else
    cout << "Erro ao salvar dados - Nao foi capaz de abrir o arquivo";

}

PROGRAMA DE TESTES QUE EU COMECEI A FAZER MAS NAO FUNCIONA (BOTEI AQUI SO PRA SALVAR)
#include <vector>
#include <fstream>
#include <cstdio>
char nomearquivo[100] = "teste.net";

using namespace std;

void salvarResultadoEmArquivo(vector < vector<double> > tabela)
{
  unsigned aux = 0;
  unsigned aux2 = 0;
  ofstream arquivo;

  arquivo.open (strcat(nomearquivo,".tab"));
  if (arquivo.isopen())
  {
    while(aux <= tabela.size())
    {
      aux2 = 0;
      while(aux2 <= tabela[0].size())
      {
        arquivo << static_cast <char*> ( tabela[aux2].at(aux) );
        aux2++;
      }
      aux++;
    }
  }
  else
    cout << "Erro ao salvar dados - Nao foi capaz de abrir o arquivo";

}

int main()
{
vector < vector <double> > teste;

teste.push_back(0);
teste.push_back(1);
teste.push_back(2);

salvarResultadoEmArquivo(teste);

return 0;
}

*/
