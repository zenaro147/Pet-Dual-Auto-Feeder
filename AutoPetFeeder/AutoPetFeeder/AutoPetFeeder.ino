//Bibliotecas do LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Biblioteca Servo
#include <Servo.h>

//Biblioteca RTC
#include <RTClib.h>

//Biblioteca EEPROM
#include <EEPROM.h> //EEPROM tem 255bytes para serem lidos/gravados

//Biblioteca Motor de passo
#include <Stepper.h>

#include "LCD_MenuOpcoes.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINIÇÃO DE VARIAVEIS DO PROGRMAA
////////////////////////////////////////////////////////////////////////////////
//Botões do menu
#define BtnMenuEsquerda 13 //Define botão "Esquerda" do Menu
#define BtnMenuDireita 12  //Define botão "Direita" do Menu
#define BtnMenuSelect 7    //Define botão "Selecionar" do Menu

//Define Servo
#define ServoPin 4
int pos = 0;
#define AnguloServo 45 //Servo irá virar 45 graus para poder bloquear uma saida e despejar comida em um pote
Servo servo_9;

//Define LCD I2C
LiquidCrystal_I2C lcd_1(0x27, 16, 2);

//Define RTC
RTC_DS1307 rtc;

//Define Motor de passo
int passosPorVolta = 32;
Stepper mp(passosPorVolta, 8,10,9,11); //Caso tenha probemas com o motor, inverter a posição do 9 e 10

//Pino Buzzer
#define BuzzerPin 2
#define BuzzerNote 698

//Variavel para "travar" o Menu
bool CheckEditandoItem = false;
//Tempo em segundos que o motor ficará ligado jogando comida
short tempoAlimentadorLigado[3] = {5,5,5}; //Timer1, Timer2, Manual

//Hora e Minuto dos timers
short dadosTimer[] = {0,0,0,0}; //horaTimer1, minutoTimer1, horaTimer2, minutoTimer2

//Data e Hora do RTC
//short dadosRTC[] = {0,0,1,1,2023}; //Hora, Minuto, Dia, Mes, Ano
short dadosRTC[] = {0,0}; //Hora, Minuto

////////////////////////////////////////////////////////////////////////////////
// FUNÇÃO SETUP DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(115200);

  Serial.println("Iniciando programa...");
  delay (1000);
  
  Serial.println("Iniciando servo..");
  SetupServo();
  
  Serial.println("Iniciando LCD..");
  SetupLCD();
  
  Serial.println("Iniciando motor de passo..");
  mp.setSpeed(500);
   
  Serial.println("Iniciando demais pinos e buzzer..");
  // Define pino dos menus
  pinMode(BtnMenuEsquerda, INPUT);
  pinMode(BtnMenuDireita, INPUT);
  pinMode(BtnMenuSelect, INPUT);
  
  //Pino do Buzzer
  pinMode(BuzzerPin, OUTPUT);
  tone(BuzzerPin, BuzzerNote, 250);  
  Serial.println("Programa iniciado!");
}

void SetupServo(){
  servo_9.attach(ServoPin, 500, 2500);
  servo_9.write(90);
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

////////////////////////////////////////////////////////////////////////////////
// FUNÇÃO LOOP DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////
void loop(){  
  if (digitalRead(BtnMenuEsquerda) == HIGH || digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuSelect) == HIGH){
    Serial.println("Botão apertado!");
    tone(BuzzerPin, BuzzerNote, 10);
    ProcessaMenu();
    delay(250); 
  }
}

void ExecAlimentar(short tempoLigado){  
  servo_9.write(90); //Reseta Servo
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("Preparando pote1");
  Serial.println("Alimentando pote 1...");  
  tone(BuzzerPin, BuzzerNote, 500);
  for (pos = 90; pos <= 90+AnguloServo; pos += 1) {
    servo_9.write(pos);
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
    servo_9.write(pos);
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
    servo_9.write(pos);
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
