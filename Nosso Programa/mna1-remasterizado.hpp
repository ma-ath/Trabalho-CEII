

class mna1
{
public:
  int resolversistema();

};


int mna1::resolversistema(double Yn[][],)
/* Resolucao de sistema de equacoes lineares.
   Metodo de Gauss-Jordan com condensacao pivotal */
   {
     int i,j,l,a;
     double t,p;

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
