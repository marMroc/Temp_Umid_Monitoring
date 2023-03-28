
#include <stdint.h>                                     //

extern char UCA0[];                                     // Vetor UCA0
extern unsigned long int indexUCA0;                     // Index vetor dados recebidos
extern unsigned char chegouDadosRX;                     // Chegou dados RX

void Configura_UART_A0_9600(void);                      // Scopo da subrotina UART A0
void Configura_UART_A0_19200(void);                     // Scopo da subrotina UART A0
void Configura_UART_A0_115200(void);                    // Scopo da subrotina UART A0
void Send_Data_UART_A0( char* STR);                     // Transmite dados UART A0
void Send_Data_UART_A0_STR( char* STR, char size );     // Transmite dados UART A0 Bin�rio

void Clean_Buffer(char *STR, unsigned long int size);   //

