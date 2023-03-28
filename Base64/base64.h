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

#ifndef BASE64_H
#define BASE64_H

size_t base64_encoded_size(size_t src_len);
size_t base64_decoded_size(const char *src);
int b64char_is_valid(char c);
unsigned char base64_encode(unsigned char *src, unsigned char src_len, char *output);
size_t base64_decode(char *src, char *output);

#endif /* BASE64_H */
