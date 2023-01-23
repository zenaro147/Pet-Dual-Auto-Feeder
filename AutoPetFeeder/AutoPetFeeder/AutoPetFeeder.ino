//#define DEBUG_MODE

//Bibliotecas do LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Biblioteca Servo
#include <Servo.h>

//Biblioteca RTC
#include <RTClib.h>

//Biblioteca EEPROM
#include <EEPROM.h>

//Biblioteca Motor de passo
#include <Stepper.h>

#include "LCD_MenuOpcoes.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINIÇÃO DE VARIAVEIS DO PROGRMAA
////////////////////////////////////////////////////////////////////////////////
//Botões do menu
#define BtnMenuEsquerda 3 //Define botão "Esquerda" do Menu
#define BtnMenuDireita 2  //Define botão "Direita" do Menu
#define BtnMenuSelect 18    //Define botão "Selecionar" do Menu
#define estadoBotao LOW

//Define Servo
#define ServoPin 22
int pos = 0;
#define AnguloServo 45 //Servo irá virar 45 graus para poder bloquear uma saida e despejar comida em um pote
Servo mServo;

//Define LCD I2C
LiquidCrystal_I2C lcd_1(0x27, 16, 2);

//Define RTC
RTC_DS1307 rtc;

//Define Motor de passo
int passosPorVolta = 32;
Stepper mp(passosPorVolta, 24,6,5,7); //Caso tenha probemas com o motor, inverter a posição do 9 e 10

//Pino Buzzer
#define BuzzerPin 23
#define BuzzerNote 698

//Variavel para "travar" o Menu
bool CheckEditandoItem = false;

//Tempo em segundos que o motor ficará ligado jogando comida
short tempoAlimentadorLigado[3] = {5,5,5}; //Timer1, Timer2, Manual

//Variavel que controla se o Timer1 e/ou Timer2 está ativado
short acionarTimerAlimentador[2] = {0,0}; //Timer1, Timer2

//Variavel que controla se o Timer1 e/ou Timer2 ja foi acionado no dia
bool timerJaAcionou[2] = {false,false}; //Timer1, Timer2

//Hora e Minuto dos timers
short dadosTimer[] = {0,0,0,0}; //horaTimer1, minutoTimer1, horaTimer2, minutoTimer2

//Data e Hora do RTC
short dadosRTC[] = {0,0,1,1,2023}; //Hora, Minuto, Dia, Mes, Ano
unsigned long millisRTC = millis();

////////////////////////////////////////////////////////////////////////////////
// FUNÇÃO SETUP DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////
void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup(){
  Serial.begin(115200);

  Serial.println("Iniciando programa...");
  delay (1000);

  Serial.print("Lendo configurações salvas...");
  if(LeMemoria()){
    Serial.println("Dados carregados!");
  }else{    
    Serial.println("Não existem valores dados a serem carregados!");
  }
  
  Serial.println("Iniciando servo...");
  SetupServo();
  
  Serial.println("Iniciando LCD...");
  SetupLCD();
  
  Serial.println("Iniciando motor de passo...");
  mp.setSpeed(500);

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
  if(dadosTimer[0] < rtc.now().hour() || (dadosTimer[0] <= rtc.now().hour() && dadosTimer[1] < rtc.now().minute())){
    timerJaAcionou[0]=true;
  }
  if(dadosTimer[2] < rtc.now().hour() || (dadosTimer[2] <= rtc.now().hour() && dadosTimer[3] < rtc.now().minute())){
    timerJaAcionou[1]=true;
  }
  
  Serial.println("Programa iniciado!");
}

void SetupServo(){
  mServo.attach(ServoPin, 500, 2500);
  mServo.write(90);
}

void SetupLCD(){
  lcd_1.init();
  lcd_1.backlight();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Alimentador Pet");
  ImprimeSetasMenu();
  lcd_1.setCursor(1,1);
  lcd_1.print(MainMenuOptions[0]);
}

void SetupRTC(){
  if (!rtc.begin()) {
    Serial.println("Erro ao iniciar relógio!");
  }else{    
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    AtualizaVarsRelogio();
    Serial.print("Sucesso! Hora Atual: ");
    Serial.print(dadosRTC[2]);
    Serial.print("/");
    Serial.print(dadosRTC[3]);
    Serial.print("/");
    Serial.print(dadosRTC[4]);
    Serial.print(" - ");
    Serial.print(dadosRTC[0]);
    Serial.print(":");
    Serial.println(dadosRTC[1]);
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
  //
  if (digitalRead(BtnMenuEsquerda) == estadoBotao || digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuSelect) == estadoBotao){
    Serial.println("Botão apertado!");
    //tone(BuzzerPin, BuzzerNote, 10);
    ProcessaMenu();
    delay(250); 
  }
  
  if(millis() - millisRTC >= 5*1000 && !CheckProgHorarioAccess && !CheckConfigRelogioAccess){
    
    AtualizaVarsRelogio();
    millisRTC = millis();
    
    #ifdef DEBUG_MODE    
      Serial.println("Status: ");
      
      Serial.print("Gatilho 1: ");
      Serial.print(timerJaAcionou[0]);
      Serial.print(" - ");
      Serial.print(dadosTimer[0]);
      Serial.print(":");
      Serial.println(dadosTimer[1]);
      
      Serial.print("Gatilho 2: ");
      Serial.print(timerJaAcionou[1]);
      Serial.print(" - ");
      Serial.print(dadosTimer[2]);
      Serial.print(":");
      Serial.println(dadosTimer[3]);
      
      Serial.print("Hora Atual: ");
      Serial.print(dadosRTC[2]);
      Serial.print("/");
      Serial.print(dadosRTC[3]);
      Serial.print("/");
      Serial.print(dadosRTC[4]);
      Serial.print(" - ");
      Serial.print(dadosRTC[0]);
      Serial.print(":");
      Serial.println(dadosRTC[1]);
    #endif

    //Verifica se já mudou o dia para resetar os acionadores
    short tmpDia = rtc.now().day();
    if(dadosRTC[2] != tmpDia){
      Serial.println("Gatilhos resetados!");
      Serial.println(dadosRTC[2]);
      Serial.println(rtc.now().day());
      if(acionarTimerAlimentador[0] == 1){
        timerJaAcionou[0] = true;
      }
      if(acionarTimerAlimentador[1] == 1){
        timerJaAcionou[1] = true;
      }
    }
  } 

  //Aciona o alimentador no horario determinado
  if(acionarTimerAlimentador[0] == 1 && !timerJaAcionou[0] && dadosRTC[0] == dadosTimer[0] && dadosRTC[1] == dadosTimer[1]){
    timerJaAcionou[0] = true;
    ExecAlimentar(tempoAlimentadorLigado[0]);
  }
  if(acionarTimerAlimentador[1] == 1 && !timerJaAcionou[1] && dadosRTC[0] == dadosTimer[2] && dadosRTC[1] == dadosTimer[3]){
    timerJaAcionou[1] = true;
    ExecAlimentar(tempoAlimentadorLigado[1]);
  }
  
}

void ExecAlimentar(short tempoLigado){  
  mServo.write(90); //Reseta Servo
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("Preparando pote1");
  Serial.println("Alimentando pote 1...");  
  tone(BuzzerPin, BuzzerNote, 500);
  for (pos = 90; pos <= 90+AnguloServo; pos += 1) {
    mServo.write(pos);
    delay(10);
  }  
  //Alimenta o pote
  unsigned long millisMotor = millis();
  while(millis() - millisMotor <= tempoLigado*1000){
    mp.step(1);
  } 

  lcd_1.setCursor(0, 1);
  lcd_1.print("Preparando pote2");
  Serial.println("Alimentando pote 2...");  
  //Vira o Servo para alimentar o segundo pote
  for (pos = 90+AnguloServo; pos >= 90-AnguloServo; pos -= 1) {
    mServo.write(pos);
    delay(10);
  }  
  //Alimenta o pote
  millisMotor = millis();  
  while(millis() - millisMotor <= tempoLigado*1000){
    mp.step(1);
  }
  
  lcd_1.setCursor(0, 1);
  lcd_1.print(" Finalizando... ");
  Serial.println("Finalizando...");  
  //Reseta o Servo
  for (pos = 90-AnguloServo; pos <= 90; pos += 1) {
    mServo.write(pos);
    delay(10);
  }   
  tone(BuzzerPin, BuzzerNote, 250);
  delay(500);
  tone(BuzzerPin, BuzzerNote, 250);

  lcd_1.setCursor(0, 1);
  lcd_1.print("   Concluido!   ");
  Serial.println("Ração colocada!");
  delay(2000);
  ResetaMenu();
}

void AtualizaVarsRelogio(){
  DateTime agora = rtc.now();
  dadosRTC[0]= agora.hour();
  dadosRTC[1]= agora.minute();
  dadosRTC[2]= agora.day();
  dadosRTC[3]= agora.month();
  dadosRTC[4]= agora.year();  
}
