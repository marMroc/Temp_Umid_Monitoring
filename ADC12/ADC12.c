
#include <msp430.h>                             // Biblioteca do MSP430
#include "./ADC12/ADC12.h"                      // Subrotina da UART A0 e A1
#define amostras 20                             // Quantidade de amostras

unsigned long int ADC_Result = 0;               // Vetor de amostras do ADC
unsigned      int contAmostra = 0;              // Contador de amostras do ADC

void Configura_Conversor_ADC12(void){
    SYSCFG2  |= ADCPCTL1;                       // Configure ADC A1 pin

    ADCCTL0  |= ADCSHT_2 | ADCON;               // ADCON, S&H=16
    ADCCTL1  |= ADCSHP | ADCSSEL_1;             // ACLK
    ADCCTL2  |= ADCRES_1;                       // Resolução: 10-bit
    ADCMCTL0 |= ADCSREF_1 | ADCINCH_1;          // P1.1: A1; Vref=AVCC
    ADCIE    |= ADCIE0;                         // Enable ADC conv complete interrupt

    // Configure reference ADC
    PMMCTL0_H = PMMPW_H;                        // Unlock the PMM registers
    PMMCTL2  |= /*REFVSEL_0 | */INTREFEN;       // Enable internal reference
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE:        break;
        case ADCIV_ADCOVIFG:    break;
        case ADCIV_ADCTOVIFG:   break;
        case ADCIV_ADCHIIFG:    break;
        case ADCIV_ADCLOIFG:    break;
        case ADCIV_ADCINIFG:    break;
        case ADCIV_ADCIFG:      ADC_Result += ADCMEM0;                      //
                                if( (++contAmostra) >= amostras ){          //
                                    contAmostra = 0;                        // Zera contador
                                    ADC_Result = (ADC_Result/amostras);     //
                                    __bic_SR_register_on_exit(LPM3_bits);   // LPM3
                                }
                                else{
                                    ADCCTL0 |= ADCENC | ADCSC;              // Star conversion
                                }
                                break;

        default:                break;
    }
}

