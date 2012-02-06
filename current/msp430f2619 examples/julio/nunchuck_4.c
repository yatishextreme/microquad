#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msp430x2619.h>
#include "lcd2112.h"
#include "uart0.h"


// Declara��o de pinos...
//
#define led         port3.out.pin0

#define sw1         port1.in.pin0
#define sw2         port1.in.pin1
#define sw3         port1.in.pin2
#define sw4         port1.in.pin3

#define sda_in      port3.in.pin1
#define sda_out     port3.out.pin1
#define scl         port3.out.pin2

#define dataout()   P3DIR = 0xd7
#define datain()    P3DIR = 0xd5


// Defini��es...
//
#define             no_lcd      0
#define             na_serial   1
#define             NACK        1
#define             ACK         0

#define             DS1338      0xd0
#define             TMP275      0x90
#define             _24LC512    0xa2
#define             SMART_CARD  0xa0

#define             i2c_TX      1
#define             i2c_RX      0


// Constantes e Vari�veis...
//
const unsigned char hexadecimal[16] = {"0123456789ABCDEF"};
unsigned char aonde = no_lcd;
unsigned char databuffer[6];


// Prot�tipos...
//
int putchar(int c);

void configura_cpu(void);
void delayus(unsigned int tempo);
void delayms(unsigned int tempo);

unsigned char readkeyboard(void);
unsigned char waitpress(void);
void waitrelease(void);
void waitkey(unsigned char key);
unsigned char waitanykey(void);

void config_i2c(unsigned char slave_add, unsigned char mode);
void config_slave(unsigned char slave_add);
unsigned char write_i2c(unsigned char *dado, unsigned char length);
unsigned char read_i2c(unsigned char *dado, unsigned char length);
unsigned char find_device(unsigned char address);


// M A I N
//
int main (void)
{
//  unsigned char buff[8];

  WDTCTL = WDTPW + WDTHOLD; 
  configura_cpu();
  lcd_initmodule(nao_virado);
  config_serial(baud_115200, mod_115200);

  config_i2c(0xa0, i2c_TX);
  printf("Searching...\n\r");

  // Smart Card...
  if(find_device(SMART_CARD) == ACK) printf("Found %02X\n\r", UCB0I2CSA << 1); else 
  {
    printf("Don't found %02X\n\r", UCB0I2CSA << 1);
    printf("Error: %02X \r\n", find_device(SMART_CARD));
  }
  // 24LC512...
  if(find_device(_24LC512) == ACK) printf("Found %02X\n\r", UCB0I2CSA << 1); else 
  {
    printf("Don't found %02X\n\r", UCB0I2CSA << 1);
    printf("Error: %02X \r\n", find_device(_24LC512));
  }
  // DS1338...
  if(find_device(DS1338) == ACK) printf("Found %02X\n\r", UCB0I2CSA << 1); else 
  {
    printf("Don't found %02X\n\r", UCB0I2CSA << 1);
    printf("Error: %02X \r\n", find_device(DS1338));
  }
  // TMP275...
  if(find_device(TMP275) == ACK) printf("Found %02X\n\r", UCB0I2CSA << 1); else 
  {
    printf("Don't found %02X\n\r", UCB0I2CSA << 1);
    printf("Error: %02X \r\n", find_device(TMP275));
  }

  while(1)
  {
  }    

  return 0;
}   

// Implementa��o do Printf...
//
int putchar(int c)
{
  if(aonde == no_lcd) lcd_putchar(c); else send_serial(c);
  return 0;
}

// Configura��o da CPU...
//
void configura_cpu(void)
{
  // configura o clock para 16MHz...
  DCOCTL = 0x77;
  BCSCTL1 = 0x0f;
  BCSCTL2 = 0x08;
  BCSCTL3 = 0x8c;

  // Configura as portas...
  P1DIR = 0xf0;
  P2DIR = 0x00;
  P3DIR = 0xd5;  
  P3SEL = 0x36;
  P4DIR = 0xff;
  P5DIR = 0x70;
  P5SEL = 0x70;

  // Apaga o led...
  led = 1;
}

// T = 0.2923us * tempo (aproximadamente...)
//
void delayus(unsigned int tempo)
{
    int i;
    
    for(i = 0; i < tempo; i++) nop();
}

// Aproximadamente 1ms...
//
void delayms(unsigned int tempo)
{
    int i;
    
    for(i = 0; i < tempo; i++) delayus(3420);
}

// Primitivas de Teclado...
//
unsigned char readkeyboard(void)
{
  unsigned char tecla = 0;
  if(sw1 == 0) tecla = '1';
  if(sw2 == 0) tecla = '2';
  if(sw3 == 0) tecla = '3';
  if(sw4 == 0) tecla = '4';

  return tecla;
}

unsigned char waitpress(void)
{
  unsigned char v = 0;
  while(v == 0) 
  {
    v = readkeyboard();
    delayms(1);
  }

  return v;
}

void waitrelease(void)
{
  while(readkeyboard() != 0) delayms(1);
}

void waitkey(unsigned char key)
{
  while(readkeyboard() != key) delayms(1);
  waitrelease();
}

unsigned char waitanykey(void)
{
  unsigned char v;

  v = waitpress();
  waitrelease();

  return v;
}

/*
// Primitivas de I2C...
//
void config_i2c_tx(unsigned char slave_add)
{
  // Configurando a porta I2C...
  //
  UCB0CTL1 = 0xc1;
  UCB0CTL0 = 0x0f;
  UCB0BR0 = 160;
  UCB0BR1 = 0;
  UCB0I2CSA = (slave_add/2);
  UCB0CTL1 = 0xc0;

  UCB0CTL1 |= UCTR+UCTXSTP;
  // Aguardando o stop condition...
  //
  while(!(UCB0CTL1 & UCTXSTP));  
}
*/

void config_i2c(unsigned char slave_add, unsigned char mode)
{
  // Configurando a porta I2C...
  //
  UCB0CTL1 = 0xc1;
  UCB0CTL0 = 0x0f;
  UCB0BR0 = 160;
  UCB0BR1 = 0;
  UCB0I2CSA = (slave_add/2);
  UCB0CTL1 = 0xc0;

  if(mode == i2c_TX) UCB0CTL1 |= UCTR+UCTXSTP; else UCB0CTL1 |= UCTXSTP;
  // Aguardando o stop condition...
  //
  while(!(UCB0CTL1 & UCTXSTP));  
}

void config_slave(unsigned char slave_add)
{
  UCB0I2CSA = (slave_add/2);
}





unsigned char read_i2c(unsigned char *dado, unsigned char length)
{
  unsigned char i = 0, status = 0;

  // Start i2c...(em tx mode!!!)
  //
  UCB0CTL1 |= UCTXSTT;
  while(!(UCB0CTL1 & UCTXSTT));

  // Envia a sequencia de caracteres, se houver...
  //
  while((!status) && (i<length))
  {  
    // Testa a ocorrencia de ACK/NACK dos dados...
    //
    while(UCB0STAT & UCBBUSY); 
    if(UCB0STAT & UCNACKIFG) status++;

    // Aguarda at� receber o caracter...
    //
    while(!(IFG2 & UCB0RXIFG)); 

    *(dado+i) = UCB0RXBUF;
    i++;
  }

  // Stop i2c...
  //
  UCB0CTL1 |= UCTXSTP;
  while(!(UCB0CTL1 & UCTXSTP));

  // Testa a ocorrencia de ACK/NACK dos dados...
  //
  while(UCB0STAT & UCBBUSY); 
  if(UCB0STAT & UCNACKIFG) status++;

  return status;
}




unsigned char write_i2c(unsigned char *dado, unsigned char length)
{
  unsigned char i = 0, status = 0;

  // Start i2c...(em tx mode!!!)
  //
  UCB0CTL1 |= UCTR + UCTXSTT;
  while(!(UCB0CTL1 & UCTXSTT));

  // Envia a sequencia de caracteres, se houver...
  //
  while((!status) && (i<length))
  {  
    UCB0TXBUF = *(dado+i);
    i++;

    // Aguarda at� enviar o caracter...
    //
    while(IFG2 & UCB0TXIFG); 

    // Testa a ocorrencia de ACK/NACK dos dados...
    //
    while(UCB0STAT & UCBBUSY); 
    if(UCB0STAT & UCNACKIFG) status++;
  }

  // Stop i2c...
  //
  UCB0CTL1 |= UCTR + UCTXSTP;
  while(!(UCB0CTL1 & UCTXSTP));

  // Testa a ocorrencia de ACK/NACK dos dados...
  //
  while(UCB0STAT & UCBBUSY); 
  if(UCB0STAT & UCNACKIFG) status++;

  return status;
}


// Primitivas do Dispositivo I2C
//
unsigned char find_device(unsigned char address)
{
  unsigned char v, buff[2];

  config_slave(address);
  v = write_i2c(buff, 0);
  return v;
}

