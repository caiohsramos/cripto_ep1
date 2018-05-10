#include "k128.h"
#include "entropy.h"
#include <stdio.h>
#include <stdlib.h>

void entropy_1(char *in_file, char *passwd) {
	FILE *fp = NULL;
	unsigned int i, k, file_size, mask, n_block;
	long int sum, max, min;
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
	sum = 0;
	max = -1;
	min = -1;
	for(k = 0; k < n_block; k++) { 
		//accumulates sum
		sum += sum;
		//for each block...
		for(i = 0; i < 128; i++) {
			//changes i bit of block k

			//generate alter with something like 'xor mask << i'

			//encrypt both blocks...

			//calculate hamming distance... 
			
			//evaluate max and min values

		}
		//compute and print max, min, average and sum
		
	}




	//closes file
	fclose(fp);
}


void entropy_2(char *in_file, char *passwd) {

}

