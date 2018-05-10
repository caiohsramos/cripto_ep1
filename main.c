#include "k128.h"
#include "entropy.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char in[50];
	char out[50];
	char passwd[50];
	strcpy(in, argv[3]);
	switch (argv[1][1]) {
		case 'c':
			strcpy(out, argv[5]);
			strcpy(passwd, argv[7]);
			//calls encrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(in, out, passwd, 1);
			else
				enc(in, out, passwd, 0);
			break;
		case 'd':
			strcpy(out, argv[5]);
			strcpy(passwd, argv[7]);
			//calls decrypt
			if(argc == 9 && argv[8][1] == 'a')
				dec(in, out, passwd, 1);
			else
				dec(in, out, passwd, 0);
			break;
		case '1':
			strcpy(passwd, argv[5]);
			//measures randomness method 1
			//entropy(in, passwd, m1);
			entropy_1(in, passwd);
			break;
		case '2':
			strcpy(passwd, argv[5]);
			//measures randomness method 1
			//entropy(in, passwd, m2);
			entropy_2(in, passwd);
			break;
		default:
			printf("Uso incorreto\n");
			return 1;
	};
	return 0;
}
