////////////////////////////////////////////////////////////////////////////////
// DEFINIÇÃO DE VARIAVEIS E PINOS DO PROGRAMA
////////////////////////////////////////////////////////////////////////////////
#define DEBUG_MODE                //HABILITA OPÇÕES DE DEBUG NO SERIAL PARA MONITORAMENTO

//Botões do menu
#define BtnMenuEsquerda 32        //Define botão "Esquerda" do Menu
#define BtnMenuDireita 30         //Define botão "Direita" do Menu
#define BtnMenuSelect 28          //Define botão "Selecionar" do Menu
#define estadoBotao LOW           //Define estado que o botão tem que estar para ser reconhecido (Padrão: LOW)

//Servo motor
#define ServoPin 22
#define AnguloServo 45            //Servo irá virar 45 graus para poder bloquear uma saida e despejar comida em um pote
#define AnguloPadraoServo 90      //Angulo padrão do Servo quando ele estiver parado

//Tela LCD
#define enderecoLCD 0x27          //Pode ser necessário mudar esse valor. Esse link contem um scanner I2C que pode ajudar a localizar o valor do seu LCD 
#define LCDqtdeColunas 16          //Pode ser necessário mudar esse valor. Esse link contem um scanner I2C que pode ajudar a localizar o valor do seu LCD 
#define LCDqtdeLinhas 2          //Pode ser necessário mudar esse valor. Esse link contem um scanner I2C que pode ajudar a localizar o valor do seu LCD 
#define timeoutLCD 10             //Tempo máximo que a tela ficará ligada

//Módulo RTC
#define intervaloAttRelogio 5     //Intervalo em segundos da atualização do relogio

//Buzzer
#define BuzzerPin 23              //Pino usado pelo Buzzer
#define BuzzerNote 698            //Nota que buzzer irá fazer

//Motor de Passo
#define passosPorVolta 32         //Passos por volta do motor de passo (Padrão: 32)
#define velocidadeMotor 500       //Velocidade do motor de passo (Padrão: 500)
#define PinoIN1 24                //Pino Digital que a entrada IN1 foi ligada
#define PinoIN2 5                 //Pino Digital que a entrada IN2 foi ligada
#define PinoIN3 6                 //Pino Digital que a entrada IN3 foi ligada
#define PinoIN4 7                 //Pino Digital que a entrada IN4 foi ligada
#define InverterIN2IN3            //Em alguns casos, pode ser necessário ativar essa linha para o motor funcionar
