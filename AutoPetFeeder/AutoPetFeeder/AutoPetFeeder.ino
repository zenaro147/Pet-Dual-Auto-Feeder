#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

//Define Servo
#define ServoPin 2
int pos = 0;
Servo servo_9;

//Define LCD I2C
int seconds = 0;
Adafruit_LiquidCrystal lcd_1(0);

//Variavel Inputs/Outputs
#define ReleControl 12 //Pino de controle do Rele

//Tempo em segundos que o motor ficará ligado jogando comida
short tempoAlimentadorLigado[3] = {5,5,5}; //Timer1, Timer2, Manual

//Hora e Minuto dos timers
short horaTimer1 = 0;
short minutoTimer1 = 0;
short horaTimer2 = 0;
short minutoTimer2 = 0;

//Data e Hora do RTC
short horaRTC = 0;
short minutoRTC = 0;
short diaRTC = 1;
short mesRTC = 1;
short anoRTC = 2023;

//Pino Buzzer
#define BuzzerPin 11

//Variavel para "travar" o Menu
bool CheckEditandoItem = false;

////////////////////////////////////////////////////////////////

#define BtnMenuEsquerda 8 //Define botão "Esquerda" do Menu
#define BtnMenuDireita 7  //Define botão "Direita" do Menu
#define BtnMenuSelect 4   //Define botão "Selecionar" do Menu

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
  "Definir Dia   ",
  "Definir Mes   ",
  "Definir Ano   ",
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
  
  SetupServo();
  SetupLCD();
  
  // Define pino dos menus
  pinMode(BtnMenuEsquerda, INPUT);
  pinMode(BtnMenuDireita, INPUT);
  pinMode(BtnMenuSelect, INPUT);
  
  //Pino Rele motor
  pinMode(ReleControl, OUTPUT);
  digitalWrite(ReleControl, LOW);
  
  //Pino do Buzzer
  pinMode(BuzzerPin, OUTPUT);
  noTone(BuzzerPin);
}

void SetupLCD(){
  lcd_1.begin(16, 2); //Display 16x2
  lcd_1.setBacklight(1);
  lcd_1.print("Alimentador Pet");
  ImprimeSetasMenu();
  lcd_1.setCursor(1,1);
  lcd_1.print(MainMenuOptions[0]);
}

void SetupServo(){
  servo_9.attach(ServoPin, 500, 2500);
  servo_9.write(0);
}

////////////////////////////////////////////////////////////////////////////////

void loop()
{  
  if (digitalRead(BtnMenuEsquerda) == HIGH || digitalRead(BtnMenuDireita) == HIGH || digitalRead(BtnMenuSelect) == HIGH){
    tone(BuzzerPin, 495, 10);
    ProcessaMenu();
    delay(250); 
  }  
}

void ExecAlimentar(int tempoLigado){
  servo_9.write(0); //Reseta Servo
  
  tone(BuzzerPin, 495, 500);
  
  //Alimenta o primeiro pote
  digitalWrite(ReleControl, HIGH);
  delay(tempoLigado*1000);
  digitalWrite(ReleControl, LOW);
  
  //Vira o Servo para alimentar o segundo pote
  for (pos = 0; pos <= 90; pos += 1) {
    servo_9.write(pos);
    delay(10);
  }
  
  //Alimenta o segund0 pote
  digitalWrite(ReleControl, HIGH);
  delay(tempoLigado*1000);  
  digitalWrite(ReleControl, LOW);
  
  //Reseta o Servo
  for (pos = 90; pos >= 0; pos -= 1) {
    servo_9.write(pos);
    delay(10);
  } 
  
  tone(BuzzerPin, 495, 250);
  delay(500);
  tone(BuzzerPin, 495, 250);
  
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
          break;
        case 3:
          //Le dados salvos na EEPROM
          break;
        case 4:
          //Salva na EEPROM os horarios
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
                if(tempoAlimentadorLigado[2] < 0){
                  tempoAlimentadorLigado[2] = 0;
                }
                if(tempoAlimentadorLigado[2] > 60){
                  tempoAlimentadorLigado[2] = 60;
                }
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
                ImprimeVlrVariavel(&horaRTC);
                break;
              case 1:
                ImprimeVlrVariavel(&minutoRTC);
                break;
              case 2:
                ImprimeVlrVariavel(&diaRTC);
                break;
              case 3:
                ImprimeVlrVariavel(&mesRTC);
                break;
              case 4:
                lcd_1.setCursor(11,1);
                lcd_1.print(anoRTC);
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == HIGH){
            switch(NavigateMenuIndex){
              case 0: //Define Hora
              case 1: //Define Minuto
              case 2: //Define Dia
              case 3: //Define Mes
              case 4: //Define Ano
                  if(!CheckEditandoItem){
                    CheckEditandoItem = true;
                    //Config Tempo em segundos que ficara ligado
                    ImprimeSinaisMenu();
                  }
                  break;
              case 5:
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
                EditaItemMenu(&horaRTC);
                if(horaRTC < 0){
                  horaRTC = 0;
                }
                if(horaRTC > 59){
                  horaRTC = 59;
                }
                ImprimeVlrVariavel(&horaRTC);
                break;
              case 1: //Define Minuto
                EditaItemMenu(&minutoRTC);
                if(minutoRTC < 0){
                  minutoRTC = 0;
                }
                if(minutoRTC > 59){
                  minutoRTC = 59;
                }
                ImprimeVlrVariavel(&minutoRTC);
                break;
              case 2: //Define Dia
                EditaItemMenu(&diaRTC);
                if(diaRTC < 1){
                  diaRTC = 1;
                }
                if(diaRTC > 31){
                  diaRTC = 31;
                }
                ImprimeVlrVariavel(&diaRTC);
                break;
              case 3: //Define Mes
                EditaItemMenu(&mesRTC);
                if(mesRTC < 1){
                  mesRTC = 1;
                }
                if(mesRTC > 12){
                  mesRTC = 12;
                }
                ImprimeVlrVariavel(&mesRTC);
                break;
              case 4: //Define Ano
              
                if (digitalRead(BtnMenuDireita) == HIGH){
                  anoRTC++;
                }
                if (digitalRead(BtnMenuEsquerda) == HIGH){
                  anoRTC--;
                }
              
                if(anoRTC < 2023){
                  anoRTC = 2023;
                }
                if(anoRTC > 2099){
                  anoRTC = 2099;
                }
              
                lcd_1.setCursor(11,1);
                lcd_1.print(anoRTC);
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
