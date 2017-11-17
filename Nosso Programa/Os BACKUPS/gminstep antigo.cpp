void gminstepping()
{
  gs = CONDUTANCIA_INICIAL_GS;
  fazendoGminStepping = 1;
  //cout << "to indo pro gmin steppp!" << endl;
  do{
      zeraSistema();
      montarEstampas();
      if (resolversistema())
      {
        getch();
        exit(0);
      }
      CopiaSolucaoNR();

      if ((fazendoGminStepping ==1) && (gs > CONUTANCIA_MINIMA_GS  )){
        if (gs > 10000){
          gs-=100;
        }
        else if (gs > 10){
         gs -= 0.1;
        }
        else if (gs >0.001){
          gs -= 0.00001;
        }
        else if (gs> 0.00000001){
          gs=-0.00000001;
        }
      }
      //cout << " gs="<< gs<< endl;
    }while(gs > CONUTANCIA_MINIMA_GS );
  //cout << "ja fiz gmin steppp!" << endl;

}
