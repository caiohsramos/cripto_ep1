#!/bin/bash

./k128 -c -i inputs/1.jpg -o outputs/1.out -p Bz4cDcLF > /dev/null
echo "Arquivo criptografado em  outputs/1.out"
./k128 -d -i outputs/1.out -o outputs/result.jpg -p Bz4cDcLF > /dev/null
echo "Arquivo decriptografado em  outputs/result.jpg"
echo "PRONTO CARALHO!"

