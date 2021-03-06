/*
 * qid_openssl_aes.c
 *
 *  Created on: Oct 21, 2012
 *      Author: ch
 */

#include <openssl/aes.h>

#include "std_hdr.h"
#include "comm_hdr.h"
#include "qid_openssl_aes.h"


int qid_aes_init( unsigned char *key_data, int key_data_len,
              unsigned char *salt,
              EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
  int i, nrounds = 5;
  unsigned char key[32], iv[32];

  /*
   * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
   * nrounds is the number of times the we hash the material. More rounds are more secure but
   * slower.
   */
  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt,
                     key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    QNAP_LOG_ERR_NL("Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

void qid_aes_clean_up(EVP_CIPHER_CTX *e)
{
  if(e)
    EVP_CIPHER_CTX_cleanup(e);
}

unsigned char *qid_aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{

  if(!e || !plaintext || !len){
    QNAP_LOG_ERR_NL("invalid parameter");
    return NULL;
  }

  /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  unsigned char *ciphertext = malloc(c_len);

  /* allows reusing of 'e' for multiple encryption cycles */
  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  /* update ciphertext with the final remaining bytes */
  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;

  //qnap_log_dump_data(2, BLUE, ciphertext, *len);

  return ciphertext;
}

unsigned char *qid_aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{

  if(!e || !ciphertext || !len){
    QNAP_LOG_ERR_NL("invalid parameter");
    return NULL;
  }

  /* because we have padding ON, we must allocate an extra cipher block size of memory */
  int p_len = *len, f_len = 0;
  unsigned char *plaintext = malloc(p_len + AES_BLOCK_SIZE);

  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;

  //qnap_log_dump_data(2, BLUE, plaintext, *len);

  return plaintext;
}
