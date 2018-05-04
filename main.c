#include "k128.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char in[50];
	char out[50];
	char passwd[50];
	strcpy(in, argv[3]);
	strcpy(out, argv[5]);
	strcpy(passwd, argv[7]);
	switch (argv[1][1]) {
		case 'c':
			//calls encrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(in, out, passwd, 1, &k128);
			else
				enc(in, out, passwd, 0, &k128);
			break;
		case 'd':
			//calls decrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(in, out, passwd, 1, &k128_d);
			else
				enc(in, out, passwd, 0, &k128_d);
			break;
		case '1':
			//measures randomness method 1
			break;
		case '2':
			//measures randomness method 1
			break;
		default:
			printf("Uso incorreto\n");
			return 1;
	};
	return 0;
}
