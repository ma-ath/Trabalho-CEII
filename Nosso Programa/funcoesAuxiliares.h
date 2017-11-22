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
//le netlist e ja te diz se circuito eh linear ou nao
int leNetlist (void);
void inicializaNetlist(int *,int*);
void atualizarMemoriasCapacitorIndutor();
void analisePontoOperacao();
//essa funcao printa uma barrinha de status da simulacao, ela printa a cada
//int porcento, e com o caracter char passado para a funcao (da pra escolher)
//exemplo: int = 1, char = *. printa uma barrina ******* a cada 1%
//exemplo: int = 2, char = \. printa uma barrina \\\\\\ a cada 2%
//etc...
void printProgresso(int,char);
void plotarGrafico();
void InicializaVetorFaltaConvergir();
void AtualizaNR();
bool SolucaoConvergiuTeste ();
void AdicionaGSSePreciso();
bool analiseNR();
void ArmazenaUltimaSolucaoYn ();
void RecuperaUltimaSolucaoYn ();
//const char* ConvertDoubleToString(double value);

void ZeraValorNR ();
int ComparaValorNR ();
void monstraValoresNaoConvergindo();
void mostraResultadoParcial ();
void AproxInicialNR();
void gminstepping();
void zeraValoresNaoConvergindo (void);
void mostraResultadoNR ();
//int ElementoPrecisaGs (void);
