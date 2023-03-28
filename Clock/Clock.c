

#include <msp430.h>                                 // Biblioteca do MSP430
#include "./Clock/Clock.h"                          // Subrotina da UART A0 e A1

void Configura_Clock( void ){
  PM5CTL0 &= ~LOCKLPM5;                       // Senha de acesso dos pinos
  FRCTL0 = FRCTLPW | NWAITS_1;                // operation beyond 8MHz _before_ configuring the clock system.

  P2SEL1 |= BIT0 | BIT1;                      // set XT1 pin as second function
  //do{                                       //
  // CSCTL7 &= ~(XT1OFFG | DCOFFG);           // Clear XT1 and DCO fault flag
  // SFRIFG1 &= ~OFIFG;                       //
  //}while (SFRIFG1 & OFIFG);                 // Test oscillator fault flag

  __bis_SR_register(SCG0);                    // disable FLL
  CSCTL3 |= SELREF__XT1CLK;                   // Set XT1 as FLL reference source
  CSCTL0 = 0;                                 // clear DCO and MOD registers
  CSCTL1 &= ~(DCORSEL_7);                     // Clear DCO frequency select bits first
  CSCTL1 |= DCORSEL_5;                        // Set DCO = 16MHz
  CSCTL2 = FLLD_0 | 487;                      // DCOCLKDIV = 16MHz
  CSCTL5 = DIVS_1 | DIVM_0;                   // SMCLK/2 = 8 MHz e MCLK/1 = 16 MHz
  __delay_cycles(3);                          //
  __bic_SR_register(SCG0);                    // enable FLL
  while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));  // FLL locked

  CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 (~32768Hz) as ACLK source, ACLK = 32768Hz
}                                               // default DCOCLKDIV as MCLK and SMCLK source


//void Configura_Clock_DCO_1MHz(void){
//    CSCTL0 = 0;                                     // clear DCO and MOD registers
//    CSCTL1 = DCORSEL_0;                             // DCO = 1MHz
//    CSCTL2 = FLLD_0 + 30;                           // DCOCLKDIV = ~1MHz
//    __delay_cycles(3);
//}
//
//void Configura_Clock_DCO_16MHz(void){
//    CSCTL0 = 0;                                     // clear DCO and MOD registers
//    CSCTL1 = DCORSEL_5;                             // DCO = 16MHz
//    CSCTL2 = FLLD_0 + 487;                          // DCOCLKDIV = 16MHz
//    __delay_cycles(3);
//}

