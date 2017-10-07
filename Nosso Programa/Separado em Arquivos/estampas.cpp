/* Monta estampas */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "estampas.h"

using namespace std;

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