i=0
echo "Speed	AvgThroughput"
while [ "$i" -le 5 ]
do
	echo "$i"
	(echo `exec awk -f avgTP.awk PFNF$i.txt`)&
	i=`expr $i + 5`
	sleep 1 
done
