#include <auteos.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	
	char cmd[64] = {0};

	if (argc < 2) {
		autelan_printf("/tmp/load_art PC_ip\n");
		return -1;
	}
	autelan_system("stop_monitor");
	autelan_system("killall wtpd");
	autelan_system("des_wlan");
	autelan_system("rmmod capwap_split_fast");
	autelan_system("/etc/rc.d/rc.wlan down && insmod /lib/modules/2.6.31/net/art-hornet.ko");
	autelan_sprintf(cmd, "cd && tftp -gr nart.out %s && chmod +x nart.out", argv[1]);
	autelan_system(cmd);
	autelan_system("/tmp/nart.out &");
	return 0;
}
