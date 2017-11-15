#include "global.h"
void estampas(char tipo);
void montarEstampas();

/* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal */
int resolversistema(void);

/* Rotina que conta os nos e atribui numeros a eles */
int numero(char *nome);

void zeraSistema (void);

double heaviside(double);

void salvarResultadoEmArquivo (vector <double>);

int leNetlist (void);

void atualizarMemoriasCapacitorIndutor();

void analisePontoOperacao();

void plotarGrafico();

const char* ConvertDoubleToString(double value);

void CopiaSolucaoNR ();
void ChutaValorNR ();
void ZeraValorNR ();
int ComparaValorNR ();
int gminstepping();
void monstraValoresNaoConvergindo();
void mostraResultadoParcial ();

//essa funcao printa uma barrinha de status da simulacao, ela printa a cada
//int porcento, e com o caracter char passado para a funcao (da pra escolher)
//exemplo: int = 1, char = *. printa uma barrina ******* a cada 1%
//exemplo: int = 2, char = \. printa uma barrina \\\\\\ a cada 2%
//etc...
void printProgresso(int,char);
void CopiaUltimaSolucaoConvergiu (void);
int analiseNR();
void zeraValoresNaoConvergindo (void);
void RecuperaUltimaSolucaoConvergiu (void);
void CopiaUltimaSolucaoNoTempo(void);
void RecuperaUltimaSolucaoNoTempo(void);
void ChutaValorNRComPeso(int);
