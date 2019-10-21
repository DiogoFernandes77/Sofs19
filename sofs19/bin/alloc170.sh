#!/bin/bash

for i in {1..170}
do
	echo -ne "adb\n0" | ./testtool -b -q1 $*
done

echo 0
