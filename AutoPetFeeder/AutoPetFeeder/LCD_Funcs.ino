////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES GENÉRICAS DO MENU
////////////////////////////////////////////////////////////////////////////////
void NavegaMenu(const String* elementsMenu, int QtdeElementos){     
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
  lcd.setCursor(1,1);
  lcd.print(elementsMenu[NavigateMenuIndex]);
}

void AcessarMenu(bool* menuToAccess, const String* elementsMenu){
  NavigateMenuIndex = 0;
  *menuToAccess = !*menuToAccess;
          
  //Atualiza Texto no LCD
  lcd.setCursor(1,1);
  lcd.print(elementsMenu[0]);
}

void EditaItemMenu(int* selectedVar){
  int tmpVar = *selectedVar;
  if (digitalRead(BtnMenuDireita) == estadoBotao){
    tmpVar++;
  }
  if (digitalRead(BtnMenuEsquerda) == estadoBotao){
    tmpVar--;
  }
  *selectedVar = tmpVar;    
}
void EditaItemMenu(volatile uint16_t* selectedVar){
  int tmpVar = *selectedVar;
  if (digitalRead(BtnMenuDireita) == estadoBotao){
    tmpVar++;
  }
  if (digitalRead(BtnMenuEsquerda) == estadoBotao){
    tmpVar--;
  }
  *selectedVar = tmpVar;    
}

void LimitaVariaveis(int* selectedVar, int limite1, int limite2){
  int tmpVar = *selectedVar;
  if(tmpVar < limite1){
    tmpVar = limite2;
  }
  if(tmpVar > limite2){
    tmpVar = limite1;
  }
  *selectedVar=tmpVar;
}
void LimitaVariaveis(volatile uint16_t* selectedVar, int limite1, int limite2){
  int tmpVar = *selectedVar;
  if(tmpVar < limite1){
    tmpVar = limite2;
  }
  if(tmpVar > limite2){
    tmpVar = limite1;
  }
  *selectedVar=tmpVar;
}

void ImprimeVlrVariavel(int* selectedVar){
  int tmpVar = *selectedVar;
  lcd.setCursor(13,1);
  if(tmpVar < 10){
    lcd.print("0");
    lcd.setCursor(14,1);
  }
  lcd.print(tmpVar);
}
void ImprimeVlrVariavel(volatile uint16_t* selectedVar){
  int tmpVar = *selectedVar;
  lcd.setCursor(13,1);
  if(tmpVar < 10){
    lcd.print("0");
    lcd.setCursor(14,1);
  }
  lcd.print(tmpVar);
}

void ImprimeSetasMenu(){
  lcd.setCursor(0,1);
  lcd.print("<");
  lcd.setCursor(15,1);
  lcd.print(">");
}

void ImprimeSinaisMenu(){
  lcd.setCursor(0,1);
  lcd.print("-");
  lcd.setCursor(15,1);
  lcd.print("+");
}

void ResetaMenu(){
  NavigateMenuIndex=0;
  CheckProgHorarioAccess=false;
  CheckConfigRelogioAccess = false;
  CheckProgHorarioSet1Access = false;
  CheckProgHorarioSet2Access = false;
  CheckAlimentarAgoraAccess = false;
  ImprimeSetasMenu();
  lcd.setCursor(1,1);
  lcd.print(MainMenuOptions[0]);
}
