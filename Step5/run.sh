#!/bin/bash

#sed '1,1 i\\n' -i rose_test.C
sed '1i #include<iostream>' -i rose_test.C
g++ rose_test.C
./a.out > log.txt
cat log.txt
