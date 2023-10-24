#!/bin/sh

val=`expr 2 + 2`
val2=`expr 2 + 4`

if [ $val -eq $val2 ]
then 
	echo "a and b are not equal"
else
	echo "base case"
fi

echo "Total value : $val2"