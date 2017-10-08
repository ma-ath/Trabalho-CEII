#include "global.h"
#include "funcoesAuxiliares.h"

elemento netlist[MAX_ELEM];
int ne;
int nv;
int nn;
int i;
int j;
int k;
char nomearquivo[MAX_LINHA+1];
char tipo;
char na[MAX_NOME];
char nb[MAX_NOME];
char nc[MAX_NOME];
char nd[MAX_NOME];
char lista[MAX_NOS+1][MAX_NOME+2];
char txt[MAX_LINHA+1];
char *p;
FILE *arquivo;
double g;
double Yn[MAX_NOS+1][MAX_NOS+2];

/*variavel para analise no tempo*/
double tempoAtual, tempoFinal, passo, passoPorPt;

int main()
{
  /* 1 - Leitura do netlist */
  denovo:

  vector <double> resultadoUmTempo;

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
    if (tipo=='R') {
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
  fclose(arquivo);
  /* Acrescenta variaveis de corrente acima dos nos, anotando no netlist */
  nn=nv;
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];

    /*se forem os tipos que precisam de UMA corrente pra montar o mna,
    o numero de variavel vai aumentar em 1, alem dos variaveis de nos*/
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O' || tipo=='K') {
      nv++;
      if (nv>MAX_NOS) {
        printf("As correntes extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
        exit(1);
      }
      strcpy(lista[nv],"j"); /* Tem espaco para mais dois caracteres */
      strcat(lista[nv],netlist[i].nome);
      netlist[i].x=nv;
    }
    /*o tipo H, q eh uma tensao ligada a corrente, precisam de 2 variaveis
    de corrente para construir a estampa*/
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
  printf("'Variaveis internas: \n");
  for (i=0; i<=nv; i++)
    printf("%d -> %s\n",i,lista[i]);
  getch();
  printf("Netlist interno final\n");
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R' || tipo=='I' || tipo=='V') {
      printf("%s %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor);
    }
    else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H' || tipo=='K') {
      printf("%s %d %d %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor);
    }
    else if (tipo=='O') {
      printf("%s %d %d %d %d\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d);
    }
    if (tipo=='V' || tipo=='E' || tipo=='F' || tipo=='O')
      printf("Corrente jx: %d\n",netlist[i].x);
    else if (tipo=='H')
      printf("Correntes jx e jy: %d, %d\n",netlist[i].x,netlist[i].y);
  }
  getch();
  /* Monta o sistema nodal modificado */
  printf("O circuito tem %d nos, %d variaveis e %d elementos\n",nn,nv,ne);
  getch();

  /*Loop responsavel pelo TEMPO*/
  tempoAtual = 0;
  while(tempoAtual <= tempoFinal)
  {

    /* Zera sistema */
    zeraSistema();
    /* Monta estampas */
    /*a b c d sao posicoes relacionadas aos nos
      x e y sao relacionadas as correntes, que entram na estampa pelos
      "pontilhadinhos" qnd a gente faz mna na mao*/
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

    /* Resolve o sistema */
    if (resolversistema())
    {
      getch();
      exit(0);
    }


    #ifdef DEBUG  /* Opcional: Mostra o sistema resolvido */
    printf("Sistema resolvido:\n");
    for (i=1; i<=nv; i++)
    {
      for (j=1; j<=nv+1; j++)
        if (Yn[i][j]!=0) printf("%+3.1f ",Yn[i][j]);
        else printf(" ... ");
        printf("\n");
    }
    getch();
    #endif

    resultadoUmTempo.push_back(tempoAtual);
    for (i=1; i<=nv; i++){
    //printf("tempo atual = %g, %g\n", tempoAtual, Yn[i][nv+1]);
    resultadoUmTempo.push_back(Yn[i][nv+1]);
    }

    salvarResultadoEmArquivo(resultadoUmTempo);
    resultadoUmTempo.clear();
/*    resultadoDeTodosOsPassos.push_back(resultadoUmTempo);

    for (i=0; i < resultadoDeTodosOsPassos.size(); i++){
      for (j=0; j< resultadoUmTempo.size(); j++){
        printf("%g ", resultadoUmTempo.at(j));
      }
      printf("\n");
    }
*/

  tempoAtual+=passo;
  } /*FIM loop de tempo*/

  /* Mostra solucao */
  printf("Solucao:\n");
  strcpy(txt,"Tensao");
  for (i=1; i<=nv; i++) {
    if (i==nn+1) strcpy(txt,"Corrente");
    printf("%s %s: %g\n",txt,lista[i],Yn[i][nv+1]);
  }
  getch();
  return 0;
}
