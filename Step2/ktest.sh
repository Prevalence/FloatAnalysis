#!/bin/sh

clang++-3.4 main.cc
./a.out

clang -I ../../include -emit-llvm -O0 -c -g example.c -o example.bc
klee example.bc

: > ./result.txt #clear the result.txt
for ktest_file in `ls ./klee-last`
do
ktest-tool --write-floats klee-last/$ktest_file
done

cd ./checking
rm -rf *
cd ..

clang++-3.4 analyze.cc
./a.out

for checking_file in `ls ./checking`
do
clang++-3.4 ./checking/$checking_file
echo'\n'========='\n'
./a.out
echo'\n'========='\n'
done
