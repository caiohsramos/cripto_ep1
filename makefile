all: k128.o main.o
	gcc main.o k128.o -o k128
k128.o: k128.c
	gcc k128.c -c
main.o: main.c
	gcc main.c -c
clean:
	rm main.o k128.o k128
