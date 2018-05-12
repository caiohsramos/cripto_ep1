//Caio Henrique Silva Ramos - NUSP 9292991
#include <stdlib.h>
#include <stdio.h>
#include "k128.h"
#include "sbox.h"
#include <string.h>

//constants
const block_t constM = 0xCB3725F7;
const block_t constR = 0x13;


void enc(char *in_filename, char *out_filename, char *passwd, int erase) {
	block_t *x = (block_t*)malloc(sizeof(block_t)*4);
	block_t *y = NULL;
	block_t *y_old = (block_t*)malloc(sizeof(block_t)*4);
	y_old[0] = y_old[1] = y_old[2] = y_old[3] = 0xffffffff;
	block_t *key = NULL;
	FILE *fp_in = NULL;
	FILE *fp_out = NULL;
	long unsigned int file_size, N;
	int i;

	//generate primary key
	key = generate_primarykey(passwd);
	//opens files
	fp_in = fopen(in_filename, "r+");
	fp_out = fopen(out_filename, "w+");
	//get filesize
	fseek(fp_in, 0, SEEK_END);
	file_size = ftell(fp_in);
	printf("Size of infile: %lu\n", file_size);
	//go back to beginning
	rewind(fp_in);
	//get N (number of full blocks)
	N = file_size/16;
	//encrypt block by block using CBC mode
	for(i = 0; i < N; i++) {
		//reads a block
		fread(x, 1, 16, fp_in);	
		y = k128(x, y_old, key); 
		fwrite(y, 1, 16, fp_out);
		memcpy(y_old, y, 16);
		//frees allocated space
		free(y);
	}
	//now we have to check and encrypt the last block
	int remain = file_size % 16;
	if(remain != 0) {
		//fill with ones
		ones(x);
		//reads the remainder block
		fread(x, 1, remain, fp_in);	
		y = k128(x, y_old, key); 
		fwrite(y, 1, 16, fp_out);
		memcpy(y_old, y, 16);
		//frees allocated space
		free(y);
	}

	//records the filesize in a adicional block
	//fill with ones
	ones(x);
	x[3] = remain;
	//write size remain on the last byte
	y = k128(x, y_old, key); 
	fwrite(y, 1, 16, fp_out);
	//frees allocated space
	free(y);

	//frees allocated space
	free(x);	
	free(y_old);	
	free(key);

	//blank + erase if asked
	if(erase) {
		char blank = 0;
		rewind(fp_in);		
		fwrite(&blank, 1, file_size, fp_in);
	}

	//closes files
	fclose(fp_in);
	fclose(fp_out);
	//deletes in_filename if asked
	if(erase) remove(in_filename);
}

void dec(char *in_filename, char *out_filename, char *passwd, int erase) {
	block_t *x = (block_t*)malloc(sizeof(block_t)*4);
	block_t *y = NULL;
	block_t *x_old = (block_t*)malloc(sizeof(block_t)*4);
	x_old[0] = x_old[1] = x_old[2] = x_old[3] = 0xffffffff;
	block_t *key = NULL;
	FILE *fp_in = NULL;
	FILE *fp_out = NULL;
	long unsigned int file_size, N;
	int i;

	//generate primary key
	key = generate_primarykey(passwd);
	//opens files
	fp_in = fopen(in_filename, "r+");
	fp_out = fopen(out_filename, "w+");
	//get filesize
	fseek(fp_in, 0, SEEK_END);
	file_size = ftell(fp_in);
	printf("Size of infile: %lu\n", file_size);
	//go back to beginning
	rewind(fp_in);
	//get N (number of full blocks)
	N = file_size/16;
	//starts decryption on CBC mode
	for(i = 0; i < (N-2); i++) {
		//reads a block
		fread(x, 1, 16, fp_in);	
		y = k128_d(x, x_old, key); 
		fwrite(y, 1, 16, fp_out);
		memcpy(x_old, x, 16);
		//frees allocated space
		free(y);
	}
	//reads the last block
	fread(x, 1, 16, fp_in);	
	y = k128_d(x, x_old, key); 
	memcpy(x_old, x, 16);
	//saves the result
	block_t *y_tmp = (block_t*)malloc(sizeof(block_t)*4);
	memcpy(y_tmp, y, 16);
	//reads the size block and saves on the result file
	fread(x, 1, 16, fp_in);	
	y = k128_d(x, x_old, key); 
	int remain = y[3];
	if(remain != 0) 
		fwrite(y_tmp, 1, remain, fp_out);
	else 
		fwrite(y_tmp, 1, 16, fp_out);
	free(y);
	//frees allocated space
	free(x);	
	free(y_tmp);	
	free(x_old);	
	free(key);
	
	//erases if asked
	if(erase) {
		char blank = 0;
		rewind(fp_in);		
		fwrite(&blank, 1, file_size, fp_in);
	}

	//closes files
	fclose(fp_in);
	fclose(fp_out);
	//deletes in_filename if asked
	if(erase) remove(in_filename);
}

//k128 to encrypt a block
block_t *k128(block_t *in, block_t *y_old, block_t *key) {
	int i;
	block_t *y = (block_t*)malloc(sizeof(block_t)*4);
	memcpy(y, in, 16);
	xor(y, y_old);
	for(i = 0; i < 12; i++) {
		y = iteration(i, y, key);
	}
	return y;
}

//k128 to decrypt a block
block_t *k128_d(block_t *in, block_t *x_old, block_t *key) {
	int i;
	block_t *y = (block_t*)malloc(sizeof(block_t)*4);
	memcpy(y, in, 16);
	for(i = 0; i < 12; i++) {
		y = iteration_d(11-i, y, key);
	}
	xor(y, x_old);
	return y;
}

//one iteration of the encryption alg
block_t *iteration(int i, block_t *x, block_t *k) {
	block_t aux;
	block_t *interk = generate_interkey(k, i);
	x[1] = x[1] ^ f2(x[0],generate_subkey5(0,interk),generate_subkey32(0,interk));
	x[2] = x[2] ^ f1(x[1],generate_subkey5(1,interk),generate_subkey32(1,interk));
	x[3] = x[3] ^ f3(x[2],generate_subkey5(2,interk),generate_subkey32(2,interk));
	x[0] = x[0] ^ f2(x[3],generate_subkey5(3,interk),generate_subkey32(3,interk));
	//make the swaps
	aux = x[1];
	x[1] = x[3];
	x[3] = aux;
	free(interk);
	return x;
}

//one iteration of the decryption alg
block_t *iteration_d(int i, block_t *x, block_t *k) {
	block_t aux;
	block_t *interk = generate_interkey(k, i);
	//make the swaps
	aux = x[1];
	x[1] = x[3];
	x[3] = aux;


	x[0] = x[0] ^ f2(x[3],generate_subkey5(3,interk),generate_subkey32(3,interk));
	x[3] = x[3] ^ f3(x[2],generate_subkey5(2,interk),generate_subkey32(2,interk));
	x[2] = x[2] ^ f1(x[1],generate_subkey5(1,interk),generate_subkey32(1,interk));
	x[1] = x[1] ^ f2(x[0],generate_subkey5(0,interk),generate_subkey32(0,interk));
	free(interk);
	return x;
}

block_t *generate_primarykey(char *key) {
	block_t *k = (block_t*)malloc(sizeof(block_t)*4);
	char x[50];
	strcpy(x, key);
	if (strlen(x) < 16) {
		char aux[50];
		strcpy(aux, x);
		strcat(x, aux);
	} 
	memcpy(k, x, 16);
	return k;
}

block_t *generate_interkey(block_t *primary_key, int i) {
	block_t *k = assign_k0(primary_key);	
	block_t aux;
	int j;
	for(j = 0; j < i; j++) {
		k[0] = k[0] ^ f2(k[3],_rotl(constR,(((i+2)*(i+2))%3)),_rotl(constM,(((i+2)*(i+2))%7)));
		k[1] = k[1] ^ f1(k[0],_rotl(constR,((i+2)%3)),_rotl(constM,((i+2))%7));
		k[2] = k[2] ^ f3(k[1],_rotl(constR,(((i+2)*(i+2)*(i+2))%3)),_rotl(constM,(((i+2)*(i+2)*(i+2))%7)));
		k[3] = k[3] ^ f2(k[2],_rotl(constR,(((i+2)*(i+2))%3)),_rotl(constM,(((i+2)*(i+2))%7)));
		//w<->x
		aux = k[0];
		k[0] = k[3];
		k[3] = aux;
		//y<->z
		aux = k[2];
		k[2] = k[1];
		k[1] = aux;
	}
	//w<->x
	aux = k[0];
	k[0] = k[3];
	k[3] = aux;
	//y<->z
	aux = k[2];
	k[2] = k[1];
	k[1] = aux;
	return k;	

}

block_t generate_subkey5(int j, block_t *interkey) {
	return (interkey[(3-j)] & 0x1f);
}

block_t generate_subkey32(int j, block_t *interkey) {
	return interkey[j];
}

block_t f1(block_t x, block_t k5, block_t k32) {
	block_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] + box_2[i2]) - box_3[i3]) ^ box_4[i4];

	return y;
}

block_t f2(block_t x, block_t k5, block_t k32) {
	block_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] - box_2[i2]) ^ box_3[i3]) + box_4[i4];

	return y;

}

block_t f3(block_t x, block_t k5, block_t k32) {
	block_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] ^ box_2[i2]) + box_3[i3]) - box_4[i4];

	return y;

}

//rotation auxiliar function
block_t _rotl(block_t value, block_t shift) {
    if ((shift &= sizeof(value)*8 - 1) == 0)
      return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

//gets a single byte of a 32 bit block
unsigned char get_byte(block_t number, int n) {
	return ((number >> (8*n)) & 0xff);
}

block_t *assign_k0(block_t *k) {
	block_t *k0 = (block_t*)malloc(sizeof(block_t)*4);
	k0[0] = 0xBD070242 ^ k[0];
	k0[1] = 0x657B7C8E ^ k[1];
	k0[2] = 0x874AA67D ^ k[2];
	k0[3] = 0x5A827999 ^ k[3]; 
	return k0;
}

//xor between two 32 bit blocks
void xor(block_t *x, block_t *y) {
	x[0] = x[0]^y[0];
	x[1] = x[1]^y[1];
	x[2] = x[2]^y[2];
	x[3] = x[3]^y[3];
}

void ones(block_t *x) {
	x[0] = 0xffffffff;
	x[1] = 0xffffffff;
	x[2] = 0xffffffff;
	x[3] = 0xffffffff;
}
