#!/bin/sh
: > ./result.txt #clear the result.txt
for ktest_file in `ls ./klee-last`
do
	ktest-tool --write-floats klee-last/$ktest_file
done

for checking_file in `ls ./checking`
do
	clang++-3.4 ./checking/$checking_file
	./a.out
done
