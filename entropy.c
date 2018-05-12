//Caio Henrique Silva Ramos - NUSP 9292991
#include "k128.h"
#include "entropy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//calculates entropy and prints table using the first method
void entropy_1(char *in_file, char *passwd) {
	FILE *fp = NULL;
	unsigned int i, k, file_size, mask, n_block;
	long int *sum, *max, *min;
	long unsigned int j;
	unsigned char *mem_file, *alter_file, *mem_fileC, *alter_fileC;
	unsigned int *x, *y;
	mask = 0x00000001;
	//opens file
	fp = fopen(in_file, "r+");
	//get the file size
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	printf("Size of infile: %u\n", file_size);
	//go back to beginning
	rewind(fp);

	n_block = file_size/16;
	//initializates values
	sum = initiate_value( 0, n_block); 
	max = initiate_value(-1, n_block); 
	min = initiate_value(129, n_block); 
	//reads the file into mem_file
	mem_file = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
	fread(mem_file, 1, file_size, fp);
	//encrypt raw file
	mem_fileC = my_encrypt(mem_file, passwd, file_size);



	//allocs alter_file
	alter_file = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
	for(i = 0; i < n_block; i++) { 
		for(j = 0; j < 128; j++) {
		memcpy(alter_file, mem_file, file_size);
		//changes j bit with something like 'xor mask << j'
		y = (unsigned int *)&(alter_file[i*16]);
		x = (unsigned int *)&(mem_file[i*16]);
		y[(j/32)] = x[(j/32)] ^ (mask << (j % 32));	
		//alter_file = (unsigned char *)y;

		//printf("original: %s\n", mem_file);
		//printf("alter: %s\n", alter_file);

		//encrypt changed file
		alter_fileC = my_encrypt(alter_file, passwd, file_size);
		//calculate hamming distance between mem_fileC and alter_fileC (for each block) 
		for(k = 0; k < n_block; k++) {
			if(k >= i) {
				//hamming distance
				int h = h_dis(mem_fileC, alter_fileC, k);
				//printf("Distância mudando o bit %lu: %d\n", j+i*128, h);
				sum[k] += h;
				if(h > max[k]) max[k] = h;
				if(h < min[k]) min[k] = h;
			}
		}

			
		free(alter_fileC);
		}
		
	}

	//print h(k) results
	printf("        Block       |         Min        |         Max        |       Average      \n");
	printf("--------------------|--------------------|--------------------|--------------------\n");
	for(k = 0; k < n_block; k++) {
		printf("%20u|%20ld|%20ld|%20.2lf\n", (k+1),min[k],max[k],sum[k]/(128.0*(k+1)));
	}


	//free allocated space
	free(sum);
	free(max);
	free(min);
	free(alter_file);
	free(mem_file);
	free(mem_fileC);
	//closes file
	fclose(fp);
}

//calculates entropy and prints table using the second method
void entropy_2(char *in_file, char *passwd) {
	FILE *fp = NULL;
	unsigned int i, k, file_size, mask, n_block;
	long int *sum, *max, *min;
	long unsigned int j;
	unsigned char *mem_file, *alter_file, *mem_fileC, *alter_fileC;
	unsigned int *x, *y;
	mask = 0x00000001;
	//opens file
	fp = fopen(in_file, "r+");
	//get the file size
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	printf("Size of infile: %u\n", file_size);
	//go back to beginning
	rewind(fp);

	n_block = file_size/16;
	//initializates values
	sum = initiate_value( 0, n_block); 
	max = initiate_value(-1, n_block); 
	min = initiate_value(129, n_block); 
	//reads the file into mem_file
	mem_file = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
	fread(mem_file, 1, file_size, fp);
	//encrypt raw file
	mem_fileC = my_encrypt(mem_file, passwd, file_size);



	//allocs alter_file
	alter_file = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
	for(i = 0; i < n_block; i++) { 
		for(j = 0; j < 128; j++) {
		memcpy(alter_file, mem_file, file_size);
		//changes j bit with something like 'xor mask << j'
		y = (unsigned int *)&(alter_file[i*16]);
		x = (unsigned int *)&(mem_file[i*16]);
		y[(j/32)] = x[(j/32)] ^ (mask << (j % 32));	
		//change one more bit
		y[(j/32)] = y[(j/32)] ^ (mask << ((j+8) % 32));	


		//encrypt changed file
		alter_fileC = my_encrypt(alter_file, passwd, file_size);
		//calculate hamming distance between mem_fileC and alter_fileC (for each block) 
		for(k = 0; k < n_block; k++) {
			if(k >= i) {
				//hamming distance
				int h = h_dis(mem_fileC, alter_fileC, k);
				sum[k] += h;
				if(h > max[k]) max[k] = h;
				if(h < min[k]) min[k] = h;
			}
		}

			
		free(alter_fileC);
		}
		
	}

	//print h(k) results
	printf("        Block       |         Min        |         Max        |       Average      \n");
	printf("--------------------|--------------------|--------------------|--------------------\n");
	for(k = 0; k < n_block; k++) {
		printf("%20u|%20ld|%20ld|%20.2lf\n", (k+1),min[k],max[k],sum[k]/(128.0*(k+1)));
	}


	//free allocated space
	free(sum);
	free(max);
	free(min);
	free(alter_file);
	free(mem_file);
	free(mem_fileC);
	//closes file
	fclose(fp);

}

//function to initialize vectors with a value
long int *initiate_value(int value, unsigned int n) {
	long int *x = (long int *)malloc(sizeof(long int)*n);
	int i;
	for(i = 0; i < n; i++) {
		x[i] = value;
	}
	return x;
}

//adapted encryption alg for in memory usage
unsigned char *my_encrypt(unsigned char *in, char *passwd, unsigned int size) {
	block_t *x = (block_t*)malloc(sizeof(block_t)*4);
	block_t *y = NULL;
	block_t *y_old = (block_t*)malloc(sizeof(block_t)*4);
	y_old[0] = y_old[1] = y_old[2] = y_old[3] = 0xffffffff;
	block_t *key = NULL;
	long unsigned int N;
	int i;	
	unsigned char *result = (unsigned char *)malloc(sizeof(unsigned char)*size);

	//generate primary key
	key = generate_primarykey(passwd);
	//get N (number of full blocks)
	N = size/16;
	for(i = 0; i < N; i++) {
		//reads a block
		memcpy(x, &(in[i*16]), 16);
		y = k128(x, y_old, key); 
		memcpy(&(result[i*16]), y, 16);
		memcpy(y_old, y, 16);
		//frees allocated space
		free(y);
	}
	//frees allocated space
	free(x);	
	free(y_old);	
	free(key);
	return result;
}	

//calculates the hamming distance between two blocks (k block)
int h_dis(unsigned char *x, unsigned char *y, unsigned int k) {
		unsigned int *a = (unsigned int *)&(x[k*16]);
		unsigned int *b = (unsigned int *)&(y[k*16]);
			
		int sum = 0;
		int j;
		for(j = 0; j < 4; j++) {
			int i  = a[j] ^ b[j];
			i = i - ((i >> 1) & 0x55555555);
			i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
			i = (i + (i >> 4)) & 0x0f0f0f0f;
			i = i + (i >> 8);
			i = i + (i >> 16);
			sum += i & 0x3f;		 
		}	

	return sum;
}
