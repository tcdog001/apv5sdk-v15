#include <auteos.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"

#define SERV_PORT 9997
#define AP_COUNT 2
#define VAL_LEN 8

#define AP_REG 1
#define AP_UNREG 2
#define AP_JOIN 3
#define AP_LEAVE 4

#define ACCESS 1
#define NOT_ACCESS 2
#define KICK 3

static AP_TAB ap_table[2];
static int ap_count;
static int sockfd;

static int threshold;
static int diff;

int ap_register( struct sockaddr_in *pcliaddr )
{
    int i = 0;
    autelan_printf("hxh:apcount %d\n",ap_count);
    if( ap_count < AP_COUNT )
    {
        for(; i<ap_count; i++)
        {
            if( pcliaddr->sin_addr.s_addr == ap_table[i].m_ip_addr )
            {
                return 0;
            }
        }
        ap_table[ap_count].m_access_flag = 1;
        ap_table[ap_count].m_ip_addr = pcliaddr->sin_addr.s_addr;
        ap_table[ap_count].m_user_count = 0;
        ap_table[ap_count++].m_port = pcliaddr->sin_port;
    }
    autelan_printf("hxh:ipaddr %x\n", ap_table[0].m_ip_addr);
    autelan_printf("hxh:ipaddr %x\n", ap_table[1].m_ip_addr);
    autelan_printf("hxh:apcount %d\n",ap_count);
}

int ap_unregister( struct sockaddr_in *pcliaddr )
{
    int i = 0;
    if( ap_count > 0 )
    {
        for(; i<ap_count; i++)
        {
            if( pcliaddr->sin_addr.s_addr == ap_table[i].m_ip_addr )
            {
                break;
            }
        }
        memset(&ap_table[i], 0, sizeof(AP_TAB));
        ap_count--;
    }
}

/*only for 2 aps*/
int user_join( struct sockaddr_in *pcliaddr )
{
    int i = 0;
    char sendbuf;
    if( ap_count <= 1 )
    {
        autelan_printf("user join");
        return 0;
    }
    for(; i<ap_count; i++)
    {
        if( pcliaddr->sin_addr.s_addr == ap_table[i].m_ip_addr )
        {
            break;
        }
    }
    autelan_printf("hxh:user join from : %x\n", ap_table[i].m_ip_addr);
    ap_table[i].m_user_count++;
    autelan_printf("hxh: table1 %d\n", ap_table[i].m_user_count);
    autelan_printf("hxh: table2 %d\n", ap_table[1-i].m_user_count);
    if( ap_table[i].m_user_count > ap_table[1-i].m_user_count )
    {
        autelan_printf("hxh: join: bigger then\n");
        if( ap_table[i].m_user_count >= threshold )
        {
            autelan_printf("hxh: join: bigger then threshold\n");
            if( ap_table[i].m_user_count - ap_table[1-i].m_user_count >= diff )
            {
                autelan_printf("hxh: join: bigger then diff\n");
                sendbuf = NOT_ACCESS;
                goto sendcmd1;
            }
            if( ap_table[1-i].m_user_count < threshold )
            {
                autelan_printf("hxh: join: smaller then threshold\n");
                sendbuf = NOT_ACCESS;
                goto sendcmd1;
            }
        }
    }
    else
    {
        autelan_printf("hxh: join: smaller then\n");
        if(ap_table[1-i].m_user_count - ap_table[i].m_user_count < diff)
        {
            if(ap_table[1-i].m_access_flag == 0)
            {
                sendbuf = ACCESS;
                goto sendcmd2;
            }
        }
    }
    return 0;
sendcmd1:
    if( ap_table[i].m_access_flag == 1 )
    {
        ap_table[i].m_access_flag = 0;
        autelan_printf("hxh:refuse ap %x join\n", pcliaddr->sin_addr.s_addr);
        pcliaddr->sin_port = ap_table[i].m_port;
        autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in) );
    }
    return 0;
sendcmd2:
    if(ap_table[1-i].m_access_flag == 0)
    {
        ap_table[1-i].m_access_flag = 1;
        autelan_printf("hxh:accept ap %x join\n", pcliaddr->sin_addr.s_addr);
        pcliaddr->sin_addr.s_addr = ap_table[1-i].m_ip_addr;
        pcliaddr->sin_port = ap_table[1-i].m_port;
        autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in) );
    }
}

/*only for 2 aps*/
int user_leave( struct sockaddr_in *pcliaddr )
{
    int i = 0;
    char sendbuf;
    if( ap_count <= 1 )
    {
        autelan_printf("user leave");
        return 0;
    }
    for(; i<ap_count; i++)
    {
        if( pcliaddr->sin_addr.s_addr == ap_table[i].m_ip_addr )
        {
            break;
        }
    }
    autelan_printf("hxh:user leave from : %x\n", ap_table[i].m_ip_addr);
    ap_table[i].m_user_count--;

    autelan_printf("hxh: table1 %d\n", ap_table[i].m_user_count);
    autelan_printf("hxh: table2 %d\n", ap_table[1-i].m_user_count);

    if( ap_table[i].m_user_count < ap_table[1-i].m_user_count )
    {
        if((ap_table[1-i].m_user_count - ap_table[i].m_user_count > diff) && 
        ap_table[1-i].m_user_count > threshold )
        {
            pcliaddr->sin_addr.s_addr = ap_table[1-i].m_ip_addr;
            pcliaddr->sin_port = ap_table[1-i].m_port;
            if( ap_table[1-i].m_access_flag == 1 )
            {
                sendbuf = NOT_ACCESS;
                autelan_printf("hxh:refuse ap %x join\n", pcliaddr->sin_addr.s_addr);
                autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in ));
                ap_table[1-i].m_access_flag = 0;
            }
            sendbuf = KICK;
            autelan_printf("hxh:kick ap %x join\n", pcliaddr->sin_addr.s_addr);
            autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in ));
            return 0;
        }
        else if( (ap_table[1-i].m_user_count - ap_table[i].m_user_count == diff) &&
                 ap_table[1-i].m_user_count >= threshold )
        {
            pcliaddr->sin_addr.s_addr = ap_table[1-i].m_ip_addr;
            if( ap_table[1-i].m_access_flag == 1)
            {
                sendbuf = NOT_ACCESS;
                pcliaddr->sin_port = ap_table[1-i].m_port;
                autelan_printf("hxh:refuse ap %x join\n", pcliaddr->sin_addr.s_addr);
                autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in ));
                ap_table[1-i].m_access_flag = 0;
            }
            return 0;
        }
        else
        {
            return 0;
        }
    }
    if( ap_table[i].m_access_flag == 0)
    {
        if( ap_table[i].m_user_count - ap_table[1-i].m_user_count < diff)
        {
            sendbuf = ACCESS;
            autelan_printf("hxh:accept ap %x join\n", pcliaddr->sin_addr.s_addr);
            pcliaddr->sin_port = ap_table[i].m_port;
            autelan_sendto( sockfd, &sendbuf, 1, 0, (struct sockaddr *)pcliaddr, sizeof(struct sockaddr_in ));
            ap_table[i].m_access_flag = 1;
        }
    }
    return 0;
}

int data_process( struct sockaddr_in *pcliaddr, char *rcvbuf )
{
    unsigned char flag = rcvbuf[0];
    switch(flag)
    {
        case AP_REG:
        {
            ap_register( pcliaddr );
            break;
        }
        case AP_UNREG:
        {
            ap_unregister( pcliaddr );
            break;
        }
        case AP_JOIN:
        {
            user_join( pcliaddr );
            break;
        }
        case AP_LEAVE:
        {
            user_leave( pcliaddr );
            break;
        }
        default:
        {
            perror("unrecognize type");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    fd_set fd_read;
    char rcvbuf[VAL_LEN];
    int rcvlen = 0;

    memset(rcvbuf, 0, sizeof(rcvbuf));

    if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        perror("create socket error");
        exit(1);
    }

    threshold = atoi(argv[1]);
    diff = atoi(argv[2]);

    autelan_printf("hxh:threshold : %d\n", threshold);
    autelan_printf("hxh:diff : %d\n", diff);
    
    memset( &servaddr, 0, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = autelan_inet_addr("0.0.0.0");
    servaddr.sin_port = autelan_htons(SERV_PORT);

    if( autelan_bind( sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("autelan_bind error");
        exit(1);
    }

    FD_ZERO(&fd_read);
    FD_SET(sockfd, &fd_read);
    while(1)
    {
        if(autelan_select(sockfd + 1, &fd_read, NULL, NULL, NULL) > 0)
        {
            autelan_printf("hxh:readable\n");
            memset( &cliaddr, 0, sizeof(cliaddr) );
            rcvlen = sizeof(struct sockaddr_in);
            autelan_recvfrom(sockfd, rcvbuf, VAL_LEN, 0, (struct sockaddr *)&cliaddr, &rcvlen);
            autelan_printf("hxh:get data from : %x\n", cliaddr.sin_addr.s_addr);
            autelan_printf("hxh:get data %d\n", rcvbuf[0]);
            data_process(&cliaddr, rcvbuf);
        }
    }

    return 0;
}
