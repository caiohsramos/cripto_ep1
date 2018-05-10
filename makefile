all: k128.o entropy.o main.o
	gcc entropy.o main.o k128.o -o k128
entropy.o: entropy.c
	gcc entropy.c -c
k128.o: k128.c
	gcc k128.c -c
main.o: main.c
	gcc main.c -c
clean:
	rm *.o k128 outputs/*
