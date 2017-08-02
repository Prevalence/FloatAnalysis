#!/bin/sh

KLEE=${HOME}/Application/klee-fp/
KLEE_INCLUDE=${KLEE}/src/include
ROOT=${HOME}/Application/klee-fp/src/examples/Nuny


${ROOT}/bin/main
if test $? -ne 0
	then exit 2
fi

clang -I${KLEE_INCLUDE} -emit-llvm -O0 -c -g test.c -o test.bc
klee test.bc

: > ./result.txt #clear the result.txt

for ktest_file in `ls klee-last/test*.ktest`
do	
	ktest-tool --write-floats $ktest_file
done
cp -r klee-out-0 klee-out
rm -r main.o test.bc klee-last klee-out-0 klee-out/klee-out-0 test.c expression.txt

