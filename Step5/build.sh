#!/bin/bash

rm exampleTranslator.lo
make -f demoMakefile
echo "========================================================================================================="

function translate() {
	for i in `ls *.C`
	do 
		./demoTranslator "$i"
	done
}
#./demoTranslator test.C

translate
