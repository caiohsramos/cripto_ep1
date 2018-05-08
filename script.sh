#!/bin/bash

./k128 -c -i inputs/6ac3f2fc-d7e6-47ea-a0b2-fd25fb0c85eb.jpg -o outputs/1.out -p Bz4cDcLF
echo "Arquivo criptografado em  outputs/1.out"
./k128 -d -i outputs/1.out -o outputs/result.jpg -p Bz4cDcLF
echo "Arquivo decriptografado em  outputs/result.jpg"
echo "PRONTO CARALHO!"

