/*
 * hostapd - command line interface for hostapd daemon
 * Copyright (c) 2004-2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include "wapi_ctrl.h"



static const char *commands_help =
"Commands:\n"
"   start		WAPID start\n"
"   stop		WAPID stop, clean all VAP key\n"
"   restart   	WAPID reload config\n"
"   check_cert path	WAPID check x.509 certificate\n"
"   rekey address	usk rekey\n"
"   help		show this usage help\n"
"   quit                 exit wapitalk\n"
#ifdef DOT11_WAPI_MIB	
"   getmib athX    Show all WAPI MIB of athX\n"
#endif
;

static struct wpa_ctrl *ctrl_conn;
static int wapitalk_quit = 0;
static int wapitalk_attached = 0;
static const char *ctrl_iface_dir = "/var/run/wapid";
static char *ctrl_ifname = NULL;


static void usage(void)
{
	fprintf(stderr, 
		"\n"	
		"usage: wapid [-hv] "
		"[command..]\n"
		"\n"
		"Options:\n"
		"   -h           help (show this usage text)\n"
		"   -v           shown version information\n"
		"/var/run/wapid)\n"
		"   -i<ifname>   Interface to listen on (default: first "
		"interface found in the\n"
		"                socket path)\n\n"
		"%s",
		commands_help);
}


static struct wpa_ctrl * wapitalk_open_connection(const char *ifname)
{
	char *cfile;
	int flen;

	if (ifname == NULL)
		return NULL;

	flen = strlen(ctrl_iface_dir) + strlen(ifname) + 2;
	cfile = malloc(flen);
	if (cfile == NULL)
		return NULL;
	snprintf(cfile, flen, "%s/%s", ctrl_iface_dir, ifname);

	ctrl_conn = wpa_ctrl_open(cfile);
	free(cfile);
	return ctrl_conn;
}


static void wapitalk_close_connection(void)
{
	if (ctrl_conn == NULL)
		return;

	if (wapitalk_attached) {
		wpa_ctrl_detach(ctrl_conn);
		wapitalk_attached = 0;
	}
	wpa_ctrl_close(ctrl_conn);
	ctrl_conn = NULL;
}


static void wapitalk_msg_cb(char *msg, size_t len)
{
	printf("%s\n", msg);
}


static int _wapitalk_command(struct wpa_ctrl *ctrl, char *cmd, int print)
{
	char buf[4096];
	size_t len;
	int ret;

	if (ctrl_conn == NULL) {
		printf("Not connected to hostapd - command dropped.\n");
		return -1;
	}
	len = sizeof(buf) - 1;
	ret = wpa_ctrl_request(ctrl, cmd, strlen(cmd), buf, &len,
			       wapitalk_msg_cb);
	if (ret == -2) {
		printf("'%s' command timed out.\n", cmd);
		return -2;
	} else if (ret < 0) {
		printf("'%s' command failed.\n", cmd);
		return -1;
	}
	if (print) {
		buf[len] = '\0';
		printf("%s", buf);
	}
	return 0;
}


static inline int wapitalk_command(struct wpa_ctrl *ctrl, char *cmd)
{
	return _wapitalk_command(ctrl, cmd, 1);
}


static int wapitalk_cmd_start(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	return wapitalk_command(ctrl, "START");
}
static int wapitalk_cmd_status(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	return wapitalk_command(ctrl, "STATUS");
}

static int wapitalk_cmd_stop(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	return wapitalk_command(ctrl, "STOP");
}

static int wapitalk_cmd_restart(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	return wapitalk_command(ctrl, "RESTART");
}

static int wapitalk_cmd_check_cert(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	char buf[64];
	if (argc != 1) {
		printf("Invalid 'check_cert' command - exactly one argument, certificate file name  is required.\n");
		return -1;
	}
	snprintf(buf, sizeof(buf), "CHECKCERT %s", argv[0]);

	return wapitalk_command(ctrl, buf);
}
static int wapitalk_cmd_rekey(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	char buf[64];
	if (argc != 1) {
		printf("Invalid 'rekey' command - exactly one argument, STA address  is required.\n");
		return -1;
	}
	snprintf(buf, sizeof(buf), "REKEY %s", argv[0]);

	return wapitalk_command(ctrl, buf);
}

static int wapitalk_cmd_help(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	printf("%s", commands_help);
	return 0;
}

static int wapitalk_cmd_quit(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	wapitalk_quit = 1;
	return 0;
}
#ifdef DOT11_WAPI_MIB
extern int wapitalk_cmd_getmib(struct wpa_ctrl *ctrl, int argc, char *argv[]);
#endif

#if 0

static void hostapd_cli_list_interfaces(struct wpa_ctrl *ctrl)
{
	struct dirent *dent;
	DIR *dir;

	dir = opendir(ctrl_iface_dir);
	if (dir == NULL) {
		printf("Control interface directory '%s' could not be "
		       "openned.\n", ctrl_iface_dir);
		return;
	}

	printf("Available interfaces:\n");
	while ((dent = readdir(dir))) {
		if (strcmp(dent->d_name, ".") == 0 ||
		    strcmp(dent->d_name, "..") == 0)
			continue;
		printf("%s\n", dent->d_name);
	}
	closedir(dir);
}
#endif

struct wapitalk_cmd {
	const char *cmd;
	int (*handler)(struct wpa_ctrl *ctrl, int argc, char *argv[]);
};

static struct wapitalk_cmd wapitalk_commands[] = {
	{ "status", wapitalk_cmd_status},
	{ "start", wapitalk_cmd_start },
	{ "stop", wapitalk_cmd_stop },
	{ "restart", wapitalk_cmd_restart },
	{ "check_cert", wapitalk_cmd_check_cert },
	{ "rekey", wapitalk_cmd_rekey },
	{ "help", wapitalk_cmd_help },
	{ "quit", wapitalk_cmd_quit },
#ifdef DOT11_WAPI_MIB	
	{ "getmib", wapitalk_cmd_getmib },
#endif	
	{ NULL, NULL }
};


static void wpa_request(struct wpa_ctrl *ctrl, int argc, char *argv[])
{
	struct wapitalk_cmd *cmd, *match = NULL;
	int count;

	count = 0;
	cmd = wapitalk_commands;
	while (cmd->cmd) {
		if (strncasecmp(cmd->cmd, argv[0], strlen(argv[0])) == 0) {
			match = cmd;
			count++;
		}
		cmd++;
	}

	if (count > 1) {
		printf("Ambiguous command '%s'; possible commands:", argv[0]);
		cmd = wapitalk_commands;
		while (cmd->cmd) {
			if (strncasecmp(cmd->cmd, argv[0], strlen(argv[0])) ==
			    0) {
				printf(" %s", cmd->cmd);
			}
			cmd++;
		}
		printf("\n");
	} else if (count == 0) {
		printf("Unknown command '%s'\n", argv[0]);
	} else {
		match->handler(ctrl, argc - 1, &argv[1]);
	}
}


static void wapitalk_recv_pending(struct wpa_ctrl *ctrl, int in_read)
{
	int first = 1;
	if (ctrl_conn == NULL)
		return;
	while (wpa_ctrl_pending(ctrl)) {
		char buf[256];
		size_t len = sizeof(buf) - 1;
		if (wpa_ctrl_recv(ctrl, buf, &len) == 0) {
			buf[len] = '\0';
			if (in_read && first)
				printf("\n");
			first = 0;
			printf("%s\n", buf);
		} else {
			printf("Could not read pending message.\n");
			break;
		}
	}
}


static void wapitalk_interactive(void)
{
	const int max_args = 10;
	char cmd[256], *res, *argv[max_args], *pos;
	int argc;

	printf("\nInteractive mode\n\n");

	do {
		wapitalk_recv_pending(ctrl_conn, 0);
		printf("> ");
		alarm(1);
		res = fgets(cmd, sizeof(cmd), stdin);
		alarm(0);
		if (res == NULL)
			break;
		pos = cmd;
		while (*pos != '\0') {
			if (*pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		argc = 0;
		pos = cmd;
		for (;;) {
			while (*pos == ' ')
				pos++;
			if (*pos == '\0')
				break;
			argv[argc] = pos;
			argc++;
			if (argc == max_args)
				break;
			while (*pos != '\0' && *pos != ' ')
				pos++;
			if (*pos == ' ')
				*pos++ = '\0';
		}
		if (argc)
			wpa_request(ctrl_conn, argc, argv);
	} while (!wapitalk_quit);
}

static void wapitalk_terminate(int sig)
{
	wapitalk_close_connection();
	exit(0);
}


static void wapitalk_alarm(int sig)
{
	if (ctrl_conn && _wapitalk_command(ctrl_conn, "PING", 0)) {
		printf("Connection to hostapd lost - trying to reconnect\n");
		wapitalk_close_connection();
	}
	if (!ctrl_conn) {
		ctrl_conn = wapitalk_open_connection(ctrl_ifname);
		if (ctrl_conn) {
			printf("Connection to hostapd re-established\n");
			if (wpa_ctrl_attach(ctrl_conn) == 0) {
				wapitalk_attached = 1;
			} else {
				printf("Warning: Failed to attach to "
				       "hostapd.\n");
			}
		}
	}
	if (ctrl_conn)
		wapitalk_recv_pending(ctrl_conn, 1);
	alarm(1);
}


int main(int argc, char *argv[])
{
	int interactive;
	int warning_displayed = 0;
	int c;

	for (;;) {
		c = getopt(argc, argv, "h");
		if (c < 0)
			break;
		switch (c) {
		case 'h':
			usage();
			return 0;
		default:
			usage();
			return -1;
		}
	}

	interactive = argc == optind;

	if (interactive) {
		printf("%s\n\n\n\n", "wapitalk start ...................");
	}

	for (;;) {
		if (ctrl_ifname == NULL) {
			struct dirent *dent;
			DIR *dir = opendir(ctrl_iface_dir);
			if (dir) {
				while ((dent = readdir(dir))) {
					
					if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
						continue;
					ctrl_ifname = strdup(dent->d_name);
					break;
				}
				closedir(dir);
			}
		}
		ctrl_conn = wapitalk_open_connection(ctrl_ifname);
		if (ctrl_conn) {
			if (warning_displayed)
				printf("Connection established.\n");
			break;
		}

		if (!interactive) {
			perror("Failed to connect to hostapd - "
			       "wpa_ctrl_open");
			return -1;
		}

		if (!warning_displayed) {
			printf("Could not connect to hostapd - re-trying\n");
			warning_displayed = 1;
		}
		sleep(1);
		continue;
	}

	signal(SIGINT, wapitalk_terminate);
	signal(SIGTERM, wapitalk_terminate);
	signal(SIGALRM, wapitalk_alarm);

	if (interactive) {
		if (wpa_ctrl_attach(ctrl_conn) == 0) {
			wapitalk_attached = 1;
		} else {
			printf("Warning: Failed to attach to wapid.\n");
		}
		wapitalk_interactive();
	} else
		wpa_request(ctrl_conn, argc - optind, &argv[optind]);

	free(ctrl_ifname);
	wapitalk_close_connection();
	return 0;
}

