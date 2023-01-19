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

////////////////
// Definições //
////////////////
//Define Servo
#define ServoPin 4
int pos = 0;
#define AnguloServo 45 //Servo irá virar 45 graus para poder bloquear uma saida e despejar comida em um pote
Servo servo_9;

//Define LCD I2C
int seconds = 0;
LiquidCrystal_I2C lcd_1(0x27, 16, 2);

//Define RTC
RTC_DS1307 rtc;

//Define Motor de passo
int passosPorVolta = 32;
Stepper mp(passosPorVolta, 8,10,9,11);

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

////////////////////////////////////////////////////////////////

#define BtnMenuEsquerda 13 //Define botão "Esquerda" do Menu
#define BtnMenuDireita 12  //Define botão "Direita" do Menu
#define BtnMenuSelect 7  //Define botão "Selecionar" do Menu

int NavigateMenuIndex = 0;

//Opcoes Menus
String MainMenuOptions[] = { 
  "Prog Horario  ",
  "Ligar Agora   ",
  "Reset Config  ",
  "Ler Config    ",
  "Salvar Config "
};

bool CheckProgHorarioAccess = false;
String ProgHorarioOptions[] = { 
  "Config Relogio",
  "Prog Horario 1",
  "Prog Horario 2",
  "Voltar        "
};

bool CheckConfigRelogioAccess = false;
String ConfigRelogioOptions[] = { 
  "Definir Hora  ",
  "Definir Min   ",
/*  "Definir Dia   ",
  "Definir Mes   ",
  "Definir Ano   ",*/
  "Voltar        "
};

bool CheckProgHorarioSet1Access = false;
bool CheckProgHorarioSet2Access = false;
String ProgHorarioSetOptions[] = { 
  "Ativar?       ",
  "Tempo(s)      ",
  "Prog Hora     ",
  "Prog Min      ",
  "Voltar        "
};

bool CheckAlimentarAgoraAccess = false;
String ProgAlimentarAgoraOptions[] = { 
  "Tempo(s)      ",
  "Ligar         ",
  "Voltar        "
};
////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(115200);

  Serial.println("Iniciando programa...");
  
  SetupServo();
  SetupLCD();
  mp.setSpeed(500);
  
  // Define pino dos menus
  pinMode(BtnMenuEsquerda, INPUT);
  pinMode(BtnMenuDireita, INPUT);
  pinMode(BtnMenuSelect, INPUT);
  
  //Pino Rele motor
  //pinMode(ReleControl, OUTPUT);
  //digitalWrite(ReleControl, LOW);
  
  //Pino do Buzzer
  pinMode(BuzzerPin, OUTPUT);
  Serial.println("Programa iniciado!");
  tone(BuzzerPin, BuzzerNote, 250);
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

void SetupServo(){
  servo_9.attach(ServoPin, 500, 2500);
  servo_9.write(90);
}

////////////////////////////////////////////////////////////////////////////////

void loop(){  
  if (digitalRead(BtnMenuEsquerda) == HIGH || digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuSelect) == HIGH){
    Serial.println("botao apertado");
    tone(BuzzerPin, BuzzerNote, 10);
    ProcessaMenu();
    delay(250); 
  }
}

void ExecAlimentar(short tempoLigado){
  servo_9.write(90); //Reseta Servo
  for (pos = 90; pos <= 90+AnguloServo; pos += 1) {
    servo_9.write(pos);
    delay(10);
  }
  tone(BuzzerPin, BuzzerNote, 500);
  
  //Alimenta o primeiro pote
  mp.step(2048);
  
  //Vira o Servo para alimentar o segundo pote
  for (pos = 90+AnguloServo; pos >= 90-AnguloServo; pos -= 1) {
    servo_9.write(pos);
    delay(10);
  }
  
  //Alimenta o segundo pote
  mp.step(-2048);
  
  //Reseta o Servo
  for (pos = 90-AnguloServo; pos <= 90; pos += 1) {
    servo_9.write(pos);
    delay(10);
  } 
  
  tone(BuzzerPin, BuzzerNote, 250);
  delay(500);
  tone(BuzzerPin, BuzzerNote, 250);
}

////////////////////////////////////////////////////////////////////////////////
// PROCESSAMENTO DA LOGICA DO MENU
////////////////////////////////////////////////////////////////////////////////
void ProcessaMenu(){
  if(!CheckProgHorarioAccess && (!CheckProgHorarioSet1Access && !CheckProgHorarioSet2Access && !CheckConfigRelogioAccess) && !CheckAlimentarAgoraAccess){
    //Se verdadeiro, então usuário está no menu principal
    if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
     NavegaMenu(MainMenuOptions, sizeof(MainMenuOptions)/sizeof(MainMenuOptions[0]));      
    }else if (digitalRead(BtnMenuSelect) == HIGH){
      switch(NavigateMenuIndex){
        case 0:
          AcessarMenu(&CheckProgHorarioAccess, ProgHorarioOptions);
          break;
        case 1:
          AcessarMenu(&CheckAlimentarAgoraAccess, ProgAlimentarAgoraOptions);
          ImprimeVlrVariavel(&tempoAlimentadorLigado[2]); //Imprime variavel do primeiro item do menu
          break;
        case 2:
          //Reseta dados para padrão e salva na EEPROM
          //ResetaMemoria();
          break;
        case 3:
          //Le dados salvos na EEPROM
          //LeMemoria();
          break;
        case 4:
          //Salva na EEPROM os horarios
          //GravaMemoria();
          break;
        default:
          break;
      }
    }
  }else{
    ////////////////////
    //FUNÇÕES SUBMENUS
    ////////////////////
      //MENU ALIMENTAR AGORA
      if(CheckAlimentarAgoraAccess){
        //Funcoes quando o usuário estiver no menu de Ligar Agora
        if(!CheckEditandoItem){
          if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
            NavegaMenu(ProgAlimentarAgoraOptions, sizeof(ProgAlimentarAgoraOptions)/sizeof(ProgAlimentarAgoraOptions[0]));
            //Imprime variáveis
            switch(NavigateMenuIndex){
              case 0:
                ImprimeVlrVariavel(&tempoAlimentadorLigado[2]);
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            switch(NavigateMenuIndex){
              case 0:
                if(!CheckEditandoItem){
                  CheckEditandoItem = true;
                  //Config Tempo em segundos que ficara ligado
                  ImprimeSinaisMenu();
                }
                break;
              case 1:
                //Ligar Maquina
                ExecAlimentar(tempoAlimentadorLigado[2]);
                break;
              case 2:
                AcessarMenu(&CheckAlimentarAgoraAccess, MainMenuOptions);
                break;
              default:
                break;
            }
          }
        }else{
          //Modo Edicao Item
          if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
            switch(NavigateMenuIndex){
              case 0:
                EditaItemMenu(&tempoAlimentadorLigado[2]);
                LimitaVariaveis(&tempoAlimentadorLigado[2],0,60);
                ImprimeVlrVariavel(&tempoAlimentadorLigado[2]);
                break;
              default:
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            if(CheckEditandoItem){
              CheckEditandoItem = false;
              ImprimeSetasMenu();
            }
          }
        }
      
      //MENU PROGRAMAR HORARIO
      }else if(CheckProgHorarioAccess && (!CheckProgHorarioSet1Access && !CheckProgHorarioSet2Access && !CheckConfigRelogioAccess)){
        //Funcoes quando o usuário estiver no menu de Programar Horarios
        if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
          NavegaMenu(ProgHorarioOptions, sizeof(ProgHorarioOptions)/sizeof(ProgHorarioOptions[0]));      
        }else if (digitalRead(BtnMenuSelect) == HIGH){
          switch(NavigateMenuIndex){
            case 0:
                //Config Relogio
              AcessarMenu(&CheckConfigRelogioAccess, ConfigRelogioOptions);
                break;
            case 1:
                //Programa Horario 1              
              AcessarMenu(&CheckProgHorarioSet1Access, ProgHorarioSetOptions);
                break;
            case 2:
                //Programa Horario 2
              AcessarMenu(&CheckProgHorarioSet2Access, ProgHorarioSetOptions);
                break;
            case 3:
              AcessarMenu(&CheckProgHorarioAccess, MainMenuOptions);
                break;
            default:
                break;
          }
        }
        
      //MENU CONFIGURAR RELOGIO
      }else if(CheckConfigRelogioAccess){
        //Funcoes quando o usuário estiver no menu de Setar Relogio
        if(!CheckEditandoItem){
          if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
            NavegaMenu(ConfigRelogioOptions, sizeof(ConfigRelogioOptions)/sizeof(ConfigRelogioOptions[0]));
            //Imprime variáveis
            switch(NavigateMenuIndex){
              case 0:
                ImprimeVlrVariavel(&dadosRTC[0]);
                break;
              case 1:
                ImprimeVlrVariavel(&dadosRTC[1]);
                break;
              /*case 2:
                ImprimeVlrVariavel(&dadosRTC[2]);
                break;
              case 3:
                ImprimeVlrVariavel(&dadosRTC[3]);
                break;
              case 4:
                lcd_1.setCursor(11,1);
                lcd_1.print(dadosRTC[4]);
                break;*/
            }
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            switch(NavigateMenuIndex){
              case 0: //Define Hora
              case 1: //Define Minuto
              /*case 2: //Define Dia
              case 3: //Define Mes
              case 4: //Define Ano*/
                  if(!CheckEditandoItem){
                    CheckEditandoItem = true;
                    //Config Tempo em segundos que ficara ligado
                    ImprimeSinaisMenu();
                  }
                  break;
              case 2:
                  AcessarMenu(&CheckConfigRelogioAccess, ProgHorarioOptions);
                  break;
              default:
                  break;
            }
          }
        }else{
          //Modo Edicao Item
          if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
            switch(NavigateMenuIndex){
              case 0: //Define Hora
                EditaItemMenu(&dadosRTC[0]);
                LimitaVariaveis(&dadosRTC[0],0,23);
                ImprimeVlrVariavel(&dadosRTC[0]);
                break;
              case 1: //Define Minuto
                EditaItemMenu(&dadosRTC[1]);
                LimitaVariaveis(&dadosRTC[1],0,59);
                ImprimeVlrVariavel(&dadosRTC[1]);
                break;
              /*case 2: //Define Dia
                EditaItemMenu(&dadosRTC[2]);                
                LimitaVariaveis(&dadosRTC[2],1,31);
                ImprimeVlrVariavel(&dadosRTC[2]);
                break;
              case 3: //Define Mes
                EditaItemMenu(&dadosRTC[3]);
                LimitaVariaveis(&dadosRTC[3],1,12);
                ImprimeVlrVariavel(&dadosRTC[3]);
                break;
              case 4: //Define Ano
                EditaItemMenu(&dadosRTC[4]);              
                LimitaVariaveis(&dadosRTC[4],2023,2099);
                lcd_1.setCursor(11,1);
                lcd_1.print(dadosRTC[4]);*/
              default:
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            if(CheckEditandoItem){
              CheckEditandoItem = false;
              ImprimeSetasMenu();
            }
          }
        }

      //MENU SETAR HORARIO 1 E 2
      }else if(CheckProgHorarioSet1Access || CheckProgHorarioSet2Access){
        //Funcoes quando o usuário estiver no menu de Programar Horarios de alimentação
        if(!CheckEditandoItem){
          if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
            NavegaMenu(ProgHorarioSetOptions, sizeof(ProgHorarioSetOptions)/sizeof(ProgHorarioSetOptions[0]));      
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            switch(NavigateMenuIndex){
              case 0: //Ativa temporizador especificado
              case 1: //Programa Tempo que ficara ligado (usado para "calibrar" a quantidade)
              case 2: //Programa Hora
              case 3: //Programa Minutos
                  if(!CheckEditandoItem){
                    CheckEditandoItem = true;
                    //Config Tempo em segundos que ficara ligado
                    ImprimeSinaisMenu();
                  }
                  break;
              case 4: 
              AcessarMenu(CheckProgHorarioSet1Access == true ? &CheckProgHorarioSet1Access : &CheckProgHorarioSet2Access, ProgHorarioOptions);
              break;
              default:
              break;
            }
          }
      }else{
        //Modo Edicao Item
        if (digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuEsquerda) == HIGH){
          switch(NavigateMenuIndex){
            case 0:
            break;
            default:
            break;
          }
        }else if (digitalRead(BtnMenuSelect) == HIGH){
          if(CheckEditandoItem){
            CheckEditandoItem = false;
            ImprimeSetasMenu();
          }
        }
      }
    }
  }
}
