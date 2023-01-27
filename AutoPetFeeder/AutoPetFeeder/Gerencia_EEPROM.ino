//////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES EEPROM
//////////////////////////////////////////////////////////////////////////////////////////////
void ResetaMemoria(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  lcd.setCursor(0,1);
  lcd.print("Memoria zerada. ");  
  Serial.println("EEPROM apagada.");
  delay(2000);
  ResetaMenu();
  resetFunc();
}

bool LeMemoria(){
  char validaConfig = EEPROM.read(0);
  if(validaConfig == 'C'){
    
    tempoAlimentadorLigado[0] = EEPROM.read(1);
    tempoAlimentadorLigado[1] = EEPROM.read(2);
    tempoAlimentadorLigado[2] = EEPROM.read(3);
    
    dadosTimer[0] = EEPROM.read(4);
    dadosTimer[1] = EEPROM.read(5);
    dadosTimer[2] = EEPROM.read(6);
    dadosTimer[3] = EEPROM.read(7);

    acionarTimerAlimentador[0] = EEPROM.read(8);
    acionarTimerAlimentador[1] = EEPROM.read(9);
    
    lcd.setCursor(0,1);
    lcd.print("Dados carregados");  
    Serial.println("Dados carregados da EEPROM.");
    delay(2000);
    ResetaMenu();
    return true;
  }else{
    return false;
  }
}

void GravaMemoria(){
  EEPROM.write(0, 'C');

  //Valores do tempo que o alimentador fica ligado
  EEPROM.write(1, tempoAlimentadorLigado[0]);
  EEPROM.write(2, tempoAlimentadorLigado[1]);
  EEPROM.write(3, tempoAlimentadorLigado[2]);

  //Hora e minuto dos timers que o alimentador será ligado
  EEPROM.write(4, dadosTimer[0]);
  EEPROM.write(5, dadosTimer[1]);
  EEPROM.write(6, dadosTimer[2]);
  EEPROM.write(7, dadosTimer[3]);

  //Indica se o Timer1 e/ou Timer2 está ativo
  EEPROM.write(8, acionarTimerAlimentador[0]);
  EEPROM.write(9, acionarTimerAlimentador[1]);
  
  lcd.setCursor(0,1);
  lcd.print("Dados salvos.   ");  
  Serial.println("Dados salvos da EEPROM.");
  delay(2000);
  ResetaMenu();
}
