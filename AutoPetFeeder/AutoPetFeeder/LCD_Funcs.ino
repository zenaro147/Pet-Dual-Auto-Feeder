////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES GENÉRICAS DO MENU
////////////////////////////////////////////////////////////////////////////////
void NavegaMenu(String* elementsMenu, short QtdeElementos){     
  if (digitalRead(BtnMenuDireita) == estadoBotao){
    NavigateMenuIndex++;
  }
  if (digitalRead(BtnMenuEsquerda) == estadoBotao){
    NavigateMenuIndex--;
  }

  //Define manualmente o Index caso ultrapasse os limites
  if(NavigateMenuIndex > QtdeElementos-1){
    NavigateMenuIndex = 0;
  } else if (NavigateMenuIndex < 0){
    NavigateMenuIndex = QtdeElementos-1;
  }
  
  //Atualiza Texto no LCD
  lcd_1.setCursor(1,1);
  lcd_1.print(elementsMenu[NavigateMenuIndex]);
}

void AcessarMenu(bool* menuToAccess, String* elementsMenu){
  NavigateMenuIndex = 0;
  *menuToAccess = !*menuToAccess;
          
  //Atualiza Texto no LCD
  lcd_1.setCursor(1,1);
  lcd_1.print(elementsMenu[0]);
}

void EditaItemMenu(short* selectedVar){
  short tmpVar = *selectedVar;
  if (digitalRead(BtnMenuDireita) == estadoBotao){
    tmpVar++;
  }
  if (digitalRead(BtnMenuEsquerda) == estadoBotao){
    tmpVar--;
  }
  *selectedVar = tmpVar;    
}

void LimitaVariaveis(short* selectedVar, short limite1, short limite2){
  short tmpVar = *selectedVar;
  if(tmpVar < limite1){
    tmpVar = limite2;
  }
  if(tmpVar > limite2){
    tmpVar = limite1;
  }
  *selectedVar=tmpVar;
}

void ImprimeVlrVariavel(short* selectedVar){
  short tmpVar = *selectedVar;
  lcd_1.setCursor(13,1);
  if(tmpVar < 10){
    lcd_1.print("0");
    lcd_1.setCursor(14,1);
  }
  lcd_1.print(tmpVar);
}

void ImprimeSetasMenu(){
  lcd_1.setCursor(0,1);
  lcd_1.print("<");
  lcd_1.setCursor(15,1);
  lcd_1.print(">");
}

void ImprimeSinaisMenu(){
  lcd_1.setCursor(0,1);
  lcd_1.print("-");
  lcd_1.setCursor(15,1);
  lcd_1.print("+");
}

void ResetaMenu(){
  NavigateMenuIndex=0;
  CheckProgHorarioAccess=false;
  CheckConfigRelogioAccess = false;
  CheckProgHorarioSet1Access = false;
  CheckProgHorarioSet2Access = false;
  CheckAlimentarAgoraAccess = false;  
  lcd_1.setCursor(0, 0);
  lcd_1.print("Alimentador Pet");
  ImprimeSetasMenu();
  lcd_1.setCursor(1,1);
  lcd_1.print(MainMenuOptions[0]);
}
