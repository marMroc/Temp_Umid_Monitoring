
#include "./Conversions/Conv.h"                     // Utilizado no CCS
#include <msp430.h>                                 // Biblioteca do MSP430
#include <string.h>                                 //
#include <stdio.h>                                  //
#include <stdlib.h>                                 //
#include <stdint.h>                                 //
#include <math.h>                                   //

char *itoa_F( unsigned long int value, char *str ){
  char temp;
  unsigned long int i;
  for( i = 0; i < 20; i++ ){
    str[i] = 0;
  }

  unsigned long int digito;
  for( digito = 0, i = 0; value > 0; i++ ){
    digito = (value % 10);
    str[i] = digito + '0';
    value /= 10;
  }

  char j;
  for( j = strlen(str) - 1, i = 0; i < j; i++, j-- ){
    temp   = str[i];
    str[i] = str[j];
    str[j] = temp;
  }
  return str;
}

// Reverses a string 'str' of length 'len'
void reverse(char* str, int len){
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d){
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint){
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

