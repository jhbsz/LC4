/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@iahu.ca, http://libtomcrypt.org
 */
#include "tomcrypt.h"

/**
   @file ofb_setiv.c
   OFB implementation, set IV, Tom St Denis
*/

#ifdef OFB

/**
   Set an initial vector
   @param IV   The initial vector
   @param len  The length of the vector (in octets)
   @param ofb  The OFB state
   @return CRYPT_OK if successful
*/
int ofb_setiv(const unsigned char *IV, unsigned long len, symmetric_OFB *ofb)
{
   int err;

   LTC_ARGCHK(IV  != NULL);
   LTC_ARGCHK(ofb != NULL);

   if ((err = cipher_is_valid(ofb->cipher)) != CRYPT_OK) {
       return err;
   }

   if (len != (unsigned long)ofb->blocklen) {
      return CRYPT_INVALID_ARG;
   }

   /* force next block */
   ofb->padlen = 0;
   cipher_descriptor[ofb->cipher].ecb_encrypt(IV, ofb->IV, &ofb->key);
   return CRYPT_OK;
}

#endif 

