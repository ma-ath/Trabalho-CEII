#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "global.h"
#include "constantes.h"
#include "funcoesmna1.h"

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
