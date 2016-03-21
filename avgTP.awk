BEGIN {
	#print "Hello"
	#printf("Time\tThroughput\n")
	totalData=0.0
}
{	
	
	totalData=totalData+$2
	
}
END{
	avgThroughput=(totalData)/5
	#printf("%s\t%s\t%s\n",startTime,endTime,totalData)
	printf("%s",avgThroughput)
}
