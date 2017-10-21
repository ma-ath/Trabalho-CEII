#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "constantes.h"

#ifndef _TIPOS_H_
#define _TIPOS_H_

typedef struct elemento /* Elemento do netlist */
{
  char nome[MAX_NOME];
  double valor;
  int a,b,c,d,x,y;
} elemento;

#endif
