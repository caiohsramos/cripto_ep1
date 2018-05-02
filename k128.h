#ifndef _K128_C_
#define _K128_C_


//typedef for byte usage
typedef unsigned int byte_t;

//general encryption function
void enc(char *, char *, char *, int );

//general decryption function
void dec(char *, char *, char *, int );

//128 bit encryption
byte_t *k128(byte_t *, byte_t*);
byte_t *k128_d(byte_t *, byte_t*);

//generate 128 bit key
byte_t *generate_primarykey(char *);

//genearte interkey
byte_t *generate_interkey(byte_t *, int);

//generate 5bit subkey
byte_t generate_subkey5(int , byte_t *);

//generate 32 bit subkey
byte_t generate_subkey32(int , byte_t *);

//funcions that use sbox
byte_t f1(byte_t , byte_t , byte_t );
byte_t f2(byte_t , byte_t , byte_t );
byte_t f3(byte_t , byte_t , byte_t );

//left rotation
byte_t _rotl(byte_t , byte_t );

//get nth byte of int
unsigned char get_byte(byte_t , int );

//make the first subkey
byte_t *assign_k0(byte_t *);

//makes an iteration
byte_t *iteration(int , byte_t *, byte_t* );
byte_t *iteration_d(int , byte_t *, byte_t* );

#endif
