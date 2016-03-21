#Script to Get Lines for UE 1
BEGIN {
	#print "Hello"
	printf("time\tThroughput\n")
}
{	
	if ( $4=="5" ){

        		printf("%s\t%s\n",$1,($10*8)/(1024*1024*0.25))
		
	}
}
END{

}

