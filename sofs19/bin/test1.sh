#!/bin/bash

for i in {1..64}
do
	echo -ne "adb\n0" |./testtool -b -r 443-443 d1
done







echo 0
