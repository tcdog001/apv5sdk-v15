/**
 *    Name       : proxy.c
 *    
 *    Purpose    : Host proxy implementation file.
 *
 *    Copyright (c) 2009 Sierra Wireless, Inc.  All rights reserved
 *
 */
#include "proxy.h"

/*global variables*/
int32_t socket_port = -1;
char tty_port[PORTNAME_LENGTH];
char *prgname;
int32_t serv_sockfd;
uint32_t numofclients = 0;
int32_t clientsockfd_list[MAX_PROXY_CLIENTS];
int32_t devicefd = -1;
int32_t offlinesigfd[2];
int32_t highestfd;
int32_t pipefd[2];
#define FALSE 1
#define TRUE  0
bool g_verbose = FALSE;
bool bNmeaProxy = FALSE;
bool bNoDevice = TRUE;

pthread_t read_sockets_thread = 0, write_sockets_thread =0 , read_device_thread = 0;
pthread_t watchdog_thread = 0;
pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t zeroclients_mutex;
pthread_cond_t zeroclients_cv;
struct sigaction sig_int;
//static char AT_flag = 0;

int main(int argc, char* argv[])
{
    int32_t c/*, retval*/;
    struct sockaddr_in serv_addr;
    fd_set fdmask;
    size_t rv;
    int16_t res;
    int32_t sig, opts;
    
    prgname = argv[0];
        
    read_sockets_thread = 0;
    write_sockets_thread = 0;
    read_device_thread = 0;
    watchdog_thread = 0;

    while( (c = getopt(argc, argv, "hvp:t:") ) != -1 )
    {
        switch(c)
        {
            case 'h':
            PrintUsage();
            exit(0);
            break;
            
            case 'v':
            g_verbose = TRUE;
            break;

            case 'p':
            socket_port = atoi(optarg);
            break;

            case 't':
            strncpy(tty_port,optarg,PORTNAME_LENGTH);
            break;
            
            case '?':
            fprintf(stderr, "Unrecognized Option - %c \n", optopt );
            PrintUsage();
            exit(1);
            break;

            default:
            PrintUsage();
            exit(1);
        }        
    }
    
    /*verifying the validity of all the cmd line args*/
    if( socket_port < 0)
    {
        PXY_LOG_MSG(LC_LOG_INFO,"Error: Server Socket port number cannot be negative\n");
        exit(1);
    }
    {
        c = strcmp(tty_port, (char*)DIAG_PORT);
        if( c == 0 )
        {
        /*
            retval=log_init(LC_LOG_IDENT_AT);
            if(retval<0)
            {
                printf(" log_init failed\n");
            }
            */
        }
        else
        {
            c = strcmp(tty_port, (char*)MODM_PORT);
            if( c == 0 )
            {
                //AT_flag = 1;
            /*
                retval=log_init(LC_LOG_IDENT_AT);
                if(retval<0)
                {
                    printf(" log_init failed\n");
                }*/
            }
            else
            {
                printf("Error: Invalid ttyUSB port specified\n");
                exit(1); 
            }
        }
    }
    
    /*Create a pipe*/
    res = pipe((int*)pipefd);
    if( res == -1)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: Cannot open the pipe");
        close(devicefd);
        exit(1);
    }
    /* Make the pipe non blocking to make it better*/
    opts = fcntl(pipefd[0],F_GETFL);
    if (opts < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (get) failed on pipefd[0] descriptor");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(pipefd[0],F_SETFL,opts) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (set) failed on pipefd[0] descriptor");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }

    opts = fcntl(pipefd[1],F_GETFL);
    if (opts < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (get) failed on pipefd[1] descriptor");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(pipefd[1],F_SETFL,opts) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (set) failed on pipefd[1] descriptor");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(1);
    }

    

    sig_int.sa_flags = 0;    
    /*Create a mostly open mask -- only masking SIGINT*/
    sigemptyset(&sig_int.sa_mask);
    sigaddset(&sig_int.sa_mask, SIGINT);
    sigaddset(&sig_int.sa_mask, SIGUSR1);
    sig_int.sa_handler = ExitProxy;
    
    /*Install sinal handler*/
    sig = sigaction(SIGINT,&sig_int, 0);
    if(sig == -1 )
    {
        PXY_LOG_MSG(LC_LOG_INFO,"Error: could not install signal handler for SIGINT");
    }
    
    /*clear the client socket list*/
    memset( (char*)&clientsockfd_list[0], 0, sizeof(clientsockfd_list) );
    
    pthread_mutex_init(&zeroclients_mutex, NULL);
    pthread_cond_init (&zeroclients_cv, NULL);


    /*Create proxy server socket */
    serv_sockfd = CreateServerSocket(&serv_addr);
    if( serv_sockfd < 0 )
    {
        PXY_LOG_MSG(LC_LOG_INFO,"Error: CreateServerSocket failed\n");
        exit(1);
    }
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Server socket created\n");
    if( listen(serv_sockfd, 5) == -1 )
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: Listening on server socket failed"); 
        exit(1);
    }
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Server socket listening\n");

    
    /*wait for client requests*/
    while(1)
    {
        FD_ZERO(&fdmask);
        FD_SET(serv_sockfd, &fdmask);
        
        do {
            rv = select(serv_sockfd + 1, &fdmask, NULL, NULL, NULL);
        } while (rv < 0 && errno == EINTR);
        
        if( rv < 0 )
        {
            PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on server socket failed, exiting...");
            break;
        }
        if( rv == 0 )   
            continue;

        if( FD_ISSET(serv_sockfd, &fdmask) )
        {
            if( !read_device_thread )
            {
                /* Create device thread*/
                res =  pthread_create(&read_device_thread,NULL,ReadDeviceThread, NULL );
                if( res != 0 )
                {
                    PXY_LOG_MSG(LC_LOG_ERROR,"Error: Create read_device_thread failed ");
                    close(pipefd[0]);
                    close(pipefd[1]);
                    exit(1);
                }
            }    

           PXY_LOG_MSG(LC_LOG_INFO,"Info: Server socket received a client request\n");

           HandleNewClient();
           /*if this is the first client launch 
            * socket read/write thread*/
            if( !read_sockets_thread && !write_sockets_thread)
            {
                rv =  pthread_create(&read_sockets_thread,NULL,ReadClientSocketThread, NULL );
                if( rv != 0 )
                {
                    PXY_LOG_MSG(LC_LOG_ERROR,"Error: Create read_sockets_thread failed ");
                    CloseAllClients();
                    close(serv_sockfd);
                    exit(1);
                }
                
                rv =  pthread_create(&write_sockets_thread,NULL,WriteClientSocketThread, NULL );
                if( rv != 0 )
                {
                    PXY_LOG_MSG(LC_LOG_ERROR,"Error: Create write_sockets_thread failed ");
                    CloseAllClients();
                    close(serv_sockfd);
                    exit(1);
                }
                
                PXY_LOG_MSG(LC_LOG_INFO,"Info: Read/Write socket threads created\n");
            }
            pthread_mutex_lock(&zeroclients_mutex);
            pthread_cond_signal(&zeroclients_cv);
            pthread_mutex_unlock(&zeroclients_mutex);
            
        }
        
    }
    close(serv_sockfd);
    return 0;
}
/*
 * Name:     PrintUsage
 *
 * Purpose:  This function displays the help menu for the program.
 *
 * Parms:    None.
 *
 * Return:   None.
 *
 * Notes:    None
 *
 */
void PrintUsage()
{
    printf("Usage:\n");
    printf("------------------------------------------------------\n");
    printf("%s [-v -p socket_port_name -t fullpath_tty_port]\n", prgname);
    printf("Help Mode:   %s -h \n", prgname );
    printf("For example: %s -v -p 5500 -t /dev/ttyUSB0\n", prgname); 
    printf("------------------------------------------------------\n");
    return ;
}

/*
 * Name:     CreateServerSocket
 *
 * Purpose:  This function creates and binds the proxy server socket.
 *
 * Parms:    None.
 *
 * Return:   Server socket file descriptor.
 *
 * Notes:    None
 *
 */
int32_t CreateServerSocket(struct sockaddr_in *serv_addr)
{
    int32_t sockfd = -1;
    int32_t opts;
    int32_t reuse_addr = 1;
    /*Create server socket*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 )
        return sockfd;
        
    bzero((char *) serv_addr, sizeof(*serv_addr));
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr->sin_port = htons(socket_port);
    
    if (bind(sockfd, (struct sockaddr *)serv_addr,sizeof(*serv_addr)) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: Cannot bind to server socket");
        close(sockfd);
        return -1;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,sizeof(reuse_addr)) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: Cannot set server socket addr options");
        close(sockfd);
        return -1;
    }
    /* Make the socket non blocking to make it better*/
    opts = fcntl(sockfd,F_GETFL);
    if (opts < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (get) failed on server socket");
        close(sockfd);
        return -1;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sockfd,F_SETFL,opts) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (set) failed on server socket");
        close(sockfd);
        return -1;
    }
    
    return sockfd;
}

/*
 * Name:     OpenDevice
 *
 * Purpose:  This function opens the ttyport and sets the port attributes.
 *
 * Parms:    ttyport - Complete path of ttyport.
 *
 * Return:   Serial port file descriptor.
 *
 * Notes:    None
 *
 */
int32_t OpenDevice(char* ttyport)
{
    int32_t fd = -1;
    struct termios term;
    
    fd = open(tty_port, O_RDWR | O_NOCTTY );
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Device NOT in OFFLINE/STANDBY mode; OPEN Device\n");
    
    if( fd < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Failed to open a device");
        return fd;
    }
    
    bzero(&term, sizeof(struct termios));
  
    term.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    term.c_iflag = IGNPAR;
    term.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    term.c_lflag = 0;

    term.c_cc[VTIME]    = 1;   /* inter-character timer */
    term.c_cc[VMIN]     = 1;   /* blocking read until n chars received */
    
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW, &term);
    
    return fd;
}

/*************
 *
 * Name: HandleNewClient
 *
 * Purpose:  Handle new incoming connections
 *           
 * Params:   None.
 *
 * Return:   No value
 *
 **************/
void HandleNewClient()
{
    
    int32_t clientfd = -1;
    int32_t i, opts;
    
    clientfd = accept(serv_sockfd, NULL, NULL );

    PXY_LOG_MSG(LC_LOG_ERROR,"################HandleNewClient start ###############"); 

    if( clientfd < 0 )
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: accept failed");
        return;
    }
    else
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"clientfd is %d here.",clientfd); 
    }

	/*
    if(1 == AT_flag)
    {
        system("killall -SIGTERM log_server >/dev/null 2>&1"); 
        system("killall -SIGTERM modem_server >/dev/null 2>&1"); 
        system("killall -SIGTERM slicmgt >/dev/null 2>&1"); 
    }
	*/
    /* Make the socket non blocking to make it better*/
    opts = fcntl(clientfd,F_GETFL);
    if (opts < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (get) failed on client socket");
        close(clientfd);
        return;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(clientfd,F_SETFL,opts) < 0)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: fcntl (set) failed on client socket");
        close(clientfd);
        return;
    }
    pthread_mutex_lock( &client_list_mutex );
    /* check if we have reached max client list*/
    for( i = 0; i < MAX_PROXY_CLIENTS && clientfd != -1; i++ )
    {
        if( clientsockfd_list[i] == 0 )
        {
            PXY_LOG_MSG(LC_LOG_ERROR,"clientfd is %d here.",clientfd); 
            highestfd = clientfd+1;
            clientsockfd_list[i] = clientfd;
            clientfd = -1;
            PXY_LOG_MSG(LC_LOG_ERROR,"clientfd is %d here.",clientfd); 
            
        }
    }
    pthread_mutex_unlock( &client_list_mutex );
    if( clientfd != -1 )
    {
        /* Server has reached the limit of Max client*/
        close(clientfd);
        PXY_LOG_MSG(LC_LOG_ERROR,"Info:  Server reached the limit of max clients, connection refused"); 
    }
    else
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"clientfd is %d here.",clientfd); 
    }
    PXY_LOG_MSG(LC_LOG_ERROR,"################ HandleNewClient END!!!! ###############"); 
    
}

/*************
 *
 * Name: CloseAllClients
 *
 * Purpose:  Close all client connections
 *           
 * Params:   None.
 *
 * Return:   No value
 *
 **************/
void CloseAllClients()
{
    int32_t i;
    
    pthread_mutex_lock( &client_list_mutex );
    for( i = 0; i < MAX_PROXY_CLIENTS; i++ )
    {
        if( clientsockfd_list[i] != 0 )
        {
            close(clientsockfd_list[i]);
            clientsockfd_list[i] = 0;
        }
    }
    numofclients = 0;
    pthread_mutex_unlock( &client_list_mutex );
}

/*************
 *
 * Name: ReadClientSocketThread
 *
 * Purpose:  Thread that read incoming data from clients sockets and
 *           writes it to a device.
 *           
 * Params:   param - dummy argument not used..
 *
 * Return:   No value
 *
 **************/
void* ReadClientSocketThread( void* param )
{
    fd_set clientset, deviceset;
    int32_t i, rv, rv1, read, written, retry = 0;
    char buf[MAX_BUF_SIZE];
    int32_t bZeroClients = 1;
        
    while( 1 )
    {
        bZeroClients = 1;
        FD_ZERO(&clientset);
        pthread_mutex_lock( &client_list_mutex );
        for( i = 0; i < MAX_PROXY_CLIENTS; i++ )
        {
            if( clientsockfd_list[i] != 0 )
            {
                FD_SET(clientsockfd_list[i], &clientset );
                /*client found*/
                pthread_mutex_lock( &zeroclients_mutex );
                bZeroClients = FALSE;
                pthread_mutex_unlock( &zeroclients_mutex );
            }
        }
        pthread_mutex_unlock( &client_list_mutex );
        
        if( !bZeroClients )
            rv = select(highestfd, &clientset, NULL, NULL, NULL);
        else
        {
            pthread_mutex_lock(&zeroclients_mutex);
            PXY_LOG_MSG(LC_LOG_INFO,"Info: No client active, blocking read socket thread\n");
            pthread_cond_wait(&zeroclients_cv, &zeroclients_mutex);
            pthread_mutex_unlock(&zeroclients_mutex);
            PXY_LOG_MSG(LC_LOG_INFO,"Info: read socket thread unblocked\n");
            continue;
        }            

        if( rv < 0 )
        {
            PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on client socket set failed");
            continue;
        }
        else if( rv == 0 )  /*Only in case of timeout and zero client*/ 
            continue;
        
        /*else rv > 0 */
        pthread_mutex_lock( &client_list_mutex );
        for( i = 0; i < MAX_PROXY_CLIENTS; i++ )
        {
            if( FD_ISSET(clientsockfd_list[i], &clientset ) )
            {
                /*read message*/
                do
                {
                    read = recv(clientsockfd_list[i],(void*) buf, sizeof(buf), MSG_NOSIGNAL);
                } while (read < 0 && errno == EINTR);
    
                if (read < 0)
                {
                    if(errno == ECONNRESET )
                    {
                      /*this means peer has close
                     * remove it from socket list*/
                    PXY_LOG_MSG(LC_LOG_ERROR,"client socket read (r) failed and clientsockfd_list[%d] is %d.",i,clientsockfd_list[i]);
                     close(clientsockfd_list[i]);
                     clientsockfd_list[i] = 0; 
                    }
                    PXY_LOG_MSG(LC_LOG_ERROR,"client socket read (r) failed");
                    continue;
                }
                else if( read == 0 )
                {
                    /*this means peer has close
                     * remove it from socket list*/
                    PXY_LOG_MSG(LC_LOG_ERROR,"client socket read (r) failed and clientsockfd_list[%d] is %d.",i,clientsockfd_list[i]);
                    close(clientsockfd_list[i]);
                     clientsockfd_list[i] = 0;
                    PXY_LOG_MSG(LC_LOG_ERROR,"client socket read (r) failed");
                    continue;
                }
                /*else read > 0 */
                /*check if ttyport descriptor is valid*/
                while( devicefd < 0 && retry < MAX_MODEM_RETRIES )
                {
                    delay(1000);
                    retry++;
                }
                
                if( devicefd > 0 )
                {
                    FD_ZERO(&deviceset);
                    FD_SET(devicefd, &deviceset );
                    rv1 = select(devicefd + 1, NULL, &deviceset, NULL, NULL);
                    if( rv1 <= 0 )
                    {
                       PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on device fd failed(RST)");
                    }
                    else if( FD_ISSET(devicefd, &deviceset ) )
                    {
                        /*write the actual data to device*/
                        written = write(devicefd, buf, read);
                        if( written < 0 )
                        {
                            PXY_LOG_MSG(LC_LOG_ERROR,"Error: write failed on device");
                        }
                    }
                }
                else
                {
                    PXY_LOG_MSG(LC_LOG_INFO,"Info: No device available , discarding data");
                } 
            }    
        }
        pthread_mutex_unlock( &client_list_mutex );

    }/*end of while*/
    
    return (void*)0;
}
/*************
 *
 * Name: WriteClientSocketThread
 *
 * Purpose:  Thread that reads incoming data from pipe and
 *           writes it to list of client sockets.
 *           
 * Params:   param - dummy argument not used..
 *
 * Return:   No value
 *
 **************/
void* WriteClientSocketThread( void* param )
{
    

    fd_set clientset, queueset;
    int32_t i, rv, rv1, rd,  bContinue = 1;
    char buf[MAX_BUF_SIZE];

    
    while(bContinue)
    {
        FD_ZERO(&queueset);
        FD_SET(pipefd[0], &queueset );
        
        rv1 = select(pipefd[0] + 1, &queueset,NULL, NULL, NULL);
        
        if( rv1 < 0 )
        {
            PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on pipe read fd failed(WST)");
            exit(1);
        }
        else if( FD_ISSET(pipefd[0], &queueset ) )
        {
            memset((void*)buf, 0, sizeof(buf) );
            /*read the actual data from device*/
            rd = read(pipefd[0], buf, sizeof(buf) );
            if( rd < 0 )
            {
                PXY_LOG_MSG(LC_LOG_ERROR,"Error: read failed on pipe read end");             
                continue;
            }
            else if( rd == 0 )
            {
                continue;
            }

            /*If we are here that means read is successful
             * so we can send the data to clients*/
             
            pthread_mutex_lock( &client_list_mutex );              
            for( i = 0; i < MAX_PROXY_CLIENTS; i++ )
            {
                if( clientsockfd_list[i] != 0 )
                {
                    FD_ZERO(&clientset);
                    FD_SET(clientsockfd_list[i], &clientset );
                    rv = select(clientsockfd_list[i] + 1, NULL, &clientset, NULL, NULL);
                    if( rv < 0 )
                    {
                       PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on client fd failed(WST)");
                    }
                    else if( FD_ISSET(clientsockfd_list[i], &clientset ) )
                    {
                        /*write the actual data to device*/
                        WriteToClient(&clientsockfd_list[i], buf, rd );
                       
                    }  
                }
            }
            pthread_mutex_unlock( &client_list_mutex );
        }
    }
    return (void*)0;
}

/*************
 *
 * Name: WriteToClient
 *
 * Purpose:  Send data in buffer to socket with descriptor s
 *           
 * Params:   s - pointer to socket file descriptor.
 *           buffer - buffer containing data to be sent.
 *           lenght - length of buffer
 *
 * Return:   No value
 *
 **************/
void WriteToClient(int32_t* s, void* buffer , size_t length )
{
    int32_t retry = 0, sent = 0;
    
       
    while(1)
    {
        sent = send(*s, buffer, length, MSG_EOR | MSG_NOSIGNAL );
        if( sent >= 0 )
        {
            if( sent < length )
            {
                PXY_LOG_MSG(LC_LOG_ERROR,"Error: bytes sent is less than bytes read");
            }
            break;
        }
        else if (errno == EINTR )
        {
            /*just retry*/
        }
        else if ( errno == ECONNREFUSED )
        {
            if(!retry)
                retry++; /*just retry one more time*/
            else
            {
                PXY_LOG_MSG(LC_LOG_ERROR,"Error: write to client socket failed");
                return;
            }   
        }
        else if (errno == EPIPE )
        {
            /* peer has been closed*/
            PXY_LOG_MSG(LC_LOG_ERROR,"Error: write to client socket failed");
            *s = 0;
            return;
        }
    }

    return;
}

/*************
 *
 * Name: delay
 *
 * Purpose: Block for a specified time.
 *           
 * Params:  timeout - timeout in ms
 *
 * Return:  No value
 *
 * Abort:   none
 *
 * Notes:   none
 *
 **************/

void delay(uint32_t timeout)
{
    struct timespec timev;
    struct timespec timer;
    int32_t rv;

    timer.tv_sec = timeout / 1000UL;
    timer.tv_nsec = (timeout * 1000000UL) % 1000000000UL;
    
    do
    {
        timev = timer;        /* remaining time */
        rv = nanosleep(&timev, &timer);
        
    } while (rv < 0 && errno == EINTR);
}


/*************
 *
 * Name: ReadDeviceThread
 *
 * Purpose:  Thread that read incoming data from device and
 *           writes it to a pipe.
 *           
 * Params:   param - dummy argument not used..
 *
 * Return:   No value
 *
 **************/
void* ReadDeviceThread( void* param)
{
    fd_set queueset, deviceset;
    int32_t rv, rv1, rd, retry, written, res, nfds;
    char buf[MAX_BUF_SIZE];


    /*Create a pipe*/
    res = pipe((int*)offlinesigfd);
    if( res == -1)
    {
        PXY_LOG_MSG(LC_LOG_ERROR,"Error: Cannot open the OFFLINE SIGNAL pipe");
        exit(1);
    }

    /*No device opened yet*/
    bNoDevice = 1;
    
    while(1)
    {
        if( bNoDevice )
        {
            devicefd = OpenDevice(tty_port);
            if(devicefd < 0 )
            {
                bNoDevice = 1;
                delay(DEFAULT_TIMEOUT);
                continue;
            }
            else
            {
                bNoDevice = 0;
                PXY_LOG_MSG(LC_LOG_INFO,"Info: Device detected, opened successfully");
            }
        }
        FD_ZERO(&deviceset);
        FD_SET(devicefd, &deviceset );
        FD_SET(offlinesigfd[0],&deviceset);
        if( devicefd < offlinesigfd[0] )
            nfds = offlinesigfd[0] +1;
        else
            nfds = devicefd +1;

        rv1 = select(nfds, &deviceset,NULL, NULL, NULL);
                
        if( rv1 <= 0 )
        {
            PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on device fd failed(RDT)");
            close(devicefd);
            devicefd = -1;
            bNoDevice = 1;
            continue;
        }
        else if( FD_ISSET(offlinesigfd[0], &deviceset ) )
        {
            PXY_LOG_MSG(LC_LOG_INFO,"Info: Read device thread got signal to go in OFFLINE/STANDBY mode");
            /*read the offline/standby signal data from pipe*/
            rd = read(offlinesigfd[0], buf, sizeof(buf) );
            if( rd < 0 )
            {
                PXY_LOG_MSG(LC_LOG_ERROR,"Error: read failed on offline/standby signal pipe");
            }
            close(devicefd);
            devicefd = -1;
            bNoDevice = 1;
            continue;
        }    
        else if( FD_ISSET(devicefd, &deviceset ) )
        {
            
            /*read the actual data from device*/
            rd = read(devicefd, buf, sizeof(buf) );
            if( rd < 0 )
            {
                PXY_LOG_MSG(LC_LOG_ERROR,"Error: read failed on device");
                if( errno == EBADF )
                {
                    /*BAD file descriptor, may be device gone*/
                    close(devicefd);
                    devicefd = -1;
                    bNoDevice = 1;
                }             
                continue;
            }
            else if( rd == 0 )
            {
                PXY_LOG_MSG(LC_LOG_INFO,"Info: Modem was reset(WST)");
                retry = 0;
                close(devicefd);
                devicefd = -1;
                do
                {
                    delay(3000);
                    /*retry to open device
                     * modem may take some time to 
                     * re- enumerate*/
                    devicefd = OpenDevice(tty_port);
                    retry++;
                    
                }while( devicefd < 0 && errno == ENOENT && retry < MAX_MODEM_RETRIES );
                
                if(devicefd < 0 )
                {
                    PXY_LOG_MSG(LC_LOG_ERROR,"Error: Failed to open device after reset");
                    bNoDevice = 1;
                    continue;
                }
                else
                    PXY_LOG_MSG(LC_LOG_INFO,"Info: Modem re-opened after reset\n");     
            } 
            
            /*If we are here that means read is successful
             * so we can send the data to pipe*/
            FD_ZERO(&queueset);
            FD_SET(pipefd[1], &queueset );
            rv = select(pipefd[1] + 1, NULL, &queueset, NULL, NULL);
            if( rv < 0 )
            {
               PXY_LOG_MSG(LC_LOG_ERROR,"Error: select on write ned of pipe fd failed(RDT)");
            }
            else if( FD_ISSET(pipefd[1], &queueset ) )
            {
                /*write the actual data to device*/                                
                written = write(pipefd[1], buf, rd);
                if( written < 0 )
                {
                    PXY_LOG_MSG(LC_LOG_ERROR,"Error: write failed on pipe");              
                }
            }  
        }
    }
    
    return (void*)0;
}

/*************
 *
 * Name: ExitProxy
 *
 * Purpose:  Signal handler for SIGINT signal.
 *           Closes all the file descriptors 
 *           before exiting. 
 *           
 * Params:   sig - signal number intercepted.
 *
 * Return:   No value
 *
 **************/
void ExitProxy(int sig)
{

    if(devicefd > 0)
     {
        close(devicefd);
        PXY_LOG_MSG(LC_LOG_INFO,"Info: Closing modem port");
     }
    pthread_cancel(read_device_thread);
    
    CloseAllClients();
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Closing all client sockets");

    if( pipefd[0] > 0 )
        close(pipefd[0]);
   
    if( pipefd[0] > 0 )    
        close(pipefd[1]);
        
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Closing pipe");

    if( offlinesigfd[0] > 0 )
        close(offlinesigfd[0]);
   
    if( offlinesigfd[1] > 0 )    
        close(offlinesigfd[1]);
        
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Closing OFFLINE Signal pipe");

     
    close(serv_sockfd);
    PXY_LOG_MSG(LC_LOG_INFO,"Info: Closing server socket");

    sig_int.sa_flags = 0;
    sig_int.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sig_int, 0);
    
}


