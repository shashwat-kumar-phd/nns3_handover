#Script to Get Lines for UE 1
BEGIN {
	#print "Hello"
	printf("time\tThroughput\n")
}
{	
	if ( $4=="1" ){
		# Keep count of the fields in second column
    		#count[$6]++;

    		# Save the line the first time we encounter a unique field
    		#if (count[$6] == 1)
        		printf("%s\t%s\n",$1,$8*8/(1024*1024*0.25))
		
	}
}
END{

}

