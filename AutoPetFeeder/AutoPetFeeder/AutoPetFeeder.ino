////////////////////////////////////////////////////////////////////////////////
// Alimentador PET v1.0 com capacidade para alimentar 2 potes
//
// Requisitos:
// Arduino Mega2560
// Modulo RTC DS1307
// Servo motor SG90
// Motor de Passo 28BYJ-48 + driver controle ULN2003 
// Display LCD 16x2 I2C
// 3 botões (push button/chave tactil)
// Módulo Conversor Step Down 6/24VDC Para 5V 3A
// Buzzer 5v Ativo (opcional)
// Rele T73 DC-12V 10A (Caso queira colocar uma fonte de energia alternativa)
//
////////////////////////////////////////////////////////////////////////////////
#include "config.h"

//Bibliotecas do LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Biblioteca Servo
#include <Servo.h>

//Biblioteca RTC
#include "RTClib.h"

//Biblioteca EEPROM
#include <EEPROM.h>

//Biblioteca Motor de passo
#include <Stepper.h>

#include "LCD_MenuOpcoes.h"

////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS INTERNAS DO PROGRMAA
////////////////////////////////////////////////////////////////////////////////
int servoPos = 0;
Servo mServo;

//Define LCD I2C
LiquidCrystal_I2C lcd(enderecoLCD, LCDqtdeColunas, LCDqtdeLinhas);
bool boolTelaLigada = true;
volatile unsigned long millisLCD = 0;

//Define RTC
RTC_DS1307 rtc;

//Define Motor de passo
#ifdef InverterIN2IN3
Stepper mp(passosPorVolta, PinoIN1,PinoIN3,PinoIN2,PinoIN4); //Caso tenha probemas com o motor, inverter a posição do 9 e 10
#else
Stepper mp(passosPorVolta, PinoIN1,PinoIN2,PinoIN3,PinoIN4); //Caso tenha probemas com o motor, inverter a posição do 9 e 10
#endif

//Variavel para "travar" o Menu
bool CheckEditandoItem = false;

//Tempo em segundos que o motor ficará ligado jogando comida
int tempoAlimentadorLigado[3] = {5,5,5}; //Timer1, Timer2, Manual

//Variavel que controla se o Timer1 e/ou Timer2 está ativado
int acionarTimerAlimentador[2] = {0,0}; //Timer1, Timer2

//Hora e Minuto dos timers
int dadosTimer[] = {0,0,0,0}; //horaTimer1, minutoTimer1, horaTimer2, minutoTimer2

//Variavel que controla se o Timer1 e/ou Timer2 ja foi acionado no dia
bool timerJaAcionou[2] = {false,false}; //Timer1, Timer2

//Data e Hora do RTC
volatile uint16_t dadosRTC[] = {0,0,1,1,2023}; //Hora, Minuto, Dia, Mes, Ano
volatile unsigned long millisRTC = 0;

////////////////////////////////////////////////////////////////////////////////
// FUNÇÃO SETUP DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////
void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup(){
  Serial.begin(115200);

  Serial.println("//////////////////////////////////////////");
  Serial.println("Alimentador PET v1.0");
  Serial.println("//////////////////////////////////////////");
  Serial.println("Iniciando programa...");
  delay (1000);

  Serial.println("Iniciando LCD...");
  SetupLCD();
  
  Serial.println("Iniciando servo...");
  SetupServo();

  Serial.println("Iniciando motor de passo...");
  mp.setSpeed(velocidadeMotor);

  Serial.print("Lendo configurações salvas...");
  if(LeMemoria()){
    Serial.println("Dados carregados!");
  }else{    
    Serial.println("Não existem valores dados a serem carregados!");
  }

  Serial.print("Iniciando relógio...");
  SetupRTC();

  Serial.println("Iniciando demais pinos e buzzer...");
  // Define pino dos menus
  pinMode(BtnMenuEsquerda, INPUT_PULLUP);
  pinMode(BtnMenuDireita, INPUT_PULLUP);
  pinMode(BtnMenuSelect, INPUT_PULLUP);  
  
  //Pino do Buzzer
  pinMode(BuzzerPin, OUTPUT);
  tone(BuzzerPin, BuzzerNote, 250);

  //Valida gatilhos a serem disparados baseado no horario
  Serial.println("Atualizando gatilhos...");
  ValidaGatilhos();
  
  millisLCD = millis();
  millisRTC = millisLCD;
  Serial.println("Programa iniciado!");
  Serial.println("//////////////////////////////////////////");

  mServo.detach();
  
  //Imprime Data e Hora no display
  char buffLCD[18] = {0};
  sprintf(buffLCD, "%02d/%02d/%4d %2d:%02d", dadosRTC[2], dadosRTC[3],dadosRTC[4],dadosRTC[0],dadosRTC[1]);
  lcd.setCursor(0,0);
  lcd.print(buffLCD);
}

void SetupServo(){
  mServo.attach(ServoPin);
  mServo.write(AnguloPadraoServo);
}

void SetupLCD(){
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Alimentador Pet");
  lcd.setCursor(0, 1);
  lcd.print("  Iniciando...");
  ImprimeSetasMenu();
  lcd.setCursor(1,1);
  lcd.print(MainMenuOptions[0]);
}

void SetupRTC(){
  if (!rtc.begin()) {
    Serial.println("Erro ao iniciar relógio!");
  }else{    
    AtualizaVarsRelogio();
    Serial.print("Sucesso! Data/Hora Atual: ");      
    char buffLCD[20];
    sprintf(buffLCD, "%02d/%02d/%4d - %2d:%02d", dadosRTC[2], dadosRTC[3],dadosRTC[4],dadosRTC[0],dadosRTC[1]);
    Serial.println(buffLCD);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC precisa ser configurado!");
    rtc.adjust(DateTime(dadosRTC[4], dadosRTC[3], dadosRTC[2], dadosRTC[0], dadosRTC[1], 0));
  }
}

////////////////////////////////////////////////////////////////////////////////
// FUNÇÃO LOOP DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////

void loop(){
  if(!CheckProgHorarioAccess && !CheckConfigRelogioAccess){
    SincronizaRelogio();
  }

  if (digitalRead(BtnMenuEsquerda) == estadoBotao || digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuSelect) == estadoBotao){
    if(!boolTelaLigada){
      lcd.display();
      lcd.backlight();
      boolTelaLigada=true;
      MostraDataHoraDisplay();
    }
    millisLCD = millis();
    tone(BuzzerPin, BuzzerNote, 10);
    ProcessaMenu();
    delay(250); 
  }else{
    if(boolTelaLigada){
      if((unsigned long)(millis() - millisLCD)/1000 > timeoutLCD){
        lcd.noDisplay();
        lcd.noBacklight();
        boolTelaLigada=false;
      }
    }
    
    //Aciona o alimentador no horario determinado
    if(acionarTimerAlimentador[0] == 1 && !timerJaAcionou[0] && dadosRTC[0] == dadosTimer[0] && dadosRTC[1] == dadosTimer[1]){
      timerJaAcionou[0] = true;
      ExecAlimentar(tempoAlimentadorLigado[0]);
    }
    if(acionarTimerAlimentador[1] ==  1&& !timerJaAcionou[1] && dadosRTC[0] == dadosTimer[2] && dadosRTC[1] == dadosTimer[3]){
      timerJaAcionou[1] = true;
      ExecAlimentar(tempoAlimentadorLigado[1]);
    }
  }
}

void ExecAlimentar(short tempoLigado){
  SetupServo();
  
  lcd.setCursor(0, 1);
  lcd.print("Preparando pote1");
  Serial.println("Alimentando pote 1...");  
  //Vira o Servo para alimentar o primeiro pote
  tone(BuzzerPin, BuzzerNote, 500);
  for (servoPos = AnguloPadraoServo; servoPos <= AnguloPadraoServo+AnguloServo; servoPos += 1) {
    mServo.write(servoPos);
    delay(10);
  }  
  //Alimenta o pote
  unsigned long millisMotor = millis();
  while(millis() - millisMotor <= tempoLigado*1000){
    mp.step(1);
  } 

  lcd.setCursor(0, 1);
  lcd.print("Preparando pote2");
  Serial.println("Alimentando pote 2...");  
  //Vira o Servo para alimentar o segundo pote
  for (servoPos = AnguloPadraoServo+AnguloServo; servoPos >= AnguloPadraoServo-AnguloServo; servoPos -= 1) {
    mServo.write(servoPos);
    delay(10);
  }  
  //Alimenta o pote
  millisMotor = millis();  
  while(millis() - millisMotor <= tempoLigado*1000){
    mp.step(1);
  }
  
  lcd.setCursor(0, 1);
  lcd.print(" Finalizando... ");
  Serial.println("Finalizando...");  
  //Reseta o Servo
  for (servoPos = AnguloPadraoServo-AnguloServo; servoPos <= AnguloPadraoServo; servoPos += 1) {
    mServo.write(servoPos);
    delay(10);
  }

  mServo.detach();
  
  tone(BuzzerPin, BuzzerNote, 250);
  delay(500);
  tone(BuzzerPin, BuzzerNote, 250);

  lcd.setCursor(0, 1);
  lcd.print("   Concluido!   ");
  Serial.println("Ração colocada!");
  delay(2000);
  ResetaMenu();
}



  
