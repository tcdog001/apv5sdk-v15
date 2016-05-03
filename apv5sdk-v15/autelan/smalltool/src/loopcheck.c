#include <stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<linux/if.h>
#include<linux/if_packet.h>
#include<linux/sockios.h>
#include<linux/if_ether.h>
#include<string.h>

#define LOOP_TYPE 0x5a5a
#define MAX_SIZE 1024

void help(void)
{
	printf("Usage: \n");
	printf("\t loopcheck -i DEVNAME -r RETRY -s PKT_SIZE\n");
	printf("\t Warning:  512 >= PKT_SIZE >= 64\n");
}

int genrand(char *buf, int count)
{
	int i;
	srand(time(0));

	for(i = 0; i < count; i++)
		buf[i] = rand()%128;
}

int main(int argc, char *argv[])
{
	struct sockaddr_ll src;
	struct sockaddr_ll dst;
	struct ifreq  ifr;

	int sock;
	int ret;
	int w_ret;
	int r_ret;
	int size;

	char rbuf[MAX_SIZE];
	char wbuf[MAX_SIZE];
	char *data;

	char iface[16] = {0};
	int  retry = 0;
	int  pkt_size = 0; 
	int debug = 0;

	int ch;

	struct ethhdr *rhdr = (struct ethhdr *)rbuf;
	struct ethhdr *whdr = (struct ethhdr *)wbuf;

	while((ch = getopt(argc, argv, "i:r:s:d:")) != -1)
	{
		switch(ch)
		{
		case 'i':
			strcpy(iface, optarg);
			break;
		case 'r':
			retry = atoi(optarg);
			break;
		case 's':
			pkt_size = atoi(optarg);
			break;
		case 'd':
			debug = atoi(optarg);
			break;
		default:
			help();
			printf("param err .\n");
			return 0;
		}
	}


	if( pkt_size > 512 || pkt_size < 64 || retry <= 0)
	{
		help();
		printf("\nparam err ..\n");
		return 0;
	}


	sock = socket(PF_PACKET, SOCK_RAW, LOOP_TYPE);
	if(sock < 0){
		printf("socket create failed\n");
		return 0;
	}

	memset(&ifr, 0 , sizeof(ifr));

	snprintf(ifr.ifr_name,sizeof(ifr.ifr_name), iface);

	ret = ioctl(sock, SIOCGIFINDEX, &ifr);
	if(ret < 0){
		printf("get ifreq failed\n");
		close(sock);
		return 0;
	}
	
	memset(&src, 0, sizeof(src));

	src.sll_family = AF_PACKET;
	src.sll_ifindex = ifr.ifr_ifindex;
	src.sll_protocol = LOOP_TYPE;

	memset(&dst, 0, sizeof(dst));

	dst.sll_family = AF_PACKET;
	dst.sll_ifindex = ifr.ifr_ifindex;
	dst.sll_protocol = LOOP_TYPE;

	ret = bind(sock , (struct sockaddr *)&src, sizeof(src));
	if(ret < 0){
		printf("bind failed\n");
		close(sock);
		return 0;
	}



    whdr->h_dest[0] = 0xff;
    whdr->h_dest[1] = 0xff;
    whdr->h_dest[2] = 0xff;
    whdr->h_dest[3] = 0xff;
    whdr->h_dest[4] = 0xff;
    whdr->h_dest[5] = 0xff;

    whdr->h_source[0] = 0x1f;
    whdr->h_source[1] = 0x2f;
    whdr->h_source[2] = 0x3f;
    whdr->h_source[3] = 0x4f;
    whdr->h_source[4] = 0x5f;
    whdr->h_source[5] = 0x6f;
 
    whdr->h_proto = htons(LOOP_TYPE);

	data = wbuf + sizeof(*whdr);
	size = MAX_SIZE - sizeof(*whdr);

    while (retry--) {

		memset(data, 0, size);
		genrand(data, pkt_size);
		if(debug)
		{
			printf("write \t%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
					data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
		}

        w_ret = write(sock, wbuf, pkt_size + sizeof(*whdr));

		//printf("write: %d\n",w_ret);

		memset(rbuf + sizeof(*rhdr), 0, MAX_SIZE - sizeof(*rhdr));
		r_ret = read(sock, rbuf, MAX_SIZE);
		if (r_ret > 0) {
			//printf("read: %d\n",r_ret);

			if(debug){
				char *buffer = rbuf + sizeof(*rhdr);
				printf("read \t%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
						buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
			}

			if(w_ret == r_ret){
				if(memcmp(data, rbuf + sizeof(*rhdr), pkt_size) != 0)
					printf("data err\n");
			} else {
				printf("data lost !\n");
			}
			if(debug)
			{
				printf("Source MAC:\t%02x:%02x:%02x:%02x:%02x:%02x\n", 
						rhdr->h_source[0],
						rhdr->h_source[1],
						rhdr->h_source[2],
						rhdr->h_source[3],
						rhdr->h_source[4],
						rhdr->h_source[5]);

				printf("Dest MAC:\t%02x:%02x:%02x:%02x:%02x:%02x\n", 
						rhdr->h_dest[0],
						rhdr->h_dest[1],
						rhdr->h_dest[2],
						rhdr->h_dest[3],
						rhdr->h_dest[4],
						rhdr->h_dest[5]);

				printf("Proto :\t%04x\n",rhdr->h_proto);

				printf("*************************************************\n");
			}
		}
	}
	printf("\n\n\t *** ETH TEST SUCCESS ***\n\n");
	return 0;

}

