#include "spectral_defs.h"
#include <signal.h>
#include <errno.h>

struct sockaddr_nl src_addr, dst_addr;
struct nlmsghdr *nlh = NULL;
struct msghdr msg;
struct iovec iov;

int spectral_debug_level=ATH_DEBUG_SPECTRAL;

int send_single=0;
int sock_fd;
 int global_do_eacs=0;
 int global_current_freq=2462;
 int global_prev_freq=2462;
 int global_minpwr=-100;
 int global_changefreq=0;
 int global_changechannel=0;
 int global_rawfft=0;
 int global_scaledata=0;
 int global_userssi=1;
 int global_flip=0;
 int global_indexdata=0;
 int global_onlyrssi=0;
 int global_is_classify=0;
 int total_send=0;

 char *samp_resp;
 char samp_msg[1024];

 struct ss lwrband;
 struct ss uprband;



eSPECTRAL_SERVER_STATE current_server_state = SPECTRAL_SERVER_STATE_IDLE;
int total_channel_switches;
int num_sent, num_saved;
int num_responses_reqd=0, num_to_save=0;
SPECTRAL_SAMP_MSG global_fake_samp_msg;
int global_gui_fd=0;

int global_num_alarms=0;

void print_usage(void)
{
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"SPECTRALDAEMON - Usage\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"spectraldaemon offers the following command line options\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"channels - use this when using the AD GUI, this switches the AP between channels 5 and 11\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"userssi - this will use RSSI in the power calculations\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"classify - this will run the spectraldaemon in classifier mode on the current channel - DO NOT use with any other option \n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"eacs - this will run the EACS algorithm - DO NOT use with any other option \n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"rawfft - this will report raw FFT data as is to the GUI, used for debug only\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"minpwr <value> - this will not report any power values below the minimum power level specified, use this to get a smoother spectral view. Power values below the minpwr will be set to minpwr value\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"maxhold <value> - this will set the max hold interval to value specified\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"debug <1/2/3> - this will set the debug level to the value specified\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"\n");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Running spectraldaemon without any options is equivalent to running classification on the current channel.\n");

}
static void spectral_alarm_handler(int sig)
{
//Fri Feb  5 16:44:47 PST 2010 - In classify mode, this will be used to send interspersed data for both channels
    if (!global_is_classify) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s curr_freq=%d\n",__func__, global_current_freq);
    } else {
        global_num_alarms++;
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s CLASSIFY scan curr_freq=%d\n",__func__, global_current_freq);
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d total_channel_switches=%d\n", __func__, __LINE__, total_channel_switches);
        ualarm(200000, 0);
   }
}

void stop_scan(void)
{
    system("spectraltool stopscan");
    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"############%s curr_freq=%d total_send=%d############\n",__func__, global_current_freq, total_send);
}

int main(int argc, char *argv[])
{
    int listener, new_fd, newfd, addrlen;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in remoteaddr; // connector's address information
    int sin_size, fdmax, i, nbytes, yes, j, nl_sock_fd;
    u_int8_t buf[MAX_PAYLOAD]={'\0'}, *tmp=NULL;;
    fd_set master;
    fd_set read_fds;
    struct sockaddr_nl src_addr, dest_addr;
    socklen_t fromlen;
    struct nlmsghdr *nlh = NULL;
    int read_bytes, paramvalue;
    struct msghdr msg;
    u_int8_t param[255]={'\0'};
    int on=0;

    SPECTRAL_SAMP_MSG *nl_samp_msg;
    SPECTRAL_SAMP_DATA *nl_samp_data;
    i = 1;
    while (i < argc) {
	if(streq(argv[i], "rawfft")) 
	    global_rawfft=1;
	if(streq(argv[i], "scaledata")) 
	    global_scaledata=1;
	if(streq(argv[i], "userssi")) 
	    global_userssi=1;
	if(streq(argv[i], "flip")) 
	    global_flip=1;
	if(streq(argv[i], "indexdata")) 
	    global_indexdata=1;
	if(streq(argv[i], "onlyrssi")) 
	    global_onlyrssi=1;
	if(streq(argv[i], "eacs")) 
	    global_do_eacs=1;
	if(streq(argv[i], "channels")) 
	    global_changechannel=1;
	if(streq(argv[i], "classify")) 
	    global_is_classify=1;
	if(streq(argv[i], "debug"))  {
	    spectral_debug_level=(ATH_DEBUG_SPECTRAL << (atoi(argv[i+1])));         
            i++;
        }
	if(streq(argv[i], "minpwr"))  {
	    global_minpwr=(atoi(argv[i+1]));         
            i++;
        }
	if(streq(argv[i], "maxhold"))  {
	    global_maxhold_interval=(atoi(argv[i+1]));         
            i++;
        }
	if(streq(argv[i], "help"))  {
	    print_usage();
            exit(0);
        }
        i ++;;
    }

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"SPECTRAL Server is up ... built at %s %s\n", __DATE__, __TIME__);

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"eacs=%d debug=%d rawfft=%d scaledata=%d userssi=%d (100-x)=%d indexdata=%d onlyrssi=%d change_channels=%d minpwrlevel=%d\n", global_do_eacs, spectral_debug_level, global_rawfft, global_scaledata, global_userssi, global_flip, global_indexdata, global_onlyrssi, global_changechannel, global_minpwr);

    if (argc == 1 || global_is_classify) {
        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL, "Running in classifier mode to monitor current channel\n");
        global_is_classify=1;
    }


    if (global_do_eacs==1) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL, "EACS mode, only monitor current channel\n");
    }

    listener = socket(PF_INET, SOCK_STREAM, 0); // do some error checking!

    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {       SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"socket option failed\n");
        exit (0);
    }


    memset(&my_addr, 0, sizeof(my_addr));
     /* interested in group 1<<0 */
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(ATHPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // auto-fill with my IP

    // don't forget your error checking for these calls:
    bind(listener, (struct sockaddr *)&my_addr, sizeof(my_addr));

    listen(listener, BACKLOG);

    sin_size = sizeof (remoteaddr);

    nl_sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_ATHEROS);
    if (nl_sock_fd < 0) {
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"socket errno=%d\n", nl_sock_fd);
            return nl_sock_fd;
    }

     memset(&src_addr, 0, sizeof(src_addr));
     src_addr.nl_family = PF_NETLINK;
     src_addr.nl_pid = getpid();  /* self pid */
     /* interested in group 1<<0 */
     src_addr.nl_groups = 1;
    
    if(read_bytes=bind(nl_sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) < 0) {
        if (read_bytes < 0)
                        perror("bind(netlink)");
            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"BIND errno=%d\n", read_bytes);
            return read_bytes;
    }

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG)));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(SPECTRAL_SAMP_MSG));
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.nl_family = PF_NETLINK;
    dst_addr.nl_pid = 0;  /* self pid */
     /* interested in group 1<<0 */
    dst_addr.nl_groups = 1;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dst_addr;
    msg.msg_namelen = sizeof(dst_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(nl_sock_fd,&master);
    FD_SET(listener,&master);

    fdmax = ((listener > nl_sock_fd) ? listener : nl_sock_fd);

    current_server_state = SPECTRAL_SERVER_STATE_IDLE;
    total_channel_switches=0;
    init_freq_message_pools();
    init_dummy_samp_msg();
    if (!global_do_eacs) 
        signal(SIGALRM, spectral_alarm_handler);
    else
        start_eacs_monitor_scan();

    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d fdmax=%d listener=%d nl_sock_fd=%d \n", __func__, __LINE__, fdmax, listener, nl_sock_fd);
 for(;;) {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
            if(errno == EINTR) {
                SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d interrupted .....\n", __func__, __LINE__);
               
                //Fri Feb  5 16:44:23 PST 2010 Only in non-classify case, send a dummy message
                 if (!global_is_classify) {
                    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d ALARM \n", __func__, __LINE__);
                    update_dummy_samp_msg(global_current_freq);
                    server_send_maxrssi_samp_data_to_gui(&global_fake_samp_msg, newfd);
                } else {
                    SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"%s %d total_channel_switches=%d client_fd=%d\n", __func__, __LINE__, total_channel_switches, newfd);
                    if (total_channel_switches > 1) 
                        classify_mode_server_send_samp_data_to_gui(NULL, newfd);
                }
            } else {
                perror("select");
                exit(1);
            }
            continue;
        } // if select

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) { // we got one!!
                if (i == listener) {
                    // handle new connections
                    addrlen = sizeof(remoteaddr);
                    if ((newfd = accept(listener, \
                        (struct sockaddr *)&remoteaddr, &addrlen)) == -1) { 

                        perror("accept");
                    } else {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax) {    // keep track of the maximum
                            fdmax = newfd;
                        }
                        /* Mon Feb  8 11:09:01 PST 2010 Since we need to use the alarm_handler to send data to the GUI when in classify mode, we need access to the GUI fd globally. There MUST be a better way to do this */
                        global_gui_fd = newfd;
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"DEMOserver: new connection from %s on " \
                            "socket %d\n", \
                            inet_ntoa(remoteaddr.sin_addr), newfd);
                            current_server_state = SPECTRAL_SERVER_STATE_GUI_CONNECTED;
                    }
                } else {
                    // handle data sent up by kernel on the netlink socket
                    if (i == nl_sock_fd) {
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d NETLINK DATA!!!\n", __func__, __LINE__);
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d nl_fd=%d, newfd=%d listener=%d\n",__func__, __LINE__, nl_sock_fd, newfd, listener);
                        if (current_server_state == SPECTRAL_SERVER_STATE_SETUP_SCAN)
                            current_server_state = SPECTRAL_SERVER_STATE_SCAN;

                        tmp=server_parse_samp_data_msg(buf, &nbytes, newfd, listener, nl_sock_fd);
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d current_server_state = %d total_chan_switches=%d\n",__func__, __LINE__, current_server_state, total_channel_switches);
                        if ((total_channel_switches % 2) != 0) {
                            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL2,"%s %d current_server_state = %d\n",__func__, __LINE__, current_server_state);
                        } else 
                            current_server_state = SPECTRAL_SERVER_STATE_SCAN_COMPLETE;
                        continue;
                    }
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"selectserver: socket %d hung up\n", i);
                            SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL,"Stop spectral scan ....\n");
                            stop_scan();
                            current_server_state = SPECTRAL_SERVER_STATE_IDLE;
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    } else {
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"REQ from the client %d bytes \n", nbytes);    
                        SPECTRAL_DPRINTF(ATH_DEBUG_SPECTRAL1,"clientfd=%d listener=%d netlinkfd=%d\n", i, listener, nl_sock_fd);
                        current_server_state = SPECTRAL_SERVER_STATE_SETUP_SCAN;
                        if (!global_is_classify)
                            server_parse_samp_req_msg(buf, &nbytes, i, listener, nl_sock_fd);
                        continue;
                        }
                    }
                } //if (FD_ISSET(i, &read_fds))
            } //for(i = 0; i <= fdmax; i++)
        } // for(;;)
    close(listener);
    close(newfd);
    free(nlh);
    return 0;
}

