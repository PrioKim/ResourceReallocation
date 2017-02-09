#!/bin/sh

i=0

echo "" > result

while [ ${i} -le 3 ]
do
	rm -f CentOS7-minimal-ieice-benchmark.img.xz
	sync
	{ time pxz -kvf -2 -T4 CentOS7-minimal-ieice-benchmark.img ; } 2>> result
	i=$((i+1))
done

cat result | grep real | awk '{print $NF}'
