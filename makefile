all:
	gcc k128.c -c
	gcc main.c -c
	gcc main.o k128.o -o k128
clean:
	rm main.o k128.o k128
debug:
	gcc k128.c -c -v
	gcc main.c -c -v
	gcc main.o k128.o -o k128 -v
