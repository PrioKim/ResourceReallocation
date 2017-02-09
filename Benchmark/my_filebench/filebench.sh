#!/bin/sh

function filebench_by_workload {
	workload=$@
	sync
	echo 3 > /proc/sys/vm/drop_caches
	filebench -f "$workload" >> result
}

i=0
f=fileserver.f
o=oltp.f
w=webserver.f
v=varmail.f

echo 0 > /proc/sys/kernel/randomize_va_space
echo "" > result

while [ ${i} -le 3 ]
do
	filebench_by_workload $f
	filebench_by_workload $w
	filebench_by_workload $v
        i=$((i+1))
done

