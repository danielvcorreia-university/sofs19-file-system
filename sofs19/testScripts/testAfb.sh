#!/usr/bin/env bash

for i in {1..1000}; do
	printf "afb\n1\n$i\n0\n" | ./testtool -b -r 441-441 -q 1 disks/dsk0
done
