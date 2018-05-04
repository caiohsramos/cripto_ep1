#include <stdio.h>
#include "k128.h"

int main(int argc, char *argv[]) {
	switch (argv[1][1]) {
		case 'c':
			//calls encrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(argv[3], argv[5], argv[7], 1, &k128);
			else
				enc(argv[3], argv[5], argv[7], 0, &k128);
			break;
		case 'd':
			//calls decrypt
			if(argc == 9 && argv[8][1] == 'a')
				enc(argv[3], argv[5], argv[7], 1, &k128_d);
			else
				enc(argv[3], argv[5], argv[7], 0, &k128_d);
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
