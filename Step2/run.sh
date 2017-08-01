#!/bin/bash
input=$1
echo $input
output=${input##*\/}
./bin/matcher -extra-arg=-I/home/xzt/Downloads/iRRAM_2013_01/include/ $1 -- > ./output/$output
diff $1 ./output/$output
cd ./output
clang-3.4 -emit-llvm -I../../../../myklee/include/klee -g -c $output
kleefile=${output%%.*}.bc
klee $kleefile
