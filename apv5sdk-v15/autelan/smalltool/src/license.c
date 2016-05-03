#include <auteos.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]){

	int license=0;
	if(argc>1)                           
	{
		if( (0 == strncmp(argv[1], "AP2400",strlen("AP2400"))) || (0 == strncmp(argv[1], "AP2600",strlen("AP2600"))) )
		{
			if(0 == strncmp(argv[1]+11, "0100",strlen("0100")))
                        {
				license=1;
			} 
 			else if (0 == strncmp(argv[1]+11, "0500",strlen("0500")))
			{
				license=2;
			}
			else if (0 == strncmp(argv[1]+11, "0101",strlen("0101")))
			{
				license=2;
			} 
			else if(0 == strncmp(argv[1]+11, "0501",strlen("0501"))) 
			{
				
				license=2;
			}
			else
			{
				license=-1;
			}
		}
		else if(strcmp(argv[1], "FH-AP2400-20N") == 0)
			license = 4;
		else if(strcmp(argv[1], "FH-AP2400-20AN") == 0)
			license = 4;
		else if(strcmp(argv[1], "FH-AP2400-27N") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-W-27G") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-W-27AG") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-W-27N") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-W-27AN") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-27G") == 0)
			license = 5;
		else if(strcmp(argv[1], "FH-AP2400-W") == 0)
			license = 4;
		else if(strcmp(argv[1], "FH-AP2400") == 0)
			license = 4;
		else if(strcmp(argv[1], "FH-AP2400-20G") == 0)
			license = 4;
		else if(strcmp(argv[1], "FH-AP2400-20AG") == 0)
			license = 4;
		else;
		
	}

		autelan_printf("%d\n",license);
	
	return 0 ;
}
