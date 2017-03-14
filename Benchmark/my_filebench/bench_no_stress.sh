#! /bin/sh

argc=$#

if [ 1 -ne $argc ]
then
        echo "Please input the workload"
        exit
fi

workload=$1
nr_cpus=`grep -c processor /proc/cpuinfo`
mem_cap=`free -g | grep Mem | awk '{print $2}'`
mem_cap=`expr $mem_cap + 1`
dirname=$workload'_'$nr_cpus'_'$mem_cap

if [ ! -d $dirname ]
then
	mkdir $dirname
fi

echo 0 > /proc/sys/kernel/randomize_va_space
echo 3 > /proc/sys/vm/drop_caches
sync

filebench -f "$workload" > tmp_result

summary=`cat tmp_result | grep "IO Summary"`
summary_path=$dirname'/results'
bandwidth=`cat tmp_result | grep "IO Summary" | awk '{print $11}'`
bandwidth_path=$dirname'/bandwidths'

echo $summary >> $summary_path
echo $bandwidth >> $bandwidth_path
echo $bandwidth
