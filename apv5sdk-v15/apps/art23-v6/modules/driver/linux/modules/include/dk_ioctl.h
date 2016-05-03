#ifndef __DK_IOCTL_H_
#define __DK_IOCTL_H_

/*  #define DK_IOCTL_GET_VERSION 1
#define DK_IOCTL_GET_CLIENT_INFO 2
#define DK_IOCTL_CFG_READ 3
#define DK_IOCTL_CFG_WRITE 4
#define DK_IOCTL_CREATE_EVENT 5
#define DK_IOCTL_GET_NEXT_EVENT 6
#define DK_IOCTL_SYS_REG_READ_32 7
#define DK_IOCTL_SYS_REG_WRITE_32 8
#define DK_IOCTL_FLASH_READ 9
#define DK_IOCTL_FLASH_WRITE 10
#define DK_IOCTL_MAC_WRITE 11
#define DK_IOCTL_GET_CHIP_ID 12
#define DK_IOCTL_RTC_REG_READ 13
#define DK_IOCTL_RTC_REG_WRITE 14
#define DK_IOCTL_FULL_ADDR_READ 15
#define DK_IOCTL_FULL_ADDR_WRITE 16
*/

#define DK_IOCTL_GET_VERSION 601
#define DK_IOCTL_GET_CLIENT_INFO 602
#define DK_IOCTL_CFG_READ 603
#define DK_IOCTL_CFG_WRITE 604
#define DK_IOCTL_CREATE_EVENT 605
#define DK_IOCTL_GET_NEXT_EVENT 606
#define DK_IOCTL_SYS_REG_READ_32 607
#define DK_IOCTL_SYS_REG_WRITE_32 608
#define DK_IOCTL_FLASH_READ 609
#define DK_IOCTL_FLASH_WRITE 610
#define DK_IOCTL_MAC_WRITE 611
#define DK_IOCTL_GET_CHIP_ID 612
#define DK_IOCTL_RTC_REG_READ 613
#define DK_IOCTL_RTC_REG_WRITE 614
#define DK_IOCTL_FULL_ADDR_READ 615
#define DK_IOCTL_FULL_ADDR_WRITE 616

#undef MAX_BARS
#define MAX_BARS    6

struct cfg_op {
	int offset;
	int size;
	int value;
};
struct client_info {
    unsigned long reg_phy_addr;
    int reg_range;
    unsigned long mem_phy_addr;
    int mem_size;
    int irq;
    unsigned long areg_phy_addr[MAX_BARS];
    int areg_range[MAX_BARS];
    int numBars;
    int device_class;
    unsigned int dma_mem_addr;
};

struct event_op {
	unsigned int valid;
	unsigned int param[16];
};

struct flash_op{
	int fcl;
	int offset;
	int len;
	int retlen;
	unsigned char value;
};

struct flash_op_wr{
	int fcl;
	int offset;
	int len;
	int retlen;
	unsigned char *pvalue;
};

#endif
