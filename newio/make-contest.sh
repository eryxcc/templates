#!/bin/sh
mkdir -p $1
g++ templator.cpp -o templator -I ../libs/
for x in a b c d e f g h i j
do mkdir $1/$x
cp Makefile templator.cpp templator threadengine.cpp work.cpp t.in ../libs/topcoder.h $1/$x/
done
