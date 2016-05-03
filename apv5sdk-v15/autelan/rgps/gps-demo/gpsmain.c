#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>
#include "dmsfunc.h"

/*********************************** Locals ***********************************/


/****************************** Forward Declarations **************************/
extern void wncqmi_init();
extern void wncqmi_release();

extern void gps_start_req();
extern void gps_stop_req();
extern void gps_set_oper_mode_req(int mode);

extern int total_sv;
extern int total_good_sv;

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */

int main(int argc, char** argv)
{
	int gps_start=0,exit=0;
		
	if(argc==1){
		fprintf(stdout,"Usage: gps [OPTION]\n");
		fprintf(stdout,"Version 1.1\n");
		fprintf(stdout,"-on	open GPS\n");
		fprintf(stdout,"-off	stop GPS\n");
		fprintf(stdout,"-sv	GPS sv statistics info\n");
		return 0;
	}

	do{
		if(0 == strcmp(argv[1], "-on")){
			if(!gps_start){
				wncqmi_init();	
				sleep(1);
				
				initGPSResp();
				sleep(1);

				gps_set_oper_mode_req(4);
				sleep(1);
				
				gps_start_req();
				gps_start=1;
			}
			sleep(5);
		}
		else if(0 == strcmp(argv[1], "-off")){
//			gps_stop_req();
			system("killall -9 gps");

			exit=1;
		}
		else if(0 == strcmp(argv[1], "-sv")){
			if(!gps_start){
				wncqmi_init();
				
				sleep(1);
				
				initGPSResp();
				sleep(1);

				gps_set_oper_mode_req(4);
				sleep(1);
				
				gps_start_req();
				gps_start=1;
			}
			sleep(600);
			
			printf("/--- GPS SV STATISTICS BEGIN ---\n");
			printf("Total SVs: %4d, Good SVs(SNR>30): %4d\n",total_sv,total_good_sv);
			printf("--- GPS SV STATISTICS END ---/\n");
		}
		else{
			exit=1;
		}
	}while(!exit);

//	wncqmi_release();

	return 0;
}

