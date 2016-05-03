#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void do_cmd(char *cmd)
{
	autelan_system(cmd);
	memset(cmd, 0, 64);
}

int main(int argc, char **argv)
{
	char cmd[64] = {0};
	
	if (argc < 6) {
		autelan_printf("create_wlan wlan_name mode essid channel txpower\n");
		return -1;
	}	
	

	autelan_system("stop_monitor > /dev/null 2>&1");
	autelan_system("killall wtpd > /dev/null 2>&1");
	autelan_system("echo 0 > /proc/sys/dev/wifi0/thinap");

	autelan_sprintf(cmd, "wlanconfig %s destroy", argv[1]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "wlanconfig %s create wlandev wifi0 wlanmode ap", argv[1]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "iwpriv %s mode %s", argv[1], argv[2]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "iwpriv %s disablecoext 1", argv[1]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "iwconfig %s essid %s", argv[1], argv[3]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "iwconfig %s channel %s", argv[1], argv[4]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "iwconfig %s txpower %s", argv[1], argv[5]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "brctl addif default %s", argv[1]);
	do_cmd(cmd);

	autelan_sprintf(cmd, "ifconfig %s up", argv[1]);
	do_cmd(cmd);
	
	return 0;
}
