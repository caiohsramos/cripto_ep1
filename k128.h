#ifndef _K128_C_
#define _K128_C_


//typedef for byte usage
typedef unsigned int block_t;

//general encryption function
void enc(char *, char *, char *, int );

//general decryption function (using pointers to function)
void dec(char *, char *, char *, int );

//128 bit encryption
block_t *k128(block_t *, block_t*, block_t*);
block_t *k128_d(block_t *, block_t*, block_t*);

//generate 128 bit key
block_t *generate_primarykey(char *);

//genearte interkey
block_t *generate_interkey(block_t *, int);

//generate 5bit subkey
block_t generate_subkey5(int , block_t *);

//generate 32 bit subkey
block_t generate_subkey32(int , block_t *);

//funcions that use sbox
block_t f1(block_t , block_t , block_t );
block_t f2(block_t , block_t , block_t );
block_t f3(block_t , block_t , block_t );

//left rotation
block_t _rotl(block_t , block_t );

//get nth byte of int
unsigned char get_byte(block_t , int );

//make the first subkey
block_t *assign_k0(block_t *);

//makes an iteration
block_t *iteration(int , block_t *, block_t* );
block_t *iteration_d(int , block_t *, block_t* );

void xor(block_t*, block_t*);

void ones(block_t *);


#endif
