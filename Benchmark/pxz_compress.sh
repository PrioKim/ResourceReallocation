#!/bin/sh

start_time=`date +%s.%N`
start_time_string=`date`
############################################ my_routine start

for i in $*
do
    echo `pxz -kvf -2 -T16 $i`
done

#pxz -kvf -2 -T16 enwik9
############################################
end_time=`date +%s.%N`
end_time_string=`date`
elapsed_time=`echo "$end_time - $start_time" | bc`
htime=`echo "$elapsed_time/3600" | bc`
mtime=`echo "($elapsed_time/60) - ($htime*60)" | bc`
stime=`echo "$elapsed_time - (($elapsed_time/60)*60)" | bc`

echo "======================================"
echo "Start time : $start_time_string"
echo "End time : $end_time_string"
echo "Total time : ${htime}:${mtime}:${stime}"
echo "======================================"

#for i in $*
#do
#    echo `rm -rf $i.xz`
#done
