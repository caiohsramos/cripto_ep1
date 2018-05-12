//Caio Henrique Silva Ramos - NUSP 9292991
#ifndef _ENTROPY_H_
#define _ENTROPY_H_

//calculates entropy using the first method
void entropy_1(char *, char *);

//calculates entropy using the second method
void entropy_2(char *, char *);

//initializes a vector with a value
long int *initiate_value(int , unsigned int );

//adapted encryption alg for in memory usage
unsigned char *my_encrypt(unsigned char *, char *, unsigned int);

//calculates hamming distance between two blocks
int h_dis(unsigned char *, unsigned char *, unsigned int);

#endif
