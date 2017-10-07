/* Monta estampas */

#include "funcoesAuxiliares.h"
#include "global.h"

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

/*using namespace std;

this* estampas::R(int i)
{
  g=1/netlist[i].valor;
  Yn[netlist[i].a][netlist[i].a]+=g;
  Yn[netlist[i].b][netlist[i].b]+=g;
  Yn[netlist[i].a][netlist[i].b]-=g;
  Yn[netlist[i].b][netlist[i].a]-=g;
  return this;
};
this* estampas::G(int i)
{
  g=netlist[i].valor;
  Yn[netlist[i].a][netlist[i].c]+=g;
  Yn[netlist[i].b][netlist[i].d]+=g;
  Yn[netlist[i].a][netlist[i].d]-=g;
  Yn[netlist[i].b][netlist[i].c]-=g;
  return this;
};
this* estampas::I(int i)
{
  g=netlist[i].valor;
  Yn[netlist[i].a][nv+1]-=g;
  Yn[netlist[i].b][nv+1]+=g;
  return this;
};
this* estampas::V(int i)
{
  Yn[netlist[i].a][netlist[i].x]+=1;
  Yn[netlist[i].b][netlist[i].x]-=1;
  Yn[netlist[i].x][netlist[i].a]-=1;
  Yn[netlist[i].x][netlist[i].b]+=1;
  Yn[netlist[i].x][nv+1]-=netlist[i].valor;
  return this;
};
this* estampas::E(int i)
{
  g=netlist[i].valor;
  Yn[netlist[i].a][netlist[i].x]+=1;
  Yn[netlist[i].b][netlist[i].x]-=1;
  Yn[netlist[i].x][netlist[i].a]-=1;
  Yn[netlist[i].x][netlist[i].b]+=1;
  Yn[netlist[i].x][netlist[i].c]+=g;
  Yn[netlist[i].x][netlist[i].d]-=g;
  return this;
};
this* estampas::F(int i)
{
  g=netlist[i].valor;
  Yn[netlist[i].a][netlist[i].x]+=g;
  Yn[netlist[i].b][netlist[i].x]-=g;
  Yn[netlist[i].c][netlist[i].x]+=1;
  Yn[netlist[i].d][netlist[i].x]-=1;
  Yn[netlist[i].x][netlist[i].c]-=1;
  Yn[netlist[i].x][netlist[i].d]+=1;
  return this;
};
this* estampas::H(int i)
{
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
  return this;
};
this* estampas::O(int i)
{
  Yn[netlist[i].a][netlist[i].x]+=1;
  Yn[netlist[i].b][netlist[i].x]-=1;
  Yn[netlist[i].x][netlist[i].c]+=1;
  Yn[netlist[i].x][netlist[i].d]-=1;
  return this;
};

this* estampas::MostrarEstampas()
{
  for (k=1; k<=nv; k++)
  {
    for (j=1; j<=nv+1; j++)
      if (Yn[k][j]!=0) printf("%+3.1f ",Yn[k][j]);
      else printf(" ... ");
    printf("\n");
  }

  return this;
};
*/
