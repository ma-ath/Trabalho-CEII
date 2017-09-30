/*
 *  Seperando alguns elementos do netlist em arquivos
 */
#include <string>
#include "constantes.hpp"

  using namespace std;

class elemento   /* Elemento do netlist */
{
public:
    elemento(string nome, double valor);
    string getNome();    /*Classes get e set para os parâmetros de um elemento*/
    double getValor();
    int getA();
    int getB();
    int getC();
    int getD();
    int getX();
    int getY();

    this* setNome(char);    /*Classes get e set para os parâmetros de um elemento*/
    this* setValor(double);
    this* setA(int);
    this* setB(int);
    this* setC(int);
    this* setD(int);
    this* setX(int);
    this* setY(int);
private:
    char nome[MAX_NOME];
    double valor;
    int a,b,c,d,x,y;
};

elemento::elemento(string nome,double valor)
{
  this->nome = nome;
  this->valor = valor;
}

string elemento::getNome(){return this->nome;};
double elemento::getValor(){return this->valor;};
int elemento::getA(){return this->a;};
int elemento::getB(){return this->b;};
int elemento::getC(){return this->c;};
int elemento::getD(){return this->d;};
int elemento::getX(){return this->x;};
int elemento::getY(){return this->y;};
this* elemento::getNome(){return this;};
this* elemento::getValor(){return this;};
this* elemento::getA(){return this;};
this* elemento::getB(){return this;};
this* elemento::getC(){return this;};
this* elemento::getD(){return this;};
this* elemento::getX(){return this;};
this* elemento::getY(){return this;};
