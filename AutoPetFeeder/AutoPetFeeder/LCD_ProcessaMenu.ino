////////////////////////////////////////////////////////////////////////////////
// PROCESSAMENTO DA LOGICA DO MENU
////////////////////////////////////////////////////////////////////////////////
void ProcessaMenu(){
  if(!CheckProgHorarioAccess && (!CheckProgHorarioSet1Access && !CheckProgHorarioSet2Access && !CheckConfigRelogioAccess) && !CheckAlimentarAgoraAccess){
    //Se verdadeiro, então usuário está no menu principal
    if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
     NavegaMenu(MainMenuOptions, sizeof(MainMenuOptions)/sizeof(MainMenuOptions[0]));      
    }else if (digitalRead(BtnMenuSelect) == estadoBotao){
      switch(NavigateMenuIndex){
        case 0:
          AcessarMenu(&CheckProgHorarioAccess, ProgHorarioOptions);
          NavigateMenuIndex = 0;
          break;
        case 1:
          AcessarMenu(&CheckAlimentarAgoraAccess, ProgAlimentarAgoraOptions);
          NavigateMenuIndex = 0;
          ImprimeVlrVariavel(&tempoAlimentadorLigado[2]); //Imprime variavel do primeiro item do menu
          break;
        case 2:
          //Reseta dados para padrão e salva na EEPROM
          ResetaMemoria();
          break;
        case 3:
          //Le dados salvos na EEPROM
          LeMemoria();
          break;
        case 4:
          //Salva na EEPROM os horarios
          GravaMemoria();
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
          if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
            NavegaMenu(ProgAlimentarAgoraOptions, sizeof(ProgAlimentarAgoraOptions)/sizeof(ProgAlimentarAgoraOptions[0]));
            //Imprime variáveis
            switch(NavigateMenuIndex){
              case 0:
                ImprimeVlrVariavel(&tempoAlimentadorLigado[2]);
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == estadoBotao){
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
          if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
            switch(NavigateMenuIndex){
              case 0:
                EditaItemMenu(&tempoAlimentadorLigado[2]);
                LimitaVariaveis(&tempoAlimentadorLigado[2],0,60);
                ImprimeVlrVariavel(&tempoAlimentadorLigado[2]);
                break;
              default:
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == estadoBotao){
            if(CheckEditandoItem){
              CheckEditandoItem = false;
              ImprimeSetasMenu();
            }
          }
        }
      
      //MENU PROGRAMAR HORARIO
      }else if(CheckProgHorarioAccess && (!CheckProgHorarioSet1Access && !CheckProgHorarioSet2Access && !CheckConfigRelogioAccess)){
        //Funcoes quando o usuário estiver no menu de Programar Horarios
        if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
          NavegaMenu(ProgHorarioOptions, sizeof(ProgHorarioOptions)/sizeof(ProgHorarioOptions[0]));      
        }else if (digitalRead(BtnMenuSelect) == estadoBotao){
          switch(NavigateMenuIndex){
            case 0:
                //Config Relogio
              AcessarMenu(&CheckConfigRelogioAccess, ConfigRelogioOptions);
              NavigateMenuIndex = 0;
              ImprimeVlrVariavel(&dadosRTC[0]); //Imprime variavel do primeiro item do menu
                break;
            case 1:
              //Programa Horario 1              
              AcessarMenu(&CheckProgHorarioSet1Access, ProgHorarioSetOptions);
              NavigateMenuIndex = 0;
              lcd_1.setCursor(12,1);
              if(acionarTimerAlimentador[0] == 1){
                lcd_1.print("Sim");
              }else{
                lcd_1.print("Nao");
              }
                break;
            case 2:
              //Programa Horario 2
              AcessarMenu(&CheckProgHorarioSet2Access, ProgHorarioSetOptions);
              ImprimeVlrVariavel(&dadosRTC[0]); //Imprime variavel do primeiro item do menu
              NavigateMenuIndex = 0;
              lcd_1.setCursor(12,1);
              if(acionarTimerAlimentador[1] == 1){
                lcd_1.print("Sim");
              }else{
                lcd_1.print("Nao");
              }
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
          if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
            NavegaMenu(ConfigRelogioOptions, sizeof(ConfigRelogioOptions)/sizeof(ConfigRelogioOptions[0]));
            //Imprime variáveis
            switch(NavigateMenuIndex){
              case 0:
                ImprimeVlrVariavel(&dadosRTC[0]);
                break;
              case 1:
                ImprimeVlrVariavel(&dadosRTC[1]);
                break;
              case 2:
                ImprimeVlrVariavel(&dadosRTC[2]);
                break;
              case 3:
                ImprimeVlrVariavel(&dadosRTC[3]);
                break;
              case 4:
                lcd_1.setCursor(11,1);
                lcd_1.print(dadosRTC[4]);
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == estadoBotao){
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
          if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
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
              case 2: //Define Dia
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
                lcd_1.print(dadosRTC[4]);
              default:
                break;
            }
          }else if (digitalRead(BtnMenuSelect) == estadoBotao){
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
          if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
            NavegaMenu(ProgHorarioSetOptions, sizeof(ProgHorarioSetOptions)/sizeof(ProgHorarioSetOptions[0]));
            //Imprime variáveis
            if(CheckProgHorarioSet1Access){
              switch(NavigateMenuIndex){
                case 0:
                  lcd_1.setCursor(12,1);
                  if(acionarTimerAlimentador[0] == 1){
                    lcd_1.print("Sim");
                  }else{
                    lcd_1.print("Nao");
                  }
                  break;
                case 1:
                  ImprimeVlrVariavel(&tempoAlimentadorLigado[0]);
                  break;
                case 2:
                  ImprimeVlrVariavel(&dadosTimer[0]);
                  break;
                case 3:
                  ImprimeVlrVariavel(&dadosTimer[1]);
                  break;
              }
            }else{
              switch(NavigateMenuIndex){
                case 0:
                  lcd_1.setCursor(12,1);
                  if(acionarTimerAlimentador[1] == 1){
                    lcd_1.print("Sim");
                  }else{
                    lcd_1.print("Nao");
                  }
                  break;
                case 1:
                  ImprimeVlrVariavel(&tempoAlimentadorLigado[1]);
                  break;
                case 2:
                  ImprimeVlrVariavel(&dadosTimer[2]);
                  break;
                case 3:
                  ImprimeVlrVariavel(&dadosTimer[3]);
                  break;
              }
            }
            
          }else if (digitalRead(BtnMenuSelect) == estadoBotao){
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
        if (digitalRead(BtnMenuDireita) == estadoBotao || digitalRead(BtnMenuEsquerda) == estadoBotao){
          if(CheckProgHorarioSet1Access){
            switch(NavigateMenuIndex){
              case 0:
                  EditaItemMenu(&acionarTimerAlimentador[0]);
                  LimitaVariaveis(&acionarTimerAlimentador[0],0,1);
                  lcd_1.setCursor(12,1);
                  if(acionarTimerAlimentador[0] == 1){
                    lcd_1.print("Sim");
                  }else{
                    lcd_1.print("Nao");
                  }
              break;
              case 1:
                  EditaItemMenu(&tempoAlimentadorLigado[0]);
                  LimitaVariaveis(&tempoAlimentadorLigado[0],0,60);
                  ImprimeVlrVariavel(&tempoAlimentadorLigado[0]);
              break;
              case 2:
                  EditaItemMenu(&dadosTimer[0]);
                  LimitaVariaveis(&dadosTimer[0],0,23);
                  ImprimeVlrVariavel(&dadosTimer[0]);
              break;
              case 3:
                  EditaItemMenu(&dadosTimer[1]);
                  LimitaVariaveis(&dadosTimer[1],0,59);
                  ImprimeVlrVariavel(&dadosTimer[1]);
              break;
              default:
              break;
            }
          }else{
            switch(NavigateMenuIndex){
              case 0:
                  EditaItemMenu(&acionarTimerAlimentador[1]);
                  LimitaVariaveis(&acionarTimerAlimentador[1],0,1);
                  lcd_1.setCursor(12,1);
                  if(acionarTimerAlimentador[1] == 1){
                    lcd_1.print("Sim");
                  }else{
                    lcd_1.print("Nao");
                  }
              break;
              case 1:
                  EditaItemMenu(&tempoAlimentadorLigado[1]);
                  LimitaVariaveis(&tempoAlimentadorLigado[1],0,60);
                  ImprimeVlrVariavel(&tempoAlimentadorLigado[1]);
              break;
              case 2:
                  EditaItemMenu(&dadosTimer[2]);
                  LimitaVariaveis(&dadosTimer[2],0,23);
                  ImprimeVlrVariavel(&dadosTimer[2]);
              break;
              case 3:
                  EditaItemMenu(&dadosTimer[3]);
                  LimitaVariaveis(&dadosTimer[3],0,59);
                  ImprimeVlrVariavel(&dadosTimer[3]);
              break;
              default:
              break;
            }
          }
          
        }else if (digitalRead(BtnMenuSelect) == estadoBotao){
          if(CheckEditandoItem){
            CheckEditandoItem = false;
            ImprimeSetasMenu();
          }
        }
      }
    }
  }
}
