#!/bin/sh

function filebench_by_workload {
	workload=$1
	fname="${2}-${workload}-${3}"
	fpath=$4'/'$fname
	sync
	echo 3 > /proc/sys/vm/drop_caches
	#echo "$@" >> result
	filebench -f "$workload" >> $fpath
	echo >> $fname
	echo >> $fname
}

argc=$#

if [ 0 -eq $argc ]
then
	echo "Please input the stree parameter"
	exit
fi

i=0
f=fileserver.f
o=oltp.f
w=webserver.f
v=varmail.f
stress_cpu_mem=$1
nr_cpus=`grep -c processor /proc/cpuinfo`
mem_cap=`free -g | grep Mem | awk '{print $2}'`

dir_name=$1'_'$nr_cpus'_'$mem_cap

mkdir $dir_name
echo 0 > /proc/sys/kernel/randomize_va_space
echo "" > result

while [ ${i} -le 3 ]
do
	filebench_by_workload $f $fname_prefix $i $dir_name
	filebench_by_workload $w $fname_prefix $i $dir_name
	filebench_by_workload $v $fname_prefix $i $dir_name
        i=$((i+1))
done

