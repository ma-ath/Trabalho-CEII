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
  double valor; /*usado tbm como DC "valor", SIN "nivel continuo", PULSE "amplitude1"*/
  int a,b,c,d,x,y;
  /*a partir daqui so vai usar se for uma fonte que n eh dc: I ou V que tem <parametro>*/
  char fonte[MAX_NOME_FONTE]; //se eh DC, SIN, ou PULSE
  double amplitude /*anplitude2 para pulse*/, freq, atraso, amortecimento,
          defasagem, ciclo, tempoSubida, tempoDescida, tempoLigada, periodo;

  /*Aqui sao os parametros de memoria para o capacitor e indutor no tempo*/
  double vt0; /*Tensao anterior sobre o capacitor/indutor*/
  double jt0; /*Corrente anterior sobre o capacitor/indutor*/
  double gon ; //condutancia da chave se vcd>vref
  double goff;//condutancia da chave se vcd<=vref
} elemento;

#endif
