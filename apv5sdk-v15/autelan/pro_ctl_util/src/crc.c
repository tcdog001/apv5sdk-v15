#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

/*wangyu change the crc file_maxsize*/
//#define BUFFER_SIZE 6656*1024
#define BUFFER_SIZE 6848*1024
/*wangyu change end*/

int crc32_be(unsigned int crc, unsigned char const *p, unsigned int len)
{
	int i;
	while (len--) {
		crc ^= *p++ << 24;
		for (i = 0; i < 8; i++)
			crc =
			    (crc << 1) ^ ((crc & 0x80000000) ? 0x04c11db7 :
					  0);
	}
	return crc;
}

int main(int argc, char *argv[])
{
	int i;
	int ch;
	int fd;
	struct stat buf;
	unsigned char *buffer_read;
	unsigned char *opt;
	
/*wangyu modify :in 32bit system or 64bit system the unsigned int datasize is 4 bytes*/
	unsigned int crc;
/*wangyu modify end*/

	if (argc != 2)
	{
		printf("usage: crc file_name\n");
		return -1;
	}

	buffer_read = (unsigned char *)malloc(BUFFER_SIZE);
	if (buffer_read == NULL)
	{
		printf("malloc buffer error\n");
		return -1;
	}

	int result = stat(argv[1], &buf);
	printf("file lenth=%d\n", (int)buf.st_size);

	fd = open(argv[1], O_RDWR, 0666);
	if (fd < 0)
	{
		printf("open file error\n");
		free(buffer_read);
		return -1;
	}

	memset(buffer_read, 0, BUFFER_SIZE);
	read(fd, buffer_read, BUFFER_SIZE);
	lseek(fd, 0, SEEK_SET);
	opt = (unsigned char *)buffer_read;
	crc = crc32_be(0, opt, buf.st_size);
	printf("crc=%u\n", crc);//wangyu change uint type print to %u
	printf("char name=%s\n", argv[1]);
	free(buffer_read);
	close(fd);
	return 0;
}
