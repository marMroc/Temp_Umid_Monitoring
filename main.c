#include <stdbool.h>                            // 
#include <msp430.h>                             // 

//---------------------------> Bibliotecas <-----------------------
#include "./Conversions/Conv.c"                 // Utilizado no IAR
//#include "./Conversions/Conv.h"               // Utilizado no CCS

#include "./I2C/I2C.c"                          // Utilizado no IAR
//#include "./I2C/I2C.h"                        // Utilizado no CCS

#include "./Clock/Clock.c"                      // Utilizado no IAR
//#include "./Clock/Clock.h"                    // Utilizado no CCS

#include "./UART/UART.c"                        // Utilizado no IAR
//#include "./UART/UART.h"                      // Utilizado no CCS

#include "./Base64/base64.c"                    // Utilizado no IAR
//#include "./Base64/base64.h"                  // Utilizado no CCS

char text[10] = {0};                            // 


void Clean_Buffer(char *STR, unsigned long int size){
  unsigned long int indexBL = 0;                // Index str
  for( indexBL = 0;  indexBL < size ;indexBL++ ){// Condição de parada
    STR[indexBL] = '\0';                        // Limpa buffer
  }
}

int main( void ){
  WDTCTL = WDTPW | WDTHOLD;                     // Halt the watchdog timer

  Configura_Clock();                            // Initialization

  

  __bis_SR_register(GIE);                       // Enable interrupts

  P1DIR |=  BIT1;                               // Alimentação: BME ou HDC
  P1OUT |=  BIT1;                               // VCC_BME2 ou HDC-1080
  Configura_HDC1080(HDC_1080_ADD);              // Sensor: Temperatura e Umidade

  while( 1 ){
    Read_HDC1080(HDC_1080_ADD);                 // HDC-1080: Temperatura e Umidade
    
//    ftoa(temperature, text, 2);               // Converte float to str
//    Send_Data_UART_A0("Temp = \0");           // 
//    Send_Data_UART_A0_STR(text, 5);           //        
//    Send_Data_UART_A0(" \0");                 // 
//
//    ftoa(humidity, text, 2);                  // Converte float to str
//    Send_Data_UART_A0("Umid = \0");           // 
//    Send_Data_UART_A0_STR(text, 5);           //
//    Send_Data_UART_A0("\r\n\0");              // 
    _NOP();
    _NOP();
    __delay_cycles(16000000);                   //
  }
}
