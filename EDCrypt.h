/* 
 * File:   EDCrypt.h
 * Author: 640bob
 *
 * Created on November 11, 2015, 11:12 PM
 */

#ifndef EDCRYPT_H
#define	EDCRYPT_H

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext);
int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext);

#endif	/* EDCRYPT_H */

