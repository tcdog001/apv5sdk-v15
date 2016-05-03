#include <auteos.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <string.h>

static const char *TTYFILE = "/dev/ttyUSB0";

int main(int argc, char **argv)
{
    int fd;
    int status = 0;
    int str_len = 0;
    char *tty_file = NULL;
    char at_cmd[128];
    char read_buf[1024];
    struct termios Opt;
    struct timeval timeout;
    fd_set fd_read;

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    memset(at_cmd, 0x00, sizeof(at_cmd));
    memset(&Opt, 0x00, sizeof(struct termios));
    memset(read_buf, 'B', sizeof(read_buf));
	
    usleep(200);

    if( 2 == argc )
    {
        strcpy(at_cmd, argv[1]);
        strcat(at_cmd, "\r\n");
        tty_file = TTYFILE;
    }
    else if( 4 == argc )
    {
        if( strcmp(argv[1], "-f") == 0 )
        {
            strcpy(at_cmd, argv[3]);
            strcat(at_cmd, "\r\n");
            tty_file = argv[2];
        }
        else if( strcmp(argv[2], "-f") == 0 )
        {
            strcpy(at_cmd, argv[1]);
            strcat(at_cmd, "\r\n");
            tty_file = argv[3];
        }
        else
        {
            autelan_printf("ttycrl error:wrong argument\n");
            return -1;
        }
    }
    else
    {
        autelan_printf("ttycrl error:wrong argument\n");
        return -1;
    }

    //printf("tty file: %s\nat cmd: %s", tty_file, at_cmd);
    
    if( (fd=autelan_open(tty_file, O_RDWR)) == -1 )
    {
        perror("ttycrl error");
        return -1;
    }

    tcgetattr(fd, &Opt);
    tcflush(fd,TCIFLUSH);
    
    cfsetispeed(&Opt, B115200);
    cfsetospeed(&Opt, B115200);
    Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
    Opt.c_oflag &= ~OPOST;   

    if((status = tcsetattr(fd, TCSANOW, &Opt)))
    {
        perror("ttycrl error");
        autelan_close(fd);
        return status;
    }

    //printf("write to tty cmd %d\n", strlen(at_cmd));
    str_len = strlen(at_cmd);
    if( autelan_write(fd, at_cmd, str_len) != str_len)
    {
        perror("ttycrl error");
        autelan_close(fd);
        return -1;
    }

    FD_ZERO(&fd_read);
    FD_SET(fd, &fd_read);
    //printf("select \n");
   
    status = autelan_select(fd + 1, &fd_read, NULL, NULL, &timeout); 
    if(status <= 0)
    {
        autelan_printf("ttycrl error:read error\n");
        autelan_close(fd);
        return -1;
    }
    //printf("read\n");

    usleep(200);
    
    str_len = autelan_read(fd, read_buf, 1024);
    read_buf[str_len] = '\0';
    
    //printf("result: \n");
    autelan_printf("%s\n", read_buf);

    autelan_close(fd);
    return 0;
    
}

