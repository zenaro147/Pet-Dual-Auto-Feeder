void AtualizaVarsRelogio(){
  DateTime agora = rtc.now();
  dadosRTC[0] = (int)agora.hour();
  dadosRTC[1] = (int)agora.minute();
  if(dadosRTC[2] != (int)agora.day()){
    dadosRTC[2] = (int)agora.day();
    dadosRTC[3] = (int)agora.month();
    dadosRTC[4] = (int)agora.year();
  }
}

void AjustaRelogio(){
  ValidaGatilhos();
  rtc.adjust(DateTime(dadosRTC[4], dadosRTC[3], dadosRTC[2], dadosRTC[0], dadosRTC[1], 0));
}

void MostraDataHoraDisplay(){
  if(boolTelaLigada){
    //Imprime Data e Hora no display
    char buffLCD[16];
    sprintf(buffLCD, "%02d/%02d/%4d %2d:%02d", dadosRTC[2], dadosRTC[3],dadosRTC[4],dadosRTC[0],dadosRTC[1]);
    lcd.setCursor(0,0);    
    lcd.print(buffLCD);
  }
}

void ValidaGatilhos(){
  if(dadosTimer[0] < dadosRTC[0] || (dadosTimer[0] <= dadosRTC[0] && dadosTimer[1] < dadosRTC[1])){
    timerJaAcionou[0]=true;
  }else{
    timerJaAcionou[0]=false;
  }
  if(dadosTimer[2] < dadosRTC[0] || (dadosTimer[2] <= dadosRTC[0] && dadosTimer[3] < dadosRTC[1])){
    timerJaAcionou[1]=true;
  }else{
    timerJaAcionou[1]=false;
  }
}

void SincronizaRelogio(){
  if((unsigned long)(millis() - millisRTC)/1000 >= intervaloAttRelogio){
    
    AtualizaVarsRelogio();
    
    #ifdef DEBUG_MODE    
      Serial.println("Status: ");
      
      char buffHorario[10];
      sprintf(buffHorario, "%2d:%02d", dadosTimer[0],dadosTimer[1]);
      Serial.print("Gatilho 1: ");
      Serial.println(buffHorario);
      
      sprintf(buffHorario, "%2d:%02d", dadosTimer[2],dadosTimer[3]);
      Serial.print("Gatilho 2: ");
      Serial.println(buffHorario);
      
      char buffAgora[20];
      sprintf(buffAgora, "%02d/%02d/%4d - %2d:%02d", dadosRTC[2], dadosRTC[3],dadosRTC[4],dadosRTC[0],dadosRTC[1]);
      Serial.print("Data/Hora Atual: ");
      Serial.println(buffAgora);
    #endif

    MostraDataHoraDisplay();
    
    millisRTC = millis();
  }
}
