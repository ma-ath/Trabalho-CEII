/*
Elementos aceitos e linhas do netlist:

Resistor:  R<nome> <no+> <no-> <resistencia>
VCCS:      G<nome> <io+> <io-> <vi+> <vi-> <transcondutancia>
VCVC:      E<nome> <vo+> <vo-> <vi+> <vi-> <ganho de tensao>
CCCS:      F<nome> <io+> <io-> <ii+> <ii-> <ganho de corrente>
CCVS:      H<nome> <vo+> <vo-> <ii+> <ii-> <transresistencia>
Fonte I:   I<nome> <io+> <io-> <corrente>
Fonte V:   V<nome> <vo+> <vo-> <tensao> <parâmetros>
Amp. op.:  O<nome> <vo1> <vo2> <vi1> <vi2>
Indutor:   L<nome> <no+> <no-> <indutancia> [Corrente Inicial]
Capacitor: C<nome> <no+> <no-> <Capacitância> [Tensão Inicial]

Parâmetros para as fontes de tensão (V):
    Fonte DC : DC <valor>
    Fonte senoidal: SIN <nível contínuo> <amplitude> <frequencia> <atraso> <amortecimento> <defasagem>
    Fonte pulso: PULSE <amplitude1> <amplitude2> <atraso> <tempo de subida> <tempo de descida> <tempo ligada> <período> <número de cíclos>

As fontes F e H tem o ramo de entrada em curto
O amplificador operacional ideal tem a saida suspensa
Os nos podem ser nomes
*/

#define versao "1.0j - 13/02/2016"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#define MAX_LINHA 80
#define MAX_NOME 11
#define MAX_ELEM 50
#define MAX_NOS 50
#define TOLG 1e-9
#define TOLG2 8e-11
#define MAX_ERRO_NR 1e-9
//#define DEBUG
#define MAX_STEPS 4

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],type[MAX_NOME];
  double valor,ini, param1, param2, param3, param4, param5, param6, param7,g1,g2,g3,i1,i2,i3;
  int a,b,c,d,x,y;

} elemento;

elemento netlist[MAX_ELEM]; /* Netlist */

int
  debugme=0,
  tryAgain=1,
  RaphsonCount=0,
  repete=1,
  intStepsCounter=0,
  goToNewton,
  useInicialConditions = 2,
  quant,
  erroElevado=0,
  intSteps,
  order,
  save = 3, /* variavel para armazenamento  */
  icc = 0, /* Correntes capacitor */
  ne, /* Elementos */
  nv, /* Variaveis */
  nn, /* Nos */
  i,j,k;

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  nomearquivo[MAX_LINHA+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1],
  method[6],
  uic[10],
  *p;
FILE *arquivo;

double
  timeA=0,
  iStepSize= TOLG2,
  finalTime,
  stepSize,
  g,
  z,
  pulseOffTime,
  pulseRealTime,
  Yc[MAX_STEPS+2][MAX_NOS+1], /* Essa matriz ira armazenar os valores das correntes nos capacitores */
  Ys[MAX_STEPS+2][MAX_NOS+1], /* Essa matriz irá armazenar até no máximo 3 valores passados da analise */
  Ynr[MAX_NOS+1][MAX_NOS+2],
  NRCompare[MAX_NOS+1],
  nrErro[MAX_NOS+1],
  Yn[MAX_NOS+1][MAX_NOS+2];

/* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal */
int resolversistema(void) {
  int i,j,l, a;
  double t, p;

  for (i=1; i<=nv; i++) {
    t=0.0;
    a=i;
    for (l=i; l<=nv; l++) {
      if (fabs(Yn[l][i])>fabs(t)) {
	    a=l;
	    t=Yn[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=nv+1; l++) {
	    p=Yn[i][l];
	    Yn[i][l]=Yn[a][l];
	    Yn[a][l]=p;
      }
    }
    if (fabs(t)<TOLG) {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=nv+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j]/= t;
      p=Yn[i][j];
      if (p!=0)  /* Evita operacoes com zero */
        for (l=1; l<=nv; l++) {
	      if (l!=i)
	        Yn[l][j]-=Yn[l][i]*p;
        }
    }
  }
  Yn[0][nv+1] = 0;
  return 0;
}

/* Rotina que conta os nos e atribui numeros a eles */
int numero(char *nome)
{
  int i,achou;

  i=0; achou=0;
  while (!achou && i<=nv)
    if (!(achou=!strcmp(nome,lista[i]))) i++;
  if (!achou) {
    if (nv==MAX_NOS) {
      printf("O programa so aceita ate %d nos\n",nv);
      exit(1);
    }
    nv++;
    strcpy(lista[nv],nome);
    return nv; /* novo no */
  }
  else {
    return i; /* no ja conhecido */
  }
}

void zeraSistema (void) {
  for (i=0; i<=nv; i++)
    for (j=0; j<=nv+1; j++)
      Yn[i][j]=0;
}
void Randomic (void) {
	srand(time(NULL));
	for (i=1; i<=nv; i++){
	  NRCompare[i] = (double)(((rand()%1001)/10)-50);
	}
}

int NewtonRapsonCompare (void) {
  erroElevado=0;
  for (i=1; i<=nv; i++){
    if ( (fabs(Yn[i][nv+1] - NRCompare[i])) > MAX_ERRO_NR ) {
      erroElevado=1;
      }
  }
     if (erroElevado==1) return 0; /*Girando mundão que não tá legal */
     else return 1; /*Tá traquilo tá favorável*/

}
void AdamsMoltonL (int i, int save) { /* ADMO do Indutor Completo! */
    if (save == 3){ /* Analise inicial */
        z=(netlist[i].ini*(netlist[i].valor/iStepSize));
        g=netlist[i].valor/iStepSize;
    }
    else if (order == 1){ /* PERFEITO */
        z=((netlist[i].valor/stepSize)*(Ys[save+1][netlist[i].x]));
        g=(netlist[i].valor/stepSize);
    }
    else if (order == 2){/* PERFEITO */
        z=((((2.0*netlist[i].valor)/stepSize)*(Ys[save+1][netlist[i].x]))
           +(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b]));
        g=((2.0*netlist[i].valor)/stepSize);
     //   printf("z: %g g: %g\n ",z,g);
     //   getch();
    }
    else if (order == 3){ /* DefEITO */
        z=( ((12.0/5.0)*((Ys[save+1][netlist[i].x])*((netlist[i].valor)/(stepSize))))
           +((8.0/5.0)*(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b]))
           -((1.0/5.0)*(Ys[save+2][netlist[i].a]-Ys[save+2][netlist[i].b])));
        g=((12.0/5.0)*((netlist[i].valor)/stepSize));
      //  printf("z: %g g: %g",z,g);
      //  getch();
    }
    else if (order == 4){ /* Perfeito */
        z=((((Ys[save+1][netlist[i].x])*((netlist[i].valor)/stepSize))*(24.0/9.0))
           +((19.0/9.0)*(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b]))
           -((5.0/9.0)*(Ys[save+2][netlist[i].a]-Ys[save+2][netlist[i].b]))
           +((1.0/9.0)*(Ys[save+3][netlist[i].a]-Ys[save+3][netlist[i].b])));
        g=((24.0/9.0)*((netlist[i].valor)/stepSize));
     //   printf("z: %g g: %g\n ",z,g);
     //   getch();
    }
}

/* Copia o valor da solucao de Yn para o vetor NRcompare, para Newton-raphson */
void SolutionRaphson (void) {
  int i;
  for (i=0; i<=nv; i++)
  	NRCompare[i] = Yn[i][nv+1];
}

void AdamsMoltonC (int i, int save){

    if (save == 3){
        z=(netlist[i].ini*(netlist[i].valor/iStepSize)); /* iStpeSize - StepSize inicial para começar a analise */
        g=((netlist[i].valor)/(iStepSize));

    }
    else if (order == 1){ /* PERFEITO */
        z=((Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])*(netlist[i].valor/stepSize));
        g=((netlist[i].valor)/(stepSize));
    }
    else if (order == 2){ /* PERFEITO */
        z=(((Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])*((2.0*netlist[i].valor)/stepSize))
           +Yc[save+1][netlist[i].x]);
        g=(2.0*(netlist[i].valor)/(stepSize));
    //    printf("z :%g g: %g\n",z,g);
    //    getch();
    }
    else if (order == 3){ /* PERFEITO */
        z=(((Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])*(((12.0/5.0)*netlist[i].valor)/stepSize))
           +((8.0/5.0)*Yc[save+1][netlist[i].x])
           -((1.0/5.0)*Yc[save+2][netlist[i].x]));
        g=((12.0/5.0)*((netlist[i].valor)/(stepSize)));
//        printf("save0: %g save2: %g save3: %g\n",(Yc[save+1][netlist[i].x]),Yc[save+2][netlist[i].x],Yc[save+3][netlist[i].x]);
//        getch();
    }
    else if (order == 4){ /* Perfeito */
        z=( ((Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])*((24.0/9.0)*((netlist[i].valor)/(stepSize))))
           +((19.0/9.0)*Yc[save+1][netlist[i].x])
           -((5.0/9.0)*Yc[save+2][netlist[i].x])
           +((1.0/9.0)*Yc[save+3][netlist[i].x]));
        g=((24.0/9.0)*((netlist[i].valor)/(stepSize)));
 //       printf("save0: %g save2: %g save3: %g\n",(Yc[save+1][netlist[i].x]),Yc[save+2][netlist[i].x],Yc[save+3][netlist[i].x]);
 //       getch();
    }
}

void CorrenteC (int save){

     for(i=1 ; i<=ne ; i++){ /* rotina que salva as correntes do capacitor na matriz Yc */
        tipo=netlist[i].nome[0];
        if (tipo=='C'){
          if(save==3){
            Yc[save][netlist[i].x]=(((netlist[i].valor)/iStepSize)*((Yn[netlist[i].a][nv+1]-Yn[netlist[i].b][nv+1])-(netlist[i].ini)));
          }
          else if (order == 1){
            Yc[save][netlist[i].x]=(((netlist[i].valor)/stepSize)*((Yn[netlist[i].a][nv+1]-Yn[netlist[i].b][nv+1])
                                                                   -(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])));
          }
          else if (order == 2){
            Yc[save][netlist[i].x]=( 2.0*((netlist[i].valor)/stepSize)*((Yn[netlist[i].a][nv+1]-Yn[netlist[i].b][nv+1])
                                                                        -(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b]))
                                                                        -Yc[save+1][netlist[i].x]);
          }
          else if (order == 3){
            Yc[save][netlist[i].x]=(  ( ((12.0/5.0)*((netlist[i].valor)/stepSize) )*((Yn[netlist[i].a][nv+1]-Yn[netlist[i].b][nv+1])
                                                                                     -(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])))
                                                                                     -((8.0/5.0)*Yc[save+1][netlist[i].x])
                                                                                     +((1.0/5.0)*Yc[save+2][netlist[i].x]));
          }
          else if (order == 4){ /* Pode ter merda aqui também. */
            Yc[save][netlist[i].x]=( ( ((24.0/9.0)*((netlist[i].valor)/stepSize) )*((Yn[netlist[i].a][nv+1]-Yn[netlist[i].b][nv+1])
                                                                                    -(Ys[save+1][netlist[i].a]-Ys[save+1][netlist[i].b])))
                                                                                    -((19.0/9.0)*Yc[save+1][netlist[i].x])
                                                                                    +((5.0/9.0)*Yc[save+2][netlist[i].x])
                                                                                    -((1.0/9.0)*Yc[save+3][netlist[i].x]));
       //     printf("%g\n",Yc[save][netlist[i].x]);
       //     getch();
          }
        }
      }
}

void montaEstampas (void){
  zeraSistema();
  /* Monta estampas */
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if(tipo=='L'){
        AdamsMoltonL(i,save);
        Yn[netlist[i].a][netlist[i].x]+=1;
        Yn[netlist[i].b][netlist[i].x]-=1;
        Yn[netlist[i].x][netlist[i].a]-=1;
        Yn[netlist[i].x][netlist[i].b]+=1;
        Yn[netlist[i].x][netlist[i].x]+=g;
        Yn[netlist[i].x][nv+1]+=z;          /* Fonte de corrente sendo Adicionada */
    }
    else if(tipo=='C'){
      AdamsMoltonC(i,save);
      Yn[netlist[i].a][netlist[i].a]+=g;
      Yn[netlist[i].b][netlist[i].b]+=g;
      Yn[netlist[i].a][netlist[i].b]-=g;
      Yn[netlist[i].b][netlist[i].a]-=g;
      Yn[netlist[i].a][nv+1]+=z; /* DETALHE PARA O SINAL Fonte de corrente sendo Adicionada */
      Yn[netlist[i].b][nv+1]-=z; /* DETALHE PARA O SINAL Fonte de corrente sendo Adicionada */
     }
    else if (tipo=='R') {
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


      if ((strcmp(netlist[i].type,"DC"))==0){
        Yn[netlist[i].x][nv+1]-=netlist[i].valor;
      }

      else if ((strcmp(netlist[i].type,"SIN"))==0){

            if (timeA<netlist[i].param3){
                Yn[netlist[i].x][nv+1]-= (  netlist[i].valor +  netlist[i].param1 *
                        sin((M_PI/180.0)*netlist[i].param5));
            }

            else if (timeA > (netlist[i].param3 + (netlist[i].param6)*(1/netlist[i].param2)) ) {
                Yn[netlist[i].x][nv+1]-= (  netlist[i].valor +  netlist[i].param1 * exp(-netlist[i].param4*(netlist[i].param6/netlist[i].param2)) *
                        sin( ((2.0* M_PI * netlist[i].param2) * (netlist[i].param6/netlist[i].param2)) + (M_PI/180.0)*netlist[i].param5 ));

            }

            else{
                Yn[netlist[i].x][nv+1]-= (  netlist[i].valor +  netlist[i].param1 * exp(-netlist[i].param4*(timeA-netlist[i].param3)) *
                        sin( ((2.0* M_PI * netlist[i].param2) * (timeA-netlist[i].param3)) + (M_PI/180.0)*netlist[i].param5 ));
            }}

      else if ((strcmp(netlist[i].type,"PULSE"))==0){

        pulseRealTime=timeA-netlist[i].param2;
        pulseRealTime= fmod(pulseRealTime,netlist[i].param6);
        pulseOffTime=netlist[i].param6- (netlist[i].param3+netlist[i].param4+netlist[i].param5);
        if (timeA<netlist[i].param2){
            Yn[netlist[i].x][nv+1]-=netlist[i].valor;
        }

        else if (timeA> (netlist[i].param2 +(netlist[i].param7*netlist[i].param6) ) ) {
            Yn[netlist[i].x][nv+1]-=netlist[i].valor;
        }
            else {
                        if (pulseRealTime<netlist[i].param3 ){ /* subindo*/
                            Yn[netlist[i].x][nv+1]-=((((netlist[i].param1-netlist[i].valor)/netlist[i].param3)*pulseRealTime)+ netlist[i].valor);
                        }
                        else if (pulseRealTime< (netlist[i].param3+netlist[i].param5)){
                            Yn[netlist[i].x][nv+1]-=netlist[i].param1;
                        }
                        else if (pulseRealTime< (netlist[i].param3+netlist[i].param5+netlist[i].param4)){
                            Yn[netlist[i].x][nv+1]-= (netlist[i].param1-
                                                     (((netlist[i].param1-netlist[i].valor)/netlist[i].param4)*
                                                     (pulseRealTime-(netlist[i].param3+netlist[i].param5))));
                            }
                        else {
                            Yn[netlist[i].x][nv+1]-=netlist[i].valor;
                        }

            }
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
    else if (tipo=='N') {
        //       printf("tensao em cima dele %g \n", NRCompare[netlist[i].a]-NRCompare[netlist[i].b]);
            if ( (NRCompare[netlist[i].a] - NRCompare[netlist[i].b]) < netlist[i].param3 ){
               g=((netlist[i].param4 - netlist[i].param2) / (netlist[i].param3-netlist[i].param1));
               z=(netlist[i].param4 - g*netlist[i].param3);
            }
                //((NRCompare[netlist[i].a] - NRCompare[netlist[i].b]) >= netlist[i].param3) &&
            else if ( ((NRCompare[netlist[i].a]-NRCompare[netlist[i].b]) < netlist[i].param5) ){

                g=(netlist[i].param6 - netlist[i].param4) / (netlist[i].param5 - netlist[i].param3);
                z=(netlist[i].param6 - g*netlist[i].param5);
            }
            //((NRCompare[netlist[i].a]-NRCompare[netlist[i].b]) >= netlist[i].param5)
            else {
                g=(netlist[i].valor - netlist[i].param6) / (netlist[i].param7 - netlist[i].param5);
                z=(netlist[i].valor - g*netlist[i].param7);
            }
            Yn[netlist[i].a][netlist[i].a]+=g;
            Yn[netlist[i].b][netlist[i].b]+=g;
            Yn[netlist[i].a][netlist[i].b]-=g;
            Yn[netlist[i].b][netlist[i].a]-=g;
            Yn[netlist[i].a][nv+1]-=z;
            Yn[netlist[i].b][nv+1]+=z;
           // getch();
    }
    else if (tipo=='$') {
            if ((NRCompare[netlist[i].c]-NRCompare[netlist[i].d])<netlist[i].param3){ //param3 = vref
              Yn[netlist[i].a][netlist[i].a]+=netlist[i].param2;//param2 = goff
              Yn[netlist[i].b][netlist[i].b]+=netlist[i].param2;
              Yn[netlist[i].a][netlist[i].b]-=netlist[i].param2;
              Yn[netlist[i].b][netlist[i].a]-=netlist[i].param2;
            }
            else {
              Yn[netlist[i].a][netlist[i].a]+=netlist[i].param1;//param1=gon
              Yn[netlist[i].b][netlist[i].b]+=netlist[i].param1;
              Yn[netlist[i].a][netlist[i].b]-=netlist[i].param1;
              Yn[netlist[i].b][netlist[i].a]-=netlist[i].param1;
            }
    }
    else if (tipo=='O') {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].c]+=1;
      Yn[netlist[i].x][netlist[i].d]-=1;
    }
                                                                     #ifdef DEBUG
                                                                     if (debugme==1){
                                                                        /* Opcional: Mostra o sistema apos a montagem da estampa */
                                                                        printf("Sistema apos a estampa de %s\n",netlist[i].nome);
                                                                        for (k=1; k<=nv; k++) {
                                                                          for (j=1; j<=nv+1; j++)
                                                                            if (Yn[k][j]!=0) printf("%+3.1f ",Yn[k][j]);
                                                                            else printf(" ... ");
                                                                          printf("\n");
                                                                        }
                                                                        getch();
                                                                        }
                                                                    #endif

    }
}

int main(void)
{
  system("cls");
  srand( (unsigned)time(NULL) );
  printf("Programa de analise no tempo, pelo metodo de Adams-Molton\n");
  printf("UFRJ- Departamento de Engenharia Eletronica\n");
  printf("Desenvolvido por : Igor F. Nascimento, Eduardo Naslausky e Alan Carpilovsky\n");
  printf("Versao %d .j\n",rand());
  //printf("Versao %s\n",versao);
 denovo:
  /* Leitura do netlist */
  ne=0; nv=0; strcpy(lista[0],"0");
  printf("Nome do arquivo com o netlist (ex: mna.net): ");
  scanf("%50s",nomearquivo);
  arquivo=fopen(nomearquivo,"r");
  if (arquivo==0) {
    printf("Arquivo %s inexistente\n",nomearquivo);
    goto denovo;
  }
  printf("Lendo netlist:\n");
  fgets(txt,MAX_LINHA,arquivo);
  printf("Titulo: %s",txt);
  while (fgets(txt,MAX_LINHA,arquivo)) {
    ne++; /* Nao usa o netlist[0] */
    if (ne>MAX_ELEM) {
      printf("O programa so aceita ate %d elementos\n",MAX_ELEM);
      exit(1);
    }
    txt[0]=toupper(txt[0]);
    tipo=txt[0];
    sscanf(txt,"%10s",netlist[ne].nome);
    p=txt+strlen(netlist[ne].nome); /* Inicio dos parametros */
    /* O que e lido depende do tipo */
    /* Le e determina o tipo da analise */
    if (tipo=='.'){
      if ((quant=sscanf(p,"%lg%lg%6s%i%4s",&finalTime,&stepSize,method,&intSteps,uic))!=5){
        useInicialConditions = 1; /* 1 = Não usar Condições Iniciais ; 2 = Usar Condições Iniciais */
      }
    printf("%lg %lg %s %i %s %i\n",finalTime,stepSize,method,intSteps,uic,quant);/* Debug - Igor */
    order=atoi(method+4); /* Tem que ser o 4 pq o 5 é o endOfString ADMO"N"  */
    ne--;
    stepSize=stepSize/intSteps; //Diminuir o passo com base no numero de passos internos
    }
    else if (tipo=='$'){
      sscanf(p,"%10s%10s%10s%10s%lg%lg%lg",na,nb,nc,nd,
                                            &netlist[ne].param1,
                                            &netlist[ne].param2,
                                            &netlist[ne].param3);
        printf("%s %s %s cntrl1:%s cntrl2:%s \n",netlist[ne].nome,na,nb,nc,nd);
        netlist[ne].a=numero(na);
        netlist[ne].b=numero(nb);
        netlist[ne].c=numero(nc);
        netlist[ne].d=numero(nd);

    }
    else if (tipo=='N'){
      sscanf(p,"%10s%10s%lg%lg%lg%lg%lg%lg%lg%lg",na,nb,&netlist[ne].param1,
                                            &netlist[ne].param2,
                                            &netlist[ne].param3,
                                            &netlist[ne].param4,
                                            &netlist[ne].param5,
                                            &netlist[ne].param6,
                                            &netlist[ne].param7,
                                            &netlist[ne].valor);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      printf("%s %s %s P1<%g,%g> P2<%g,%g> P3<%g,%g> P4<%g,%g>\n ",netlist[ne].nome,na,nb,netlist[ne].param1,netlist[ne].param2,netlist[ne].param3,netlist[ne].param4,netlist[ne].param5,netlist[ne].param6,netlist[ne].param7,netlist[ne].valor);
    //  printf("i1  %g  g1 %g i2 %g g2 %g i3 %g g3 %g \n", netlist[ne].i1, netlist[ne].g1, netlist[ne].i2, netlist[ne].g2, netlist[ne].i3, netlist[ne].g3);
    } /*
     Conferir depois se tá lendo certinho tb*/


    else if (tipo=='L' || tipo=='C'){
      if ((quant=sscanf(p,"%10s%10s%lg IC=%lg" ,na,nb,&netlist[ne].valor, &netlist[ne].ini))!=4){
        netlist[ne].ini=0; /* caso UIC não seja especificada */
      };
      printf("%s %s %s %g %g quant =%i\n",netlist[ne].nome,na,nb,netlist[ne].valor,netlist[ne].ini,quant);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
    }
    else if (tipo=='V') {
      sscanf(p,"%10s%10s%10s%lg%lg%lg%lg%lg%lg%lg%lg",na,nb,netlist[ne].type,
                                            &netlist[ne].valor, /* Amplitude 1*/
                                            &netlist[ne].param1,/* Amplitude2*/
                                            &netlist[ne].param2, /* Atraso*/
                                            &netlist[ne].param3, /* Tempo de subida*/
                                            &netlist[ne].param4, /* Tempo de descida*/
                                            &netlist[ne].param5, /* Tempo ligada*/
                                            &netlist[ne].param6, /* Periodo */
                                            &netlist[ne].param7);/* n de ciclos*/
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
   // printf("%s %g %g %g %g %g %g %g \n", netlist[ne].type, netlist[ne].param1,netlist[ne].param2,netlist[ne].param3,netlist[ne].param4,netlist[ne].param5,netlist[ne].param6,netlist[ne].param7);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
    }
    else if (tipo=='R' || tipo=='I') {
      sscanf(p,"%10s%10s%lg",na,nb,&netlist[ne].valor);
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
    }
    else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
      sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[ne].valor);
      printf("%s %s %s %s %s %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='O') {
      sscanf(p,"%10s%10s%10s%10s",na,nb,nc,nd);
      printf("%s %s %s %s %s\n",netlist[ne].nome,na,nb,nc,nd);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='*') { /* Comentario comeca com "*" */
      printf("Comentario: %s",txt);
      ne--;
    }
    else {
      printf("Elemento desconhecido: %s\n",txt);
      getch();
      exit(1);
    }
  }
  fclose(arquivo);


  /* Acrescenta variaveis de corrente acima dos nos, anotando no netlist */
  nn=nv;
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O' || tipo=='L') {
      nv++;
      if (nv>MAX_NOS) {
        printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
        exit(1);
      }
      strcpy(lista[nv],"j"); /* Tem espaco para mais dois caracteres */
      strcat(lista[nv],netlist[i].nome);
      netlist[i].x=nv;
    }
    else if (tipo=='C'){ /* Salva no .x onde estarão salvas as correntes dos capacitores. */
      icc++;
      netlist[i].x=icc;
    }
    else if (tipo=='H') {
      nv=nv+2;
      if (nv>MAX_NOS) {
        printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
        exit(1);
      }
      strcpy(lista[nv-1],"jx"); strcat(lista[nv-1],netlist[i].nome);
      netlist[i].x=nv-1;
      strcpy(lista[nv],"jy"); strcat(lista[nv],netlist[i].nome);
      netlist[i].y=nv;
    }
  }
  getch();


  /* Lista tudo */
  printf("Variaveis internas: \n");
  for (i=0; i<=nv; i++)
    printf("%d -> %s\n",i,lista[i]);
  getch();
  printf("Netlist interno final\n");
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R' || tipo=='I' || tipo=='V') {
      printf("%s %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor);
    }
    else if (tipo=='C' || tipo=='L'){
      printf("%s %d %d %g IC = %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor,netlist[i].ini);
    }
    else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
      printf("%s %d %d %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor);
    }
    else if (tipo=='O') {
      printf("%s %d %d %d %d\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d);
    }
    else if (tipo=='N') {
      printf("%s %d %d %g %g %g %g %g %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].i1,netlist[i].g1,netlist[i].i2,netlist[i].g2,netlist[i].i3,netlist[i].g3);
    }
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O'){
      printf("Corrente jx: %d\n",netlist[i].x);
      }
    else if (tipo=='L'){
      printf("Corrente %s: %d\n",lista[netlist[i].x],netlist[i].x);
    }
    else if (tipo=='H'){
      printf("Correntes %s: %d e %s: %d\n",lista[netlist[i].x],netlist[i].x,lista[netlist[i].y],netlist[i].y);
    }
  }
  getch();
  /* Monta o sistema nodal modificado */

  printf("O circuito tem %d nos, %d variaveis e %d elementos\n",nn,nv,ne);
  getch();

  arquivo=fopen("output.tab", "w");

  fprintf (arquivo, "t ");

  for (i=1;i<=nv; i++){
    fprintf(arquivo, "%s ", lista[i]);
  }
  fprintf (arquivo, "\n");
  fclose(arquivo);



  while(timeA<finalTime){ /* While para analise no tempo.*/

  zeraSistema();
  RaphsonCount = 0;
  tryAgain=0;

  do {
    SolutionRaphson();
            if (RaphsonCount > 10){
              Randomic();
              RaphsonCount=0;
              tryAgain++;
              if (tryAgain == 5){
                printf("Sistema sem solucao");
                exit(1);
              }
            }
    montaEstampas(); /* Usa B.E para Cond. Iniciais */
    resolversistema();
    RaphsonCount++;
  } while (NewtonRapsonCompare() == 0);

                                                                    #ifdef DEBUG
                                                                      /* Opcional: Mostra o sistema resolvido */
                                                                      printf("Sistema resolvido:\n");
                                                                      for (i=1; i<=nv; i++) {
                                                                          for (j=1; j<=nv+1; j++)
                                                                            if (Yn[i][j]!=0) printf("%+3.1f ",Yn[i][j]);
                                                                            else printf(" ... ");
                                                                          printf("\n");
                                                                        }
                                                                      getch();
                                                                    #endif

/* Zera a matriz de saves */
  if(save==3){
    for (i=0; i<=5; i++) { /* deixei essa rotina por precaução, para não ter valores indefinidos na matriz de saves */
      for (j=0; j<=nv+1; j++){ /* zera também a matriz que salva as correntes do capacitor. */
        Ys[i][j]=0;
        Yc[i][j]=0;
      }
    }
  }
  for (i=1; i<=nv; i++){
    Ys[save][i]=Yn[i][nv+1]; /* Aproveitei que ele já estava listando tudo e copio o valor para a matriz Ys */
  }
    CorrenteC(save);/* Calcula as correntes nos capacitores e salva na matriz Yc */

  /* Mostra solucao e salva tensões na matriz de saves */
  #ifdef DEBUG
  printf("Solucao:\n");
  #endif // DEBUG

  /* Rotina Para extender as condições iniciais para t(-1) e t(-2) Ys e Yc para para o caso INICIAL ; PODE JUNTAR ESSA ROTINA NO FOR ACIMA*/
    if (save==3){
      for (i=1; i<=nv; i++) {
        Ys[4][i]=Ys[3][i]; /* Copia os valores iniciais para os slots extras no tempo */
        Yc[4][i]=Yc[3][i]; /* Copia os valores das correntes dos cap. iniciais para os slots extras no tempo */
        Ys[5][i]=Ys[3][i]; /* Copia os valores iniciais para os slots extras no tempo */
        Yc[5][i]=Yc[3][i]; /* Copia os valores das correntes dos cap. iniciais para os slots extras no tempo */
      }
    }
    if (save == 0){ /* rotina que da shift nos valores de Yc e Ys; PODE JUNTAR ESSA ROTINA EM UM FOR ACIMA !*/
      for (i=1; i<=nv;i++){
        Ys[3][i]=Ys[2][i];
        Yc[3][i]=Yc[2][i];
        Ys[2][i]=Ys[1][i];
        Yc[2][i]=Yc[1][i];
        Ys[1][i]=Ys[0][i];
        Yc[1][i]=Yc[0][i];
      }
      save=1;

    }
  strcpy(txt,"Tensao");
  if (intStepsCounter%intSteps==0){
                  arquivo=fopen ("output.tab","a");
                  fprintf(arquivo, "%g ",timeA);
                  for (i=1; i<=nv; i++){
                    if (i==nn+1){
                      strcpy(txt,"Corrente");
                    }

                    #ifdef DEBUG
                    printf("%s %s: %g\n",txt,lista[i],Yn[i][nv+1]);
                    #endif // DEBUG
                    fprintf(arquivo, "%g ",Yn[i][nv+1]);
                  }
                  fprintf (arquivo, "\n");
                  fclose (arquivo);
  }
 //  printf("Saves:\n");
 //   for (i=0; i<=6; i++) { /* deixei essa rotina por precaução, para não ter valores indefinidos na matriz de saves */
 //     printf("\n saveC%i",i);
 //     for (j=0; j<=nv; j++){
 //       if ( tipo == 'C' ){
 //         printf(" %s %g ",lista[j],Ys[i][j]); /* printa a matriz de saves - debug */
 //       //  printf(" %i eh: %g ",i,Yc[i][j]); /* printa a matriz de saves - debug */
 //       }
 //     }
 //   }
 //   getch();
timeA+=stepSize;
intStepsCounter++;
save--;
}
  printf("Analise concluida: Pressione qualquer tecla para continuar \n");
  getch();
  return 0;
}
