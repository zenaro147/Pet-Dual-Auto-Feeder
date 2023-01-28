# Alimentador Automático para Pets
Sim, mais um projeto de alimentador automático. Porém o diferencial dele é que é possível alimentar 2 potes com o mesmo alimentador. Muito útil caso tenha 2 bichinhos que comem em potes separados.

## Componentes:
* Arduino Mega2560
* Modulo RTC DS1307
* Servo motor SG90
* Motor de Passo 28BYJ-48 + driver controle ULN2003 
* Display LCD 16x2 I2C
* 3 botões (push button/chave tactil)
* Módulo Conversor Step Down 6/24VDC Para 5V 3A
* Buzzer 5v Ativo (opcional)
* Rele T73 DC-12V 10A (Caso queira colocar uma fonte de energia alternativa)

## Montagem
**I2C LCD**
* VCC >> 5v
* GND >> GND
* SDA >> Pino 20
* SCL >> Pino 21

**RTC DS1307**
* VCC >> 5v
* GND >> GND
* SDA >> Pino 20
* SCL >> Pino 21 

**Buzzer**
* Negativo >> GND
* Positivo >> Pino 24

**Servo Motor**
* Fio Laranja >> Pino 9
* Fio Vermelho >> 5v
* Fio Marrom >> GND

**Motor de passo**
* In1 >> Pino 24
* In2 >> Pino 5
* In3 >> Pino 6
* In4 >> Pino 7

**3 Push Buttons (Chave Tactil)**
* GND e Pinos 28, 30, 32

![Esquema de ligação](/Schematic.jpg)

### To do
* ~~⚠Correção de bug onde o arduino trava após alguns minutos (mais ou menos uns 10min rodando direto)~~
* Possível otimização para executar usando um Arduino Uno? (O projeto começou a ser desenvolvido nele, porém alegou falta de memória, o que interfiria nas mensagens no LCD)
* Desenho completo do chassi para montagem
