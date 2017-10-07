#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "constantes.h"

typedef struct elemento /* Elemento do netlist */
{
  char nome[MAX_NOME];
  double valor;
  int a,b,c,d,x,y;
} elemento;

elemento netlist[MAX_ELEM]; /* Netlist */

int ne,     /*Numero de Elementos */
    nv,     /*Numero de Variaveis */
    nn,     /*Numero de Nos */
    i,j,k;  /*Variaveis auxiliares*/

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  nomearquivo[MAX_LINHA+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  /*lista associa o nome que o usuario deu na netlist com o numero
  que o programa definiu.
  Ex: usuario > no bonitinho = no 1 < programa (funcao "numero")*/
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1],
  *p;

FILE *arquivo;

double
  g,
  Yn[MAX_NOS+1][MAX_NOS+2];   /*Matriz a ser resolvida*/
