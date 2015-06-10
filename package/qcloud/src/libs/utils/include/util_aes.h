/*
 * openssl_aes.h
 *
 *  Created on: Jan 21, 2013
 *      Author: ch
 */

#ifndef UTIL_AES_H_
#define UTIL_AES_H_

#include <openssl/evp.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
int util_aes_init( unsigned char *key_data, int key_data_len, unsigned char *salt,
                   EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);

/*
 * Clean the initialized cipher context up.
 */
void util_aes_clean_up(EVP_CIPHER_CTX *e);

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *util_aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *util_aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);

void util_aes_decrypt_str(EVP_CIPHER_CTX *aes_dec_ctx, char *str_buff,
                          size_t buff_size);

char * util_aes_encrypt_str(EVP_CIPHER_CTX *aes_enc_ctx, char *src);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_AES_H_ */
