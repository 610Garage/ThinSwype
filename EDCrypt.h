/* 
 * File:   EDCrypt.h
 * Author: 640bob
 *
 * Created on November 11, 2015, 11:12 PM
 */

#ifndef EDCRYPT_H
#define	EDCRYPT_H

#include <openssl/evp.h>

#define MAX_PASSWORD_LEGNTH 128

typedef struct{
    unsigned char key[EVP_MAX_KEY_LENGTH];
    int KL;
    unsigned char iv[EVP_MAX_IV_LENGTH];
    int IL;
} KEY;

bool keyGen(KEY *k, char * password);
int TS_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
int TS_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext);

#endif	/* EDCRYPT_H */

