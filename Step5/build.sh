#!/bin/bash

rm exampleTranslator.lo
make -f demoMakefile
echo "========================================================================================================="
./demoTranslator test.C
