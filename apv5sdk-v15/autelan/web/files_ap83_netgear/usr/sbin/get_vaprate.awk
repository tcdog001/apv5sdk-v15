# cat iwlist.txt | awk -f test.awk | awk '{b=NF-1; print $b }' | tail -n1

BEGIN{
    startflag=0;
    stopflag=0;
}
{
	if($0 ~ /Bit Rates/)
	{
           startflag=1;
        }
	if($0 ~ /Extra/)
	{
	   stopflag=1;
	}
	if( startflag == 1 && stopflag == 0)
	{
	   print $0;
	}
}
END{
}
