

#include <msp430.h>                                 // Biblioteca do MSP430
#include "./Timer/Timer.h"                          // Subrotina da Timer


unsigned long int ContadorSegundos = 1;             // Tempo de funcionamento
unsigned char     flagContaTempoTotal = 1;          // Controle de incremento Tempo Total de funcionamento

void Configura_Timer_A0(void){
    TA0CCTL0 = OUTMOD_0;                            // LCD - Setup serial tx I/O

    //TA0CCTL0 = CCIE;                              // Habilita interrupção do Timer A0-CCR0
    //TA0CCR0 = 4096;                               // Conta xx segundos

    TA0CCTL1 &= ~CCIE;                              // Habilita interrupção do Timer A0-CCR1
    TA0CCR1 = 25000;                                // Conta xx segundos

    TA0CCTL2 &= ~CCIE;                              // Habilita interrupção do Timer A0-CCR2
    TA0CCR2 = 50000;                                // Conta xx segundos

    TA0CTL = TASSEL_2 | MC_2 | ID_0;                // SMCLK, Up to CCRx e Div. por 1
}

//#pragma vector = TIMER0_A0_VECTOR
//__interrupt void Timer0_A0(void){
//    TA0CCR0 += 4096;                              // Conta mais 1 segundos
//    __bic_SR_register_on_exit(LPM0_bits);         // Acorda LPMx
//}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void){
    switch( TA0IV ){
        case  2: TA0CCR1 += 25000;                      // Conta xx segundos
                 __bic_SR_register_on_exit(LPM0_bits);  // Acorda LPMx
                 break;                                 //

        case  4: TA0CCR2 += 50000;                      // Conta xx segundos
                 __bic_SR_register_on_exit(LPM0_bits);  // Acorda LPMx
                 break;                                 //

        default: break;
    }
}

void Configura_Timer_A1(void){
    TA1CCTL0 &= ~CCIE;                              // Habilita interrupção do Timer A1-CCR0
    TA1CCR0 = 4096;                                 // Conta xx segundos

    TA1CCTL1 = CCIE;                              // Habilita interrupção do Timer A1-CCR1
    TA1CCR1 = 4096;                                 // Conta xx segundos

    TA1CCTL2 &= ~CCIE;                              // Habilita interrupção do Timer A1-CCR2
    TA1CCR2 = 4096;                                 // Conta xx segundos

    TA1CTL = TASSEL_1 | MC_2 | ID_3;                // ACLK, Up to CCRx e Div. por 8
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void){
  TA1CCR0 += 4096;                                      // Conta mais 1 segundos
  __bic_SR_register_on_exit(LPM0_bits);                 // Acorda LPMx
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A12(void){
    switch( TA1IV ){
        case  2: TA1CCR1 += 4096;                       // Conta xx segundos
                ContadorSegundos++;                     //
                if( flagContaTempoTotal == 1 ){         //
                  __bic_SR_register_on_exit(LPM3_bits); // Exit and active CPU
                }
                 break;                                 //

        case  4: //TA1CCR2 += 4096;                     // Conta xx segundos
                TA1CCTL2 &= ~CCIE;                      // Desabilita interrupção do Timer A1-CCR2
                 __bic_SR_register_on_exit(LPM0_bits);  // Acorda LPMx
                 break;                                 //

        default: break;
    }
}

void Spends_Time_Per_ms(unsigned long int tempo){
  TA1CCTL2 = CCIE;                                      // Habilita interrupção do Timer A2-CCR0
  TA1CCR2 = (TA1R+(unsigned int)((tempo*4096.0)/1000.0));// Conta [tempo] segundos
  __bis_SR_register(LPM3_bits);                         // Dorme por ~x.y ms
}
