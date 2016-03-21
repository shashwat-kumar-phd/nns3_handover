#Script to Get Lines for UE 1
BEGIN {
	#print "Hello"
	printf("time\tsinr\n")
}
{	
	if ( $3=="1" ){
		# Keep count of the fields in second column
    		#count[$6]++;

    		# Save the line the first time we encounter a unique field
    		#if (count[$6] == 1)
        		printf("%s\t%s\n",$1,log($6)/2.302585093*10)
		
	}
}
END{

}

