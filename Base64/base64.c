 /*  [ LT ] 2020
  *  Simple Base64 encoding/decoding
  *
  *  Based on Base64 Encode and Decode in C by John Schember
  *  https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/
  *
  * This code is provided "AS IS" and may be modified or distributed under
  * the terms of the MIT license.
  *
  */

#include <string.h>
#include "base64.h"

static const char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const int base64_inverse_table[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56,
                                            57, 58, 59, 60, 61, -1, -1, -1, -1, -1,
                                            -1, -1,  0,  1,  2,  3,  4,  5,  6,  7,
                                             8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
                                            18, 19, 20, 21, 22, 23, 24, 25, -1, -1,
                                            -1, -1, -1, -1, 26, 27, 28, 29, 30, 31,
                                            32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                                            42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

size_t base64_encoded_size(size_t src_len)
{
    size_t result;

    result = src_len;
    if (src_len % 3 != 0)
        result += 3 - (src_len % 3);
    result /= 3;
    result *= 4;

    return result;
}

size_t base64_decoded_size(const char *src)
{
    size_t src_len;
    size_t result;
    size_t i;

    if (src == NULL)
        return 0;

    src_len = strlen(src);
    result = src_len / 4 * 3;

    for (i=src_len; i-->0; ) {
        if (src[i] == '=') {
            result--;
        } else {
            break;
        }
    }

    return result;
}

int b64char_is_valid(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c == '+' || c == '/' || c == '=')
        return 1;
    return 0;
}
/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @src_len: Length of the data to be encoded
 * @output: Pointer to encoded data output
 * Returns: error if == 0
 */
unsigned char base64_encode(unsigned char *src, unsigned char src_len, char *output){
    size_t i;
    size_t j;
    long int k;
    size_t  out_len;

    if (src == NULL || src_len == 0){  // check if the source is nul
        return 0;
    }

    out_len = base64_encoded_size(src_len);
    output[out_len] = '\0';

    for (i=0, j=0; i<src_len; i+=3, j+=4){
        k = src[i];
        k = i+1 < src_len ? k << 8 | src[i+1] : k << 8;
        k = i+2 < src_len ? k << 8 | src[i+2] : k << 8;

        output[j]   = base64_table[(k >> 18) & 0x3F];
        output[j+1] = base64_table[(k >> 12) & 0x3F];
        if (i+1 < src_len) {
            output[j+2] = base64_table[(k >> 6) & 0x3F];
        } else {
            output[j+2] = '=';
        }
        if (i+2 < src_len){
            output[j+3] = base64_table[k & 0x3F];
        } else {
            output[j+3] = '=';
        }
    }

    return 1;
}

/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @output: Pointer to decoded data output
 * Returns: Number of decoded characters
 *          error if == 0
 */
size_t base64_decode( char *src, char *output ){
    size_t src_len;
    size_t i;
    size_t j;
    long int k;
    char x = 0;

    if (src == NULL || output == NULL){
        return 0;
    }

    src_len = strlen(src);
//    if ((output_len < base64_decoded_size(src)) || ((src_len % 4) != 0))
//    {
//        return 0;
//    }

    for (i=0; i<src_len; i++) {
        if (!b64char_is_valid(src[i])) {
            return 0;
        }
    }

    for (i=0, j=0; i<src_len; i+=4, j+=3) {
        k = base64_inverse_table[src[i]-43];
        k = (k << 6) | base64_inverse_table[src[i+1]-43];
        k = src[i+2]=='=' ? k << 6 : (k << 6) | base64_inverse_table[src[i+2]-43];
        k = src[i+3]=='=' ? k << 6 : (k << 6) | base64_inverse_table[src[i+3]-43];

        output[j] = (k >> 16) & 0xFF;
        if (src[i+2] != '='){
            output[j+1] = (k >> 8) & 0xFF;
            x = 1;
        }
        if (src[i+3] != '='){
            output[j+2] = k & 0xFF;
            x = 2;
        }
    }

    return (j - x); // number of char
}
