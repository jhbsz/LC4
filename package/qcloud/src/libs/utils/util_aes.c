/*
 * openssl_aes.c
 *
 *  Created on: Jan 21, 2013
 *      Author: ch
 */

#include <openssl/aes.h>

#include "std_hdr.h"
#include "comm_hdr.h"
#include "util_aes.h"


int util_aes_init( unsigned char *key_data, int key_data_len,
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

  if(e_ctx){
    EVP_CIPHER_CTX_init(e_ctx);
    EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  }

  if(d_ctx){
    EVP_CIPHER_CTX_init(d_ctx);
    EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  }

  return 0;
}

void util_aes_clean_up(EVP_CIPHER_CTX *e)
{
  if(e)
    EVP_CIPHER_CTX_cleanup(e);
}

unsigned char *util_aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
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

unsigned char *util_aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
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

void util_aes_decrypt_str(EVP_CIPHER_CTX *aes_dec_ctx, char *str_buff,
                          size_t buff_size)
{

  if(str_buff && aes_dec_ctx){
    int len = strlen(str_buff);
    uint8_t *cipher_text = NULL;
    uint8_t *plain_text = NULL;

    if(len == 0 || buff_size == 0){
      str_buff[0] = '\0';
      return;
    }
    cipher_text = malloc(len);
    if(cipher_text){
      /*
       * convert the cipher from character sting to hex string
       */
      util_convert_char_to_hex(str_buff, len, cipher_text);
      len = len >> 1;
      /*
       * clear buffer, and save plain text back
       */
      memset(str_buff, 0x0, buff_size);
      plain_text = util_aes_decrypt(aes_dec_ctx, cipher_text, &len);
      if(plain_text && len > 0){
        if(buff_size > len){
          strncpy(str_buff, (char*)plain_text, len);
          str_buff[len] = '\0';
        }else{
          QNAP_LOG_ERR_NL("buffer is too small; buff: %zd < len: %d", buff_size, len);
        }
        free(plain_text);
      }else{
        QNAP_LOG_ERR_NL("failed to decrypt: %p; len: %d", plain_text, len);
      }
      free(cipher_text);
    }
  }

}

char * util_aes_encrypt_str(EVP_CIPHER_CTX *aes_enc_ctx, char *src)
{

  if(!aes_enc_ctx || !src){
    QNAP_LOG_ERR_NL("failed to encrypt: aes_enc_ctx:%s", aes_enc_ctx);
    QNAP_LOG_ERR_NL("failed to encrypt: src:%s", src);
    return NULL;
  }

  uint8_t *plain_text = (uint8_t *)src;
  uint8_t *cipher_text ;
  int len = strlen(src);

  if(len == 0) {
    return NULL;
  }

  /*
   * encrypt the given string
   */
  cipher_text = util_aes_encrypt(aes_enc_ctx, plain_text, &len);
  if(cipher_text){
    /*
     * convert hex binary to char string
     */
    int len_str = len*2+1;
    char *cipher_text_str = malloc(len_str);
    if(cipher_text_str){
      util_convert_hex_to_char(cipher_text, len, cipher_text_str, len);
      return cipher_text_str;
    }
    free(cipher_text);
  }

  QNAP_LOG_ERR_NL("failed to encrypt: %s", src);
  return NULL;

}
