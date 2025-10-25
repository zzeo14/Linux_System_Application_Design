#!/bin/sh

if [ $# -ne 2 ]
then
	echo "Invalid Input"
	exit 0
fi

if [ $1 -le 0 ] || [ $2 -le 0 ] ; then
	echo "Input must be greater than 0"
	exit 0
fi

for i in $(seq 1 $1) ; do
	for j in $(seq 1 $2) ; do
		printf "%d*%d=%d\t" $i $j $((i * j))
	done
	echo ""
done

exit 0
