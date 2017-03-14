fname="fileserver.f-0"
summary=`cat $fname | grep "IO Summary"`
summary=$fname' : '$summary
echo $summary >> results
