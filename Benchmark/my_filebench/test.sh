~/usage_write/take_time_client & 

sleep 5

pid=`ps -ef | grep take_time_client | grep -v grep | awk '{print $2}'`

kill -9 $pid
