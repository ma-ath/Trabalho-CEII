/* Monta estampas */
#include "funcoesAuxiliares.h"
#include "global.h"
//#define DEBUG
using namespace std;

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
  else if (tipo=='I')
  {
    if (strcmp(netlist[i].fonte, "DC") == 0)
    /*
    Fonte DC - Nao muda com o tempoAtual
    */
    {
        g=netlist[i].valor;
        Yn[netlist[i].a][nv+1]-=g;
        Yn[netlist[i].b][nv+1]+=g;
    }
    else
    if (strcmp(netlist[i].fonte, "SIN") == 0)
    /*
    Fonte SIN - Muda de acordo com:
    off+Vp*exp(-dec*(tempoAtual-atraso))*sin(2*pi*f*(tempoAtual-atraso)+(pi/180)*fase)
    */
    {
      g =
        (
          netlist[i].valor +
          (
            netlist[i].amplitude*exp(-1*netlist[i].amortecimento*(tempoAtual-netlist[i].atraso)) *
            sin(2*PI*netlist[i].freq*(tempoAtual-netlist[i].atraso) + (PI/180)*netlist[i].defasagem)
          )*( heaviside(tempoAtual-netlist[i].atraso) -
              heaviside((tempoAtual-netlist[i].atraso) - (2*PI/netlist[i].freq)*netlist[i].ciclo) )
        );

        Yn[netlist[i].a][nv+1]-=g;
        Yn[netlist[i].b][nv+1]+=g;

    }
    if (strcmp(netlist[i].fonte, "PULSE") == 0){
    /*
    Fonte PULSE - Muda de acordo com:
    add expressao aqui
    */
    //A fonte de tensão é sempre implementado pela estampa:
    //Yn[netlist[i].a][nv+1]-=g;
    //Yn[netlist[i].b][nv+1]+=g;
    // onde g é o valor da fonte no tempo atual

      pulseRealTime=tempoAtual-netlist[i].atraso;
      pulseRealTime= fmod(pulseRealTime,netlist[i].periodo);
      pulseOffTime=netlist[i].periodo- (netlist[i].tempoSubida+netlist[i].tempoDescida+netlist[i].tempoLigada);
      if (tempoAtual<netlist[i].atraso){

          Yn[netlist[i].a][nv+1]-=netlist[i].valor;
          Yn[netlist[i].b][nv+1]+=netlist[i].valor;
      }

      else if (tempoAtual> (netlist[i].atraso +(netlist[i].ciclo*netlist[i].periodo) ) ) {

        Yn[netlist[i].a][nv+1]-=netlist[i].valor;
        Yn[netlist[i].b][nv+1]+=netlist[i].valor;

      }

      else {
        if (pulseRealTime<netlist[i].tempoSubida ){ /* subindo*/
          Yn[netlist[i].a][nv+1]-=((((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].valor);
          Yn[netlist[i].b][nv+1]+=((((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].valor);
        }
        else if (pulseRealTime< (netlist[i].tempoSubida+netlist[i].tempoLigada)){
          Yn[netlist[i].a][nv+1]-=netlist[i].amplitude;
          Yn[netlist[i].b][nv+1]+=netlist[i].amplitude;
        }
        else if (pulseRealTime< (netlist[i].tempoSubida+netlist[i].tempoLigada+netlist[i].tempoDescida)){
          Yn[netlist[i].a][nv+1]-= (netlist[i].amplitude-
            (((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoDescida)*
            (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));

          Yn[netlist[i].b][nv+1]+= (netlist[i].amplitude-
              (((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoDescida)*
              (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));
        }
        else {
          Yn[netlist[i].a][nv+1]-=netlist[i].valor;
          Yn[netlist[i].b][nv+1]+=netlist[i].valor;
        }

      }
    }
  }
  else if (tipo=='V')   /*A estampa de V deve mudar de acordo com o tempoAtual*/
  {
    if (strcmp(netlist[i].fonte, "DC") == 0)
    /*
    Fonte DC - Nao muda com o tempoAtual
    */
    {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=netlist[i].valor;
    }
    else
    if (strcmp(netlist[i].fonte, "SIN") == 0)
    /*
    Fonte SIN - Muda de acordo com:
    off+Vp*exp(-dec*(tempoAtual-atraso))*sin(2*pi*f*(tempoAtual-atraso)+(pi/180)*fase)
    */
    {
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;
      Yn[netlist[i].x][nv+1]-=
        (
          netlist[i].valor +
          (
            netlist[i].amplitude*exp(-1*netlist[i].amortecimento*(tempoAtual-netlist[i].atraso)) *
            sin(2*PI*netlist[i].freq*(tempoAtual-netlist[i].atraso) + (PI/180)*netlist[i].defasagem)
          )*( heaviside(tempoAtual-netlist[i].atraso) -
              heaviside((tempoAtual-netlist[i].atraso) - (2*PI/netlist[i].freq)*netlist[i].ciclo) )
        );
    }
    if (strcmp(netlist[i].fonte, "PULSE") == 0){
    /*
    Fonte PULSE - Muda de acordo com:
    add expressao aqui
    */
    //Yn[netlist[i].x][nv+1]-=netlist[i].valor;
      Yn[netlist[i].a][netlist[i].x]+=1;
      Yn[netlist[i].b][netlist[i].x]-=1;
      Yn[netlist[i].x][netlist[i].a]-=1;
      Yn[netlist[i].x][netlist[i].b]+=1;

      pulseRealTime=tempoAtual-netlist[i].atraso;
      pulseRealTime= fmod(pulseRealTime,netlist[i].periodo);
      pulseOffTime=netlist[i].periodo- (netlist[i].tempoSubida+netlist[i].tempoDescida+netlist[i].tempoLigada);
      if (tempoAtual<netlist[i].atraso){
          Yn[netlist[i].x][nv+1]-=netlist[i].valor;
      }

      else if (tempoAtual> (netlist[i].atraso +(netlist[i].ciclo*netlist[i].periodo) ) ) {
        Yn[netlist[i].x][nv+1]-=netlist[i].valor;
      }

      else {
        if (pulseRealTime<netlist[i].tempoSubida ){ /* subindo*/
          Yn[netlist[i].x][nv+1]-=((((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoSubida)*pulseRealTime)+ netlist[i].valor);
        }
        else if (pulseRealTime< (netlist[i].tempoSubida+netlist[i].tempoLigada)){
          Yn[netlist[i].x][nv+1]-=netlist[i].amplitude;
        }
        else if (pulseRealTime< (netlist[i].tempoSubida+netlist[i].tempoLigada+netlist[i].tempoDescida)){
          Yn[netlist[i].x][nv+1]-= (netlist[i].amplitude-
            (((netlist[i].amplitude-netlist[i].valor)/netlist[i].tempoDescida)*
            (pulseRealTime-(netlist[i].tempoSubida+netlist[i].tempoLigada))));
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
  else if (tipo=='O') {
    Yn[netlist[i].a][netlist[i].x]+=1;
    Yn[netlist[i].b][netlist[i].x]-=1;
    Yn[netlist[i].x][netlist[i].c]+=1;
    Yn[netlist[i].x][netlist[i].d]-=1;
  }
  else if (tipo=='K') {     /*Transformador Ideal*/
    g=netlist[i].valor;
    Yn[netlist[i].a][netlist[i].x]-=g;
    Yn[netlist[i].b][netlist[i].x]+=g;
    Yn[netlist[i].c][netlist[i].x]+=1;
    Yn[netlist[i].d][netlist[i].x]-=1;

    Yn[netlist[i].x][netlist[i].a]+=g;
    Yn[netlist[i].x][netlist[i].b]-=g;
    Yn[netlist[i].x][netlist[i].c]-=1;
    Yn[netlist[i].x][netlist[i].d]+=1;
  }
  //
  //      Estampas variantes no temppo para Capacitor e Indutor - Metodo dos Trapezios
  //      Pagina 104 do livro
  else if (tipo=='C') {
    //se esta no modo de analise de ponto de op,
    //substitui o capacitor por circuito aberto (ou resistor mt grande)
    if (analisandoPontodeOp ==1){
      Yn[netlist[i].a][netlist[i].a]+=GCapacitorAberto;
      Yn[netlist[i].b][netlist[i].b]+=GCapacitorAberto;
      Yn[netlist[i].a][netlist[i].b]-=GCapacitorAberto;
      Yn[netlist[i].b][netlist[i].a]-=GCapacitorAberto;
    }
    else{
      g=((2*netlist[i].valor)/(passo));     //Valor da condutancia para o MetodoDosTrapezios
      Yn[netlist[i].a][netlist[i].a]+=g;    //Estampas para o resistor
      Yn[netlist[i].b][netlist[i].b]+=g;
      Yn[netlist[i].a][netlist[i].b]-=g;
      Yn[netlist[i].b][netlist[i].a]-=g;

      Yn[netlist[i].a][nv+1]+=(g*netlist[i].vt0+netlist[i].jt0);           //Estampas para a fonte de corrente
      Yn[netlist[i].b][nv+1]-=(g*netlist[i].vt0+netlist[i].jt0);
    }
  }
  else if (tipo=='L') {
    if (  analisandoPontodeOp ==1){
      Yn[netlist[i].a][netlist[i].a]+=GIndutorCurto;
      Yn[netlist[i].b][netlist[i].b]+=GIndutorCurto;
      Yn[netlist[i].a][netlist[i].b]-=GIndutorCurto;
      Yn[netlist[i].b][netlist[i].a]-=GIndutorCurto;
    }
    else{
      g=((passo)/(2*netlist[i].valor));     //Valor da condutancia para o MetodoDosTrapezios
      Yn[netlist[i].a][netlist[i].a]+=g;    //Estampas para o resistor
      Yn[netlist[i].b][netlist[i].b]+=g;
      Yn[netlist[i].a][netlist[i].b]-=g;
      Yn[netlist[i].b][netlist[i].a]-=g;

      Yn[netlist[i].a][nv+1]-=(g*netlist[i].vt0+netlist[i].jt0);           //Estampas para a fonte de corrente
      Yn[netlist[i].b][nv+1]+=(g*netlist[i].vt0+netlist[i].jt0);
    }
  }
  else if (tipo=='$') {     /*Chave*/

    //EXPERIMENTAL
    if (netlist[i].c == 0){
    NewtonRaphsonVetor[netlist[i].c] = 0;
    }
    if (netlist[i].d == 0){
      NewtonRaphsonVetor[netlist[i].d] = 0;
    }

      if ((NewtonRaphsonVetor[netlist[i].c]-NewtonRaphsonVetor[netlist[i].d]) < netlist[i].valor){ //param3 = vref =valor
        Yn[netlist[i].a][netlist[i].a]+=netlist[i].goff;//param2 = goff
        Yn[netlist[i].b][netlist[i].b]+=netlist[i].goff;
        Yn[netlist[i].a][netlist[i].b]-=netlist[i].goff;
        Yn[netlist[i].b][netlist[i].a]-=netlist[i].goff;
      }
      else {
        Yn[netlist[i].a][netlist[i].a]+=netlist[i].gon;//param1=gon
        Yn[netlist[i].b][netlist[i].b]+=netlist[i].gon;
        Yn[netlist[i].a][netlist[i].b]-=netlist[i].gon;
        Yn[netlist[i].b][netlist[i].a]-=netlist[i].gon;
      }

  }
}

void montarEstampas()
{
  for (i=1; i<=ne; i++)
  {
    tipo=netlist[i].nome[0];
    estampas(tipo);

   #ifdef DEBUG  /* Opcional: Mostra o sistema apos a montagem da estampa */
      printf("Sistema apos a estampa de %s\n",netlist[i].nome);
      for (k=1; k<=nv; k++)
      {
        for (j=1; j<=nv+1; j++)
        if (Yn[k][j]!=0) printf("%+3.1f ",Yn[k][j]);
        else printf(" ... ");
        printf("\n");
      }
      getch();
   #endif

 }
};

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

void zeraSistema (void) {
  for (i=0; i<=nv; i++)
    for (j=0; j<=nv+1; j++)
      Yn[i][j]=0;
}

double heaviside(double t)
{
  if (t >= 0)
    return 1;
  else
    return 0;
}

void salvarResultadoEmArquivo (vector <double> resultadoUmTempo)
{
  //  ofstream arquivo;
     //arquivo.open ("analise no tempo.txt", ios::app);
     for (i=0; i < static_cast <int> (resultadoUmTempo.size()); i++){
       fprintf (arquivoSolucao, "%g ",resultadoUmTempo.at(i) );
     }
     if (tempoAtual < tempoFinal){
       fprintf (arquivoSolucao, "\n");
     }

    // arquivo.close();
}

int leNetlist (void){
  ne=0; nv=0; strcpy(lista[0],"0");
  printf("Nome do arquivo com o netlist (ex: mna.net): ");
  scanf("%50s",nomearquivo);
  arquivoNetlist=fopen(nomearquivo,"r");
  if (arquivoNetlist==0) {
    printf("Arquivo %s inexistente\n",nomearquivo);
    return 1;
  }

  printf("Lendo netlist:\n");
  fgets(txt,MAX_LINHA,arquivoNetlist);
  printf("Titulo: %s",txt);
  while (fgets(txt,MAX_LINHA,arquivoNetlist)) {
    ne++; /* Nao usa o netlist[0] */
    if (ne>MAX_ELEM) {
      printf("O programa so aceita ate %d elementos\n",MAX_ELEM);
      exit(1);
    }
    /*transforma o primeiro caractere da linha lida em letra maiuscula,
    para associar com algum tipo.
    ex: R = Resistor*/
    txt[0]=toupper(txt[0]);
    tipo=txt[0];
    sscanf(txt,"%10s",netlist[ne].nome);
    p=txt+strlen(netlist[ne].nome); /* Inicio dos parametros */
    /* dependendo do tipo que esta sendo lida, a forma de preencher
    a netlist eh diferente. aqui ele agrupa os tipos que tem os mesmos
    tratamentos e separa com if */
    if (tipo=='R' || tipo=='C' || tipo=='L') {
      sscanf(p,"%10s%10s%lg",na,nb,&netlist[ne].valor);
      printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
      /*a funcao "numero" converte o nó que o usuario nomeou em um numero int
      e coloca esse numero no netlist do elemento.
      caracteriza em qual no o componente esta ligado.
      Ex: Resistor ligado a no1 e no2, a=1, b=2*/
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
    }

    else if (tipo=='I' || tipo=='V') {
      /*pega os nos que as fontes estao ligadas, e o tipo de fonte
      Ex: fonte DC, fonte senoidal*/
      sscanf(p,"%10s%10s%10s",na,nb,netlist[ne].fonte);
      /*esse +3 eh para mover o ponteiro p que aponta
      para as posicoes da linha "txt" no lugar certo.
      representa os 3 espacos q o sscanf pulou ate agr*/
      p=txt+strlen(netlist[ne].nome)+strlen(na)+strlen(nb)
      +strlen(netlist[ne].fonte)+1+1+1;

      /*se nivel DC*/
      if (strcmp(netlist[ne].fonte, "DC") == 0){
        sscanf(p,"%lg", &netlist[ne].valor);
        printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
      }
      /*se for sin*/
      else if (strcmp(netlist[ne].fonte, "SIN") == 0){
        sscanf(p, "%lg%lg%lg%lg%lg%lg%lg", &netlist[ne].valor,
                                                &netlist[ne].amplitude,
                                                &netlist[ne].freq,
                                                &netlist[ne].atraso,
                                                &netlist[ne].amortecimento,
                                                &netlist[ne].defasagem,
                                                &netlist[ne].ciclo);
        printf("%s %s %s %g ",netlist[ne].nome,na,nb,netlist[ne].valor);
        printf("%lg %lg\n", netlist[ne].amplitude, netlist[ne].freq );
      }
      /*se for pulso*/
      else if (strcmp(netlist[ne].fonte, "PULSE") == 0){
        sscanf(p, "%lg %lg %lg %lg %lg %lg %lg %lg", &netlist[ne].valor,
                                                &netlist[ne].amplitude,
                                                &netlist[ne].atraso,
                                                &netlist[ne].tempoSubida,
                                                &netlist[ne].tempoDescida,
                                                &netlist[ne].tempoLigada,
                                                &netlist[ne].periodo,
                                              &netlist[ne].ciclo);
        printf("%s %s %s %g ",netlist[ne].nome,na,nb,netlist[ne].valor);
        printf("%lg %lg\n", netlist[ne].amplitude, netlist[ne].atraso );

      }
      /*se n for nada*/
      else{
        printf("Fonte desconhecida - Tente DC ou SIN\n");
        getch();
        exit(1);
      }
      /*a funcao "numero" converte o nó que o usuario nomeou em um numero int
      e coloca esse numero no netlist do elemento.
      caracteriza em qual no o componente esta ligado.
      Ex: Resistor ligado a no1 e no2, a=1, b=2*/
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
    else if (tipo=='K') {         /*Transmador Ideal - 07/10/2017*/
      sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[ne].valor);
      printf("%s %s %s %s %s %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='$') {         /*Chave - 22/10/2017*/
      /*valor, nesse caso, eh o valor de referencia para decidir se vai
      operar no gon ou goff*/
      sscanf(p,"%10s%10s%10s%10s%lg%lg%lg",na,nb,nc,nd,&netlist[ne].gon,&netlist[ne].goff, &netlist[ne].valor);
      printf("%s %s %s %s %s %g %g %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].gon, netlist[ne].goff,netlist[ne].valor);
      netlist[ne].a=numero(na);
      netlist[ne].b=numero(nb);
      netlist[ne].c=numero(nc);
      netlist[ne].d=numero(nd);
    }
    else if (tipo=='*') { /* Comentario comeca com "*" */
      printf("Comentario: %s",txt);
      ne--;
    }

    else if (tipo=='.') { /* Comentario comeca com "*" */
      sscanf(p, "%lg%lg%10s%lg", &tempoFinal, &passo, netlist[ne].nome, &passoPorPt);
      printf("Configuracao: %g %g %g \n", tempoFinal, passo, passoPorPt);
      ne--;

    }

    else {
      printf("Elemento desconhecido: %s\n",txt);
      getch();
      exit(1);
    }
  }
  fclose(arquivoNetlist);
  return 0;
}

void atualizarMemoriasCapacitorIndutor()
{
  //  Apos resolucao do sistema no dominio do tempo, eh preciso atualizaar as memorias
  // de corrente e tensao nos capacitores e indutores do sistema (feito nessa funcao);
  // Lembrar que: "Yn[i][nv+1]" é a solucao da tensao no nó i para o tempo atual (é mesmo?)
  // Olha pagina 104 pra entender as contas
  for (i=1; i<=ne; i++)
    {
      tipo=netlist[i].nome[0];

      if (tipo=='C')
      {
        g=((2*netlist[i].valor)/(passo)); //Valor da condutancia para o MetodoDosTrapezios
        //aqui defini a tensao da terra. qnd a netlist[i].a/b eh zero, quer dzr
        // q uma das perninhas esta ligado a terra.
        if (netlist[i].a == 0){
        Yn[netlist[i].a][nv+1] = 0;
        }
        if (netlist[i].b == 0){
          Yn[netlist[i].b][nv+1] = 0;
        }
        netlist[i].jt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1]))*g -1*(g*netlist[i].vt0+netlist[i].jt0); //jt0 = (ea-eb)/R - I ; R = 1/g, I = g*vt0+jt0
        netlist[i].vt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1])); //vt0 = (ea - eb)
      }
      else
      if (tipo=='L')
      {
        if (netlist[i].a == 0){
          Yn[netlist[i].a][nv+1] = 0;
        }
        if (netlist[i].b == 0){
          Yn[netlist[i].b][nv+1] = 0;
        }
        g=((passo)/(2*netlist[i].valor));                                                                              //Valor da condutancia para o MetodoDosTrapezios
        netlist[i].jt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1]))*g + (g*netlist[i].vt0+netlist[i].jt0);  //jt0 = (ea-eb)/R + I ; R = 1/g, I = g*vt0+jt0
        netlist[i].vt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1]));                                        //vt0 = (ea - eb)
      }
    }
}

void analisePontoOperacao()  //POR ENQUANTO SO INICIA TUDO COMO ZERO
{
  analisandoPontodeOp = 1;
  //resolve o circuito uma vez para achar o ponto de operacao
  //monsta estampa aqui, e resolve o sistema uma vez
  for (i=1; i<=ne; i++)
  {
    tipo=netlist[i].nome[0];
    estampas(tipo);
  }
  if (resolversistema())
  {
    getch();
    exit(0);
  }

  //Inicializa as Tensoes/Correntes em Capacitores/Indutores
  for (i=1; i<=ne; i++)
  {
    tipo=netlist[i].nome[0];
    if (netlist[i].a == 0){
      Yn[netlist[i].a][nv+1] = 0;
    }
    if (netlist[i].b == 0){
      Yn[netlist[i].b][nv+1] = 0;
    }
    if (tipo=='C'){
      netlist[i].jt0 = 0;
      netlist[i].vt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1]));
    }
    if (tipo=='L'){
      netlist[i].jt0 = ((Yn[netlist[i].a][nv+1]) - (Yn[netlist[i].b][nv+1]))*GIndutorCurto;
      netlist[i].vt0 = 0;
    }
  }

  cout << "Sistema resolvido para Ponto de Operacao:" << endl;
  for (i=1; i<=nv; i++)
  {
    for (j=1; j<=nv+1; j++)
      if (Yn[i][j]!=0) printf("%+3.1f ",Yn[i][j]);
      else printf(" ... ");
      printf("\n");
  }

  analisandoPontodeOp = 0;
}

void printProgresso(int i, char simbolo) //Printa o progresso do calculo, dependendo do passo atual
{
  int porcentagem = 100/i;
  int progredi =
  static_cast <int> (floor((tempoAtual/(tempoFinal/porcentagem))) - floor( (tempoAtual-passo) / (tempoFinal/porcentagem) ) );

  if(tempoAtual == 0)
    {
      cout << "Progresso: ";
    }

  if(progredi != 0)   //se eu tiver progredido
    {
      for(int i = 1;i <= progredi;i++)  //printo pauzinhos suficientes para quanto eu progredi
      cout << simbolo;
    }

  if(tempoAtual > tempoFinal-passo)
  {
      cout << endl;
  }
}

void plotarGrafico()
{
  string constTab(NOME_ARQUIVO_TAB);    //Nome do arquivo
  string constPY(NOME_ARQUIVO_GERAR_PLOT_PYTHON); //Nome do script python que plota arquivo
  string varPlot = "";        //variavel a ser plotada
  string SysString = "";      //menssagem a ser enviada pro cmd do windows
  string varNomes;            //string com primeira linha do arquivo .tab (todas as variaveis)
  string varPontos = ConvertDoubleToString(floor(tempoFinal/passo));
  ifstream file;

  file.open(NOME_ARQUIVO_TAB);        //SCRIPT Q PEGUEI DA INTERNET PRA LER PRIMEIRA LINHA DE UM ARQUIVO

  if (!file) //checks to see if file opens properly
    {
      varNomes = "";
    }
    else
    {
      if (getline(file, varNomes))// Get line.
      file.close(); // Remember to close the file.
    }

  while(true) //loop pra perguntar a variavel pra plotar, fica aqui enquanto nao digita nada
  {
    cin.clear();        //precisa disso se nao buga
    fflush(stdin);      //precisa disso se nao buga
    cout << "Qual variavel deseja plotar?(0 para nenhuma)" << endl;
    cout << "[Nome das variaveis: " << varNomes << " ]: ";

    getline(cin,varPlot);
      if ((varPlot.compare(""))!= 0)
        break;
    cout << "[" << varPlot << "] eh um caracter invalido, tente outro" << endl;
  }

  if ((varPlot.compare("0"))!= 0) //caso eu queira plotar algo
  {
    SysString = "python " + constPY + " " + constTab + " " + varPlot + " " + varPontos; //monta a menssagem para o cmd
    //cout <<typeid(constPY).name() <<endl;
    //cout << varPontos << endl;
    cout << "Chamando por: " << SysString << endl;

    system(SysString.c_str());  //funcao feia que funciona
  }
}

const char* ConvertDoubleToString(double value){//funcao que peguei na internet pra converter double e char*
    stringstream ss ;
    ss << value;
    const char* str = ss.str().c_str();
    return str;
}

void CopiaSolucaoNR (void) {
  int i;
  for (i=0; i<=nv; i++)
  	NewtonRaphsonVetor[i] = Yn[i][nv+1];
}

void ChutaValorNR (void) {
	srand(time(NULL));
	for (i=1; i<=nv; i++){
	  NewtonRaphsonVetor[i] = (double)(((rand()%1001)/10)-50);
	}
}

int ComparaValorNR (void) {
  erroGrande=0;
  for (i=1; i<=nv; i++){
    if ( (fabs(Yn[i][nv+1] - NewtonRaphsonVetor[i])) > MAX_ERRO_NR ) {
      erroGrande=1;
      }
  }
     if (erroGrande==1) return 0;
     else return 1;
}

void gminstepping()
{
  cout << "EITA FERRO" << endl;
}
