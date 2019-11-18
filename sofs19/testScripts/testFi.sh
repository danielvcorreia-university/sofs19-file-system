como#!/usr/bin/env bash

for j in {1..70}; do

	printf "ai\n1\n777\n0\n" | ./testtool -b -q 1 dsk0
done


#for i in {1..70}; do
#
#	printf "fi\n$i\n0\n" | ./testtool -b -r 402-402 -q 1 dsk0
#done
