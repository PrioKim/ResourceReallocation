!/bin/sh

function filebench_by_workload {
        workload=$1
        fname="${workload}-${2}"
        fpath=${3}'/'$fname
        sync
        echo 3 > /proc/sys/vm/drop_caches
        filebench -f "$workload" >> $fpath
        summary=`cat $fpath | grep "IO Summary"`
        summary=$fname' : '$summary
        summary_path=${3}'/results'
        bw_summary_path=${3}'/bandwidth_result'
        bandwidth=`cat $fpath | grep "IO Summary" | awk '{print $11}'`
        bandwidth=$fname' : '$bandwidth
        echo $bandwidth >> $bw_summary_path
        echo $summary >> $summary_path
}

argc=$#

if [ 1 -ne $argc ]
then
        echo "Please input the stree parameter (by one string)"
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
mem_cap=`expr $mem_cap + 1`
dir_name=$1'_'$nr_cpus'_'$mem_cap

mkdir $dir_name
echo 0 > /proc/sys/kernel/randomize_va_space
echo "" > result
echo "" > bandwidth_result

while [ ${i} -le 2 ]
do
        filebench_by_workload $f $i $dir_name
        filebench_by_workload $w $i $dir_name
        filebench_by_workload $v $i $dir_name
        i=$((i+1))
done
