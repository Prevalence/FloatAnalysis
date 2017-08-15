#!/bin/bash

rm exampleTranslator.lo
make -f exampleMakefile
echo "========================================================================================================="
./exampleTranslator test.C

