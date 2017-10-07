extern elemento netlist[MAX_ELEM]; /* Netlist */

extern int
  ne, /* Elementos */
  nv, /* Variaveis */
  nn, /* Nos */
  i,j,k;

extern char
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
// FILE *arquivo;

extern double
  g,
  Yn[MAX_NOS+1][MAX_NOS+2];
