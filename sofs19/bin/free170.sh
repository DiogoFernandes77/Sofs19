#!/bin/bash

for i in {1..170}
do
	echo -ne "fdb\n${i}\n0\n" | ./testtool -b -q1 $*
	
	((i++))	
done

echo 0
