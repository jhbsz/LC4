/*
 * qid_openssl_aes.h
 *
 *  Created on: Oct 21, 2012
 *      Author: ch
 */

#ifndef QID_OPENSSL_AES_H_
#define QID_OPENSSL_AES_H_


#include <openssl/evp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int qid_aes_init( unsigned char *key_data, int key_data_len, unsigned char *salt,
              EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);

/*
 * Clean the initialized cipher context up.
 */
void qid_aes_clean_up(EVP_CIPHER_CTX *e);

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *qid_aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *qid_aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);

#ifdef __cplusplus
}
#endif


#endif /* QID_OPENSSL_AES_H_ */
