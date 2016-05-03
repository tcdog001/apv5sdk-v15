#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd, &options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB;
	options.c_iflag |= IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	tcsetattr(fd,TCSANOW,&options);
}


int send(int fd, char *cmdbuf)
{
	int nread,nwrite;
	char buff[128];
	char reply[128];
	char *pbuf = buff;
	(void)cmdbuf;
	memset(buff,0,sizeof(buff));
	pbuf += sprintf(pbuf, "%s", cmdbuf);
	pbuf += sprintf(pbuf, "\r");


	nwrite = write(fd,buff,strlen(buff));


	memset(reply,0,sizeof(reply));
	sleep(1);
	nread = read(fd,reply,sizeof(reply));
	printf("Response(%d):%s\n",nread,reply);


	return 0;
}

int main(int arg, char **argv)
{
	int fd;
	char cmdbuf[128];


	fd = open( argv[1], O_RDWR|O_NOCTTY|O_NDELAY);
	if (-1 == fd){
       perror("Can't Open Serial Port");
}


serial_init(fd);
printf("=======Demo to test AT command=======\n");

while(1) {
	memset(cmdbuf,0,sizeof(cmdbuf));
	printf("Please input AT command cmd=");
	scanf("%s", cmdbuf);
	//printf("at cmd:%s\n", cmdbuf);
	if (strcmp(cmdbuf, "exit") == 0)
		break;
		send(fd, cmdbuf);
	}
	close(fd);
	return 0;
}

