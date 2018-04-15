#include <stdio.h>
#include "k128.h"

int main(int argc, char *argv[]) {
	switch (argv[1][1]) {
		case 'c':
			//calls decrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(argv[3], argv[5], argv[7], 1);
			else
				enc(argv[3], argv[5], argv[7], 0);
			break;
		case 'd':
			//calls encrypt
			if(argc == 9 && argv[8][1] == 'a')
				dec(argv[3], argv[5], argv[7], 1);
			else
				dec(argv[3], argv[5], argv[7], 0);
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
