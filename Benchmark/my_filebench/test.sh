#echo $#

#-------------------------------------------------------------------
#nr_cpus=`grep -c processor /proc/cpuinfo`
#mem_cap_cmd=`cat /proc/meminfo | grep MemTotal | awk '{print $2}'`
#mem_cap=`free -g | grep Mem | awk '{print $2}'`
#mem_cap=`expr $mem_cap + 1`
#echo $nr_cpus
#echo $mem_cap
#dir_name=$nr_cpus'_'$mem_cap
#echo $dir_name

#------------------------------------------------------------------

#fname="test_file"
#dirname="test"
#fpath=$dirname'/results'
#echo "asdfasdfasdf" >> $fpath

#if [ ! -d $dirname ]
#then
#	echo "$dirname is not exist"
#fi

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
	echo "There is no directory : $dirname."
	mkdir $dirname
fi
