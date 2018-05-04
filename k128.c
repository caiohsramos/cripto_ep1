#include <stdlib.h>
#include <stdio.h>
#include "k128.h"
#include "sbox.h"
#include <string.h>

//constants
const byte_t constM = 0xCB3725F7;
const byte_t constR = 0x13;

void enc(char *in_filename, char *out_filename, char *passwd, int erase) {
	byte_t *x = (byte_t*)malloc(sizeof(byte_t)*4);
	byte_t *y = NULL;
	byte_t *key = NULL;
	FILE *fp_in = NULL;
	FILE *fp_out = NULL;
	long unsigned int file_size;

	//generate primary key
	key = generate_primarykey(passwd);
	//opens files
	fp_in = fopen(in_filename, "r");
	fp_out = fopen(out_filename, "w+");
	//get filesize
	fseek(fp_in, 0, SEEK_END);
	file_size = ftell(fp_in);
	printf("Size of infile: %lu\n", file_size);
	//go back to beginning
	rewind(fp_in);
	//reads a block
	fread(x, 1, 16, fp_in);	
	y = k128(x, key); 
	fwrite(y, 1, 16, fp_out);
	//frees allocated space
	free(y);
	free(x);	
	free(key);
	//closes files
	fclose(fp_in);
	fclose(fp_out);
}

void dec(char *in_filename, char *out_filename, char *passwd, int erase) {
	byte_t *x = (byte_t*)malloc(sizeof(byte_t)*4);
	byte_t *y = NULL;
	byte_t *key = NULL;
	FILE *fp_in = NULL;
	FILE *fp_out = NULL;
	long unsigned int file_size;

	//generate primary key
	key = generate_primarykey(passwd);
	//opens files
	fp_in = fopen(in_filename, "r");
	fp_out = fopen(out_filename, "w+");
	//gwt filesize
	fseek(fp_in, 0, SEEK_END);
	file_size = ftell(fp_in);
	printf("Size of infile: %lu\n", file_size);
	//go back to beginning
	rewind(fp_in);
	//reads a block
	fread(x, 1, 16, fp_in);	
	y = k128_d(x,key); 
	fwrite(y, 1, 16, fp_out);
	//frees allocated space
	free(x);
	free(y);
	free(key);
	//closes files
	fclose(fp_in);
	fclose(fp_out);
}

byte_t *k128(byte_t *in, byte_t *key) {
	int i;
	byte_t *y = (byte_t*)malloc(sizeof(byte_t)*4);
	memcpy(y, in, 16);
	for(i = 0; i < 12; i++) {
		y = iteration(i, y, key);
	}
	return y;
}

byte_t *k128_d(byte_t *in, byte_t *key) {
	int i;
	byte_t *y = (byte_t*)malloc(sizeof(byte_t)*4);
	memcpy(y, in, 16);
	for(i = 0; i < 12; i++) {
		y = iteration_d(11-i, y, key);
	}
	return y;
}


byte_t *iteration(int i, byte_t *x, byte_t *k) {
	byte_t aux;
	byte_t *interk = generate_interkey(k, i);
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

byte_t *iteration_d(int i, byte_t *x, byte_t *k) {
	byte_t aux;
	byte_t *interk = generate_interkey(k, i);
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

byte_t *generate_primarykey(char *key) {
	byte_t *k = (byte_t*)malloc(sizeof(byte_t)*4);
	char x[50];
	strcpy(x, key);
	if (strlen(x) < 16) {
		strcat(x, x);
	} 
	memcpy(k, x, 16);
	return k;
}

byte_t *generate_interkey(byte_t *primary_key, int i) {
	byte_t *k = assign_k0(primary_key);	
	byte_t aux;
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

byte_t generate_subkey5(int j, byte_t *interkey) {
	return (interkey[(3-j)] & 0x1f);
}

byte_t generate_subkey32(int j, byte_t *interkey) {
	return interkey[j];
}

byte_t f1(byte_t x, byte_t k5, byte_t k32) {
	byte_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] + box_2[i2]) - box_3[i3]) ^ box_4[i4];

	return y;
}

byte_t f2(byte_t x, byte_t k5, byte_t k32) {
	byte_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] - box_2[i2]) ^ box_3[i3]) + box_4[i4];

	return y;

}

byte_t f3(byte_t x, byte_t k5, byte_t k32) {
	byte_t i, y;
	unsigned char i1, i2, i3, i4;
	i = _rotl((k32 ^ x), k5);
	i1 = get_byte(i, 0);
	i2 = get_byte(i, 1);
	i3 = get_byte(i, 2);
	i4 = get_byte(i, 3);

	y = ((box_1[i1] ^ box_2[i2]) + box_3[i3]) - box_4[i4];

	return y;

}

byte_t _rotl(byte_t value, byte_t shift) {
    if ((shift &= sizeof(value)*8 - 1) == 0)
      return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

unsigned char get_byte(byte_t number, int n) {
	return ((number >> (8*n)) & 0xff);
}

byte_t *assign_k0(byte_t *k) {
	byte_t *k0 = (byte_t*)malloc(sizeof(byte_t)*4);
	k0[0] = 0xBD070242 ^ k[0];
	k0[1] = 0x657B7C8E ^ k[1];
	k0[2] = 0x874AA67D ^ k[2];
	k0[3] = 0x5A827999 ^ k[3]; 
	return k0;
}
