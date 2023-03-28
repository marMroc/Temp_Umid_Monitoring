

#include <msp430.h>                                 // Biblioteca do MSP430
#include "./UART/UART.h"                            // Subrotina da UART A0 e A1
#include <stdint.h>                                 //

#define TM 50                                       // Quantidade de caracteres recebidos

char UCA0[TM];                                      // Vetor UCA0
unsigned long int indexUCA0 = 0;                    // Index vetor dados recebidos
unsigned char chegouDadosRX = 0;                    // Chegou dados RX


void Configura_UART_A0_9600(void){
    P1SEL0    |=   BIT4 | BIT5;                 //
    P1SEL1    &= ~(BIT4 | BIT5);                //

    UCA0CTLW0  =  UCSWRST;                      //
    UCA0CTLW0 |=  UCSSEL_1;                     //
    UCA0BR0    =  3;                            //
    UCA0BR1    =  0x00;                         // BR0 e BR1 = 16MHz/(16*9600) = 104,16
    UCA0MCTLW  =  0x9200;                       // UCBRF_3 | UCOS16
    UCA0CTLW0 &= ~UCSWRST;                      //
    UCA0IE     =  UCRXIE;                       //
}//http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html

void Configura_UART_A0_19200(void){
    P1SEL0    |=   BIT4 | BIT5;                     //
    P1SEL1    &= ~(BIT4 | BIT5);                    //

    UCA0CTLW0  =  UCSWRST;                          //
    UCA0CTLW0 |=  UCSSEL_2;                         //
    UCA0BR0    =  52;                               //
    UCA0BR1    =  0x00;                             // BR0 e BR1 = 16MHz/(16*19200) = 52,083
    UCA0MCTLW  =  UCBRF_1 | UCOS16;                 //
    UCA0CTLW0 &= ~UCSWRST;                          //
    UCA0IE     =  UCRXIE;                           //
}//http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html

void Configura_UART_A0_115200(void){
    P1SEL0    |=   BIT4 | BIT5;                     //
    P1SEL1    &= ~(BIT4 | BIT5);                    //

    UCA0CTLW0  =  UCSWRST;                          //
    UCA0CTLW0 |=  UCSSEL_2;                         //
    UCA0BR0    =  8;                               //
    UCA0BR1    =  0x00;                             // BR0 e BR1 = 16MHz/(16*115200) = 52,083
    UCA0MCTLW  =  UCBRF_11 | UCOS16;                 //
    UCA0CTLW0 &= ~UCSWRST;                          //
    UCA0IE     =  UCRXIE;                           //
}//http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR( void ){
  switch (__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG)){
    case USCI_UART_UCRXIFG:
                            UCA0[indexUCA0] = UCA0RXBUF;
                            //UCA0TXBUF = UCA0[indexUCA0];
                            if( UCA0[indexUCA0] == '\n' ){
                                if( UCA0[indexUCA0-1] == '\r' ){
                                    indexUCA0 = 0;
                                    chegouDadosRX = 1;
                                  __bic_SR_register_on_exit(LPM3_bits);     // Limpa LPM3
                                }
                                else{
                                    indexUCA0++;
                                }
                            }
                            else{
                                if( UCA0[indexUCA0] == '$' ){
                                    Clean_Buffer(UCA0, TM);
                                    UCA0[0] = '$';
                                    indexUCA0 = 1;
                                }
                                else{
                                    indexUCA0++;
                                    if( indexUCA0 > TM ){
                                        indexUCA0 = 0;
                                    }
                                }
                            }
                            __no_operation();
                            break;

    default:                break;
  }
}

void Send_Data_UART_A0( char* STR){
    unsigned char index = 0;                            //
    for( index = 0; STR[index] != '\0'; index++){       //
        while( !(UCA0IFG & UCTXIFG) );                  //
        UCA0TXBUF = STR[index];                         //
    }
    //__no_operation();                                 // NOP
}

void Send_Data_UART_A0_STR( char* STR, char size ){
    unsigned char index = 0;                            //
    for( index = 0; index < size; index++){             //
        while( !(UCA0IFG & UCTXIFG) );                  //
        UCA0TXBUF = STR[index];                         //
    }
    //__no_operation();                                 // NOP
}

