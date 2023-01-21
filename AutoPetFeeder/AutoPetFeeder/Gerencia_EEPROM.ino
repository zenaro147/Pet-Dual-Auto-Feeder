//////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES EEPROM
//////////////////////////////////////////////////////////////////////////////////////////////
void ResetaMemoria(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  lcd_1.setCursor(0,1);
  lcd_1.print("Memoria zerada. ");  
  Serial.println("Memoria zerada.");
  delay(2000);
  ResetaMenu();
  resetFunc();
}

void LeMemoria(){
  //EEPROM.read(address);
}

void GravaMemoria(){
  //EEPROM.write(addr, val);
}
