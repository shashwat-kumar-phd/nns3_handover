#Script to Get Lines for UE 1
BEGIN {
	#print "Hello"
	#printf("Time\tThroughput\n")
	totalData=0.0
	endTime=0.0
	startTime=0.0
}
{	
	if(NR==2)
	{
		startTime=$1;
	}
	totalData=totalData+$10
	endTime=$1	
	
}
END{
	time=endTime-startTime
	avgThroughput=(totalData*8)/(20*time*1024*1024)
	#printf("%s\t%s\t%s\n",startTime,endTime,totalData)
	printf("\nAverageThrougput\t%s\n",avgThroughput)
}
