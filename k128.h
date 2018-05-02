#ifndef _K128_C_
#define _K128_C_




//typedef for byte usage
typedef unsigned char byte_t;
//general encryption function
void enc(char *, char *, char *, int );
//general decryption function
void dec(char *, char *, char *, int );
//128 bit encryption
byte_t *k128(byte_t *);
//generate 128 bit key
byte_t *generate_primearykey(char *);
//genearte interkey
byte_t *generate_interkey(byte_t *, int);

#endif
