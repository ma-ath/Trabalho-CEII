#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <time.h>
#include <tgmath.h>
#include "constantes.h"
#include "tipos.h"
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

using namespace std;

extern elemento netlist[MAX_ELEM]; /* Netlist */
extern int repete;
extern int ne;     /*Numero de Elementos */
extern int nv;     /*Numero de Variaveis */
extern int nn;     /*Numero de Nos */
extern int i;      /*Variaveis auxiliares*/
extern int j;
extern int k;
extern bool analisandoPontodeOp;
extern long double GCapacitorAberto;
extern char nomearquivo[MAX_LINHA+1];/* Foram colocados limites nos formatos de leitura para alguma protecao contra excesso de caracteres nestas variaveis */
extern char tipo;
extern char na[MAX_NOME];
extern char nb[MAX_NOME];
extern char nc[MAX_NOME];
extern char nd[MAX_NOME];
  /*lista associa o nome que o usuario deu na netlist com o numero
  que o programa definiu.
  Ex: usuario > no bonitinho = no 1 < programa (funcao "numero")*/
extern char lista[MAX_NOS+1][MAX_NOME+2]; /*Tem que caber jx antes do nome */
extern char txt[MAX_LINHA+1];
extern char *p;

extern FILE *arquivoNetlist;
extern FILE *arquivoSolucao;

extern double g, pulseRealTime, pulseOffTime;
extern double Yn[MAX_NOS+1][MAX_NOS+2];   /*Matriz a ser resolvida*/

extern double tempoAtual, tempoFinal, passo, passoPorPt;

extern double NewtonRaphsonVetor[MAX_NOS+1];
extern bool FaltaConvergir[MAX_NOS+1];
extern int iteracaoNR;
extern int erroGrande;

extern double z;

extern bool fazendoGminStepping;
extern bool circuitolinear;

#endif
