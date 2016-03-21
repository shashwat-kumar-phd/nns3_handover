#Script to run pdr.awk iterativly
i=11
while [ "$i" -le 15 ]
do
	#echo "PDR for RngRun = $i"
	(echo `exec awk -f tput.awk ./PF/0NF$i/DlPdcpStats.txt`)&
	i=`expr $i + 1`
	sleep 1 
done
