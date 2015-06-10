/* encrypt V1.1 Fri Oct 18 04:28:03 NZDT 2002 */
/* File de/encryption, using libtomcrypt */
/* Written by Daniel Richards <kyhwana@world-net.co.nz> */
/* Help from Tom St Denis with various bits */
/* This code is public domain, no rights reserved. */
/* Encrypts by default, -d flag enables decryption */
/* ie: ./encrypt blowfish story.txt story.ct */
/* ./encrypt -d blowfish story.ct story.pt */

#include <stdio.h>
#include "tomcrypt.h"

#include "ezpkey.h"


int my_errno;

int usage(char *name) 
{
    int x;

    fprintf(stderr, "usage: %s infile ", name) ;
    exit(1);
}

void register_algs(void)
{
    int x;

    register_cipher (&rc6_desc);
    register_cipher (&des3_desc);

    if (register_hash(&sha256_desc) == -1) {
        fprintf(stderr, "Error registering SHA256\n");
        exit(-1);
    } 

    if (register_prng(&yarrow_desc) == -1) {
        fprintf(stderr, "Error registering yarrow PRNG\n");
        exit(-1);
    }

    if (register_prng(&sprng_desc) == -1) {
        fprintf(stderr, "Error registering sprng PRNG\n");
        exit(-1);
    }
}

unsigned long decode(FILE *fdin, FILE *fdout)
{
    unsigned char plaintext[512],ciphertext[512];
    unsigned char tmpkey[512], key[MAXBLOCKSIZE], IV[MAXBLOCKSIZE];
    unsigned char inbuf[2048]; /* i/o block size */
    unsigned long outlen, y, ivsize, x, wlen;
    symmetric_CTR ctr;
    int cipher_idx, hash_idx, ks;
    char *cipher = "3des";
    cipher_idx = find_cipher(cipher);
    if (cipher_idx == -1) {
        fprintf(stderr, "Invalid cipher(%s)\n", cipher);
        exit(-1);
    }

    hash_idx = find_hash("sha256");
    if (hash_idx == -1) {
        fprintf(stderr, "SHA256 not found...?\n");
        exit(-1);
    }

    ivsize = cipher_descriptor[cipher_idx].block_length;
    ks = hash_descriptor[hash_idx].hashsize;
    if (cipher_descriptor[cipher_idx].keysize(&ks) != CRYPT_OK) { 
        fprintf(stderr, "Invalid keysize???\n");
        exit(-1);
    }

    strcpy(tmpkey, EZPUPGKEY) ;
    outlen = sizeof(key);
    if ((my_errno = hash_memory(hash_idx,tmpkey,strlen((char *)tmpkey),key,&outlen)) != CRYPT_OK) {
        fprintf(stderr, "Error hashing key: %s\n", error_to_string(my_errno));
        exit(-1);
    }

    /* Decrypt only */
    /* Need to read in IV */
    if (fread(IV,1,ivsize,fdin) != ivsize) {
        fprintf(stderr, "Error reading IV from input.\n");
        exit(-1);
    }

    if ((my_errno = ctr_start(cipher_idx,IV,key,ks,0,&ctr)) != CRYPT_OK) {
        fprintf(stderr, "ctr_start error: %s\n",error_to_string(my_errno));
        exit(-1);
    }

    wlen = 0 ;
    /* IV done */
    do {
        y = fread(inbuf,1,sizeof(inbuf),fdin);

        if ((my_errno = ctr_decrypt(inbuf,plaintext,y,&ctr)) != CRYPT_OK) {
            fprintf(stderr, "ctr_decrypt error: %s\n",
                    error_to_string(my_errno));
            exit(-1);
        }
        if (fwrite(plaintext,1,y,fdout) != y) {
            fprintf(stderr, "Error writing to file.\n");
            exit(-1);
        }
        wlen += y ;
    } while (y == sizeof(inbuf));
    return wlen;
}

int main(int argc, char *argv[]) 
{
    char *infile, *outfile, cmdl[128], *partname;
    FILE *fdin, *fdout;
    unsigned long wlen;
    char *result_infile = "/tmp/mtd.verify";
    char result[128];

    /* register algs, so they can be printed */
    register_algs();

    if (argc < 2) {
        return usage(argv[0]);
    }

    infile  = argv[1];   /* input file */
    partname = "linux" ; /* Partition name */

    /* Open the image file. */
    fdin = fopen(infile,"rb");
    if (fdin == NULL) {
        perror("Can't open input for reading");
        exit(-1);
    }
    /* Invoke mtd verify. */
    sprintf(cmdl, "mtd verify - > %s", result_infile) ;
    fdout = popen(cmdl, "w") ;
    if(fdout == NULL) {
        perror("popen") ;
        exit(-1);
    }
    /* Decode the incoming file and sent it to the standard output. */
    wlen = decode(fdin, fdout);
    if (wlen < 4200) {
        fprintf(stderr, "Firmware too small.\n") ;
        exit(-1); /* too small, cannot be real firmware */
    }

    /* pclose */
    pclose(fdout);
    /* fclose */
    fclose(fdin);
    /* check result of mtd verify */
    fdin = fopen(result_infile, "r");
    if (fdin == NULL) {
        perror("Can't open the result file for reading");
        exit(-1);
    }
    fgets(result, sizeof(result), fdin);
    fclose(fdin);
    if((strcmp(result, "verify:ok")) != 0) {
        printf("Image file verify fail\n");
        exit(-1);
    }

    /* file handles setup */
    fdin = fopen(infile,"rb");
    if (fdin == NULL) {
        perror("Can't open input for reading");
        exit(-1);
    }
    /* Program decrypted firmware by calling 'mtd' */
    sprintf(cmdl, "mtd write - %s", partname) ;
    if( argc < 3) /* Only one parameter */
        fdout = popen(cmdl, "w") ;
    else /* an extra parameter as output filename for debugging purpose */
        fdout = fopen(argv[2], "w") ;

    if(fdout == NULL) {
        fprintf(stderr, "Cannot open pipe to upgrade firmware\n") ;
        perror("popen") ;
        exit(-1) ;
    }
    /* TODO: check fdout */
    /* Decode. */
    wlen = decode(fdin, fdout);
    pclose(fdout);
    fclose(fdin);

    if (wlen < 4200) {
        fprintf(stderr, "Firmware too small.\n") ;
        exit(-1); /* too small, cannot be real firmware */
    }
    return 0;
}

