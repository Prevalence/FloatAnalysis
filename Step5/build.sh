#!/bin/bash

rm exampleTranslator.lo
make -f demoMakefile
echo "========================================================================================================="

function save() {
	echo ' ' > temp.txt
	for line in `cat $1 | grep '#include'`
	do
		echo $line
		sed -i '1i'${line} temp.txt
		sed -i '1d' $1
	done
}

function load() {
	for line in `cat temp.txt | grep '#include'`
	do
		sed -i '1i'${line} $1
		sed -i '1d' temp.txt
	done
}


function translate() {
	for i in `ls testdir/*.C`
	do 
		./demoTranslator "$i"
	done
}
#./demoTranslator test.C

translate

