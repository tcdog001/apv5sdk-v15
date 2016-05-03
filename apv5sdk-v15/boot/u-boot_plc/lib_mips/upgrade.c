#include <common.h>
#include <command.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <devices.h>
#include <environment.h>
#include "upgrade.h"
//#include "../board/ar7240/common/ar7240_flash.h" 
//#include "ar7240_soc.h"
#include <asm/addrspace.h>

#define BIN_SIZE 65536
#define FLAG_OFFSET 40176
#define OS_TYPE_OFFSET 40196
/*kernel1 start*/
unsigned char *kernel_crc_addr = 0x9ffe9cd0;
unsigned char *rootfs_crc_addr = 0x9ffe9cd4;
unsigned char *kernel_lenth_addr = 0x9ffe9ce0;
unsigned char *rootfs_lenth_addr = 0x9ffe9ce4;
/*kernel1 end*/

/*kernel2 start*/
unsigned char *kernel2_crc_addr = 0x9ffe9cd8;
unsigned char *rootfs2_crc_addr = 0x9ffe9cdc;
unsigned char *kernel2_lenth_addr = 0x9ffe9ce8;
unsigned char *rootfs2_lenth_addr = 0x9ffe9cec;
/*kernel2 end*/

/*crc start address*/
unsigned char *kernel_crc_start = 0x9f050000;
unsigned char *rootfs_crc_start = 0x9f130000;
unsigned char *boot_flag_addr = 0x9ffe9cf0;
unsigned char *boot_mark_addr = 0x9ffe9D04;

unsigned char *kernel2_crc_start = 0x9f7e0000;
unsigned char *rootfs2_crc_start = 0x9f8c0000;

unsigned char *magic_begin = 0x9ffe0000;
unsigned char *magic_end = 0x9ffeffff;
unsigned char config_magic_data[BIN_SIZE];

u32  crc32_be(u32 crc, unsigned char const *p, size_t len)
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
void make_magic_data(void)
{
	int i;
	memset(config_magic_data,0,BIN_SIZE);
	unsigned char *tmp_addr = magic_begin;
	for(i=0;i<BIN_SIZE;i++)
	{
		config_magic_data[i] = *tmp_addr ;
		tmp_addr++;
	}
	printf("make_magic_data ok\n");
	return 0;
}
#if 0
static void
ar7240_spi_write_enable()  
{
    ar7240_reg_wr_nf(AR7240_SPI_FS, 1);                  
    ar7240_reg_wr_nf(AR7240_SPI_WRITE, AR7240_SPI_CS_DIS);     
    ar7240_spi_bit_banger(AR7240_SPI_CMD_WREN);             
    ar7240_spi_go();
}
static void
ar7240_spi_poll()   
{
    int rd;                                                 

    do {
        ar7240_reg_wr_nf(AR7240_SPI_WRITE, AR7240_SPI_CS_DIS);     
        ar7240_spi_bit_banger(AR7240_SPI_CMD_RD_STATUS);        
        ar7240_spi_delay_8();
        rd = (ar7240_reg_rd(AR7240_SPI_RD_STATUS) & 1);               
    }while(rd);
}
static void
ar7240_spi_sector_erase(uint32_t addr)
{
    ar7240_spi_write_enable();
    ar7240_spi_bit_banger(AR7240_SPI_CMD_SECTOR_ERASE);
    ar7240_spi_send_addr(addr);
    ar7240_spi_go();
    display(0x7d);
    ar7240_spi_poll();
}

static void
ar7240_spi_write_page(uint32_t addr, uint8_t *data, int len)
{
    int i;
    uint8_t ch;

    display(0x77);
    ar7240_spi_write_enable();
    ar7240_spi_bit_banger(AR7240_SPI_CMD_PAGE_PROG);
    ar7240_spi_send_addr(addr);

    for(i = 0; i < len; i++) {
        ch = *(data + i);
        ar7240_spi_bit_banger(ch);
    }

    ar7240_spi_go();
    display(0x66);
    ar7240_spi_poll();
    display(0x6d);
}
#endif
int crc_kernel2(void)
{	
	printf("crc kernel2\n");
	ulong len_k,len_r;
	ulong crc_kernel,crc_rootfs;
	ulong crc1,crc2;
	
	crc_kernel = *(ulong *)kernel2_crc_addr;
	printf("kernel_crc_addr=0x%x\n",crc_kernel);
	crc_rootfs = *(ulong *)rootfs2_crc_addr;
	printf("rootfs_crc_addr=0x%x\n",crc_rootfs);
	len_k = *(ulong *)kernel2_lenth_addr;
	printf("kernel lenth=%u\n",len_k);
	len_r = *(ulong *)rootfs2_lenth_addr;
	printf("rootfs lenth=%u\n",len_r);
	
#if 1
	if(len_k > 1114112 || len_r > 7012352){
		printf("len is too long , now jump the crc,default bootm kernel2\n");
		setenv ("bootcmd", "bootm 0x9f7e0000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:05 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		return 1;
	}else{
	crc1 = crc32_be(0,kernel2_crc_start,len_k);
	crc2 = crc32_be(0,rootfs2_crc_start,len_r); 
	printf("crc1 = 0x%x\n",crc1);
	printf("crc2 = 0x%x\n",crc2);
	}
	if(crc_kernel == crc1 && crc_rootfs == crc2){
		setenv ("bootcmd", "bootm 0x9f7e0000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:05 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		printf("bootm kernel2\n");
	}else{
		make_magic_data();
		puts ("Erasing Flash...");

		if (flash_sect_erase (magic_begin, magic_end))
			return 1;
		*(ulong *)(config_magic_data + FLAG_OFFSET) = 1;
		int rc = flash_write((char *)config_magic_data, magic_begin, 0x10000);
		if (rc != 0) {
			flash_perror (rc);
			return 1;
		} else {
			puts ("done\n");
		}

		setenv ("bootcmd", "bootm 0x9f050000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:03 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		printf("bootm kernel1\n");
	}
	return 0;
#endif	
}
int crc_kernel1(void)
{
	ulong len_k,len_r;
	ulong crc_kernel,crc_rootfs;
	ulong crc1,crc2;
	
	crc_kernel = *(ulong *)kernel_crc_addr;
	printf("kernel_crc_addr=0x%x\n",crc_kernel);
	crc_rootfs = *(ulong *)rootfs_crc_addr;
	printf("rootfs_crc_addr=0x%x\n",crc_rootfs);
	len_k = *(ulong *)kernel_lenth_addr;
	printf("kernel lenth=%u\n",len_k);
	len_r = *(ulong *)rootfs_lenth_addr;
	printf("rootfs lenth=%u\n",len_r);
	
#if 1
	if(len_k > 1114112 || len_r > 7012352){
		printf("len is too long , now jump the crc,defaule bootm kernel1\n");
		setenv ("bootcmd", "bootm 0x9f050000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:03 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		return 1;
	}else{
		crc1 = crc32_be(0,kernel_crc_start,len_k);
		crc2 = crc32_be(0,rootfs_crc_start,len_r); 
		printf("crc1 = 0x%x\n",crc1);
		printf("crc2 = 0x%x\n",crc2);
	}
	if(crc_kernel == crc1 && crc_rootfs == crc2){
		setenv ("bootcmd", "bootm 0x9f050000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:03 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		printf("bootm kernel1\n");
	}else{
		make_magic_data();
		puts ("Erasing Flash...");

		if (flash_sect_erase (magic_begin, magic_end))
			return 1;
		*(ulong *)(config_magic_data + FLAG_OFFSET) = 2;
		int rc = flash_write((char *)config_magic_data, magic_begin, 0x10000);
		if (rc != 0) {
			flash_perror (rc);
			return 1;
		} else {
			puts ("done\n");
		}

		setenv ("bootcmd", "bootm 0x9f7e0000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:05 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
		printf("bootm kernel2\n");
	}
	return 0;
#endif	
}

void crc_flash(void)
{
	ulong  crc_value,crc_value2,lenth,lenth2;
	ulong  crc_kernel,crc_rootfs;
	ulong  flag = *(ulong *)boot_flag_addr;
	ulong mark = *(ulong *)boot_mark_addr;
	char *s;
	char *c;
#if 0
	make_magic_data();
	puts ("Erasing Flash...");
	if (flash_sect_erase (0x9ffe0000, 0x9ffeffff))
		return 1;
	*(ulong *)(config_magic_data + FLAG_OFFSET) = 2;
	int rc = flash_write((char *)config_magic_data, 0x9ffe0000, 0x10000);
	if (rc != 0) {
		flash_perror (rc);
	} else {
		puts ("done\n");
	}
#endif	
	printf("mark=%u\n",mark);
	if (mark == 2){
		printf("flag=%u\n",flag);
		if (flag == 1){
			if(crc_kernel1())
				printf("kernel1 crc error\n");
			s = getenv ("bootcmd");
			printf("bootcmd = %s\n",s);
			c = getenv ("bootargs");
			printf("bootargs = %s\n",c);
		}else if(flag ==2){
			if(crc_kernel2())
				printf("kernel2 crc error\n");
			s = getenv ("bootcmd");
			printf("bootcmd = %s\n",s);
			c = getenv ("bootargs");
			printf("bootargs = %s\n",c);
		}else{
			printf("frist time startup,now write 1 to flag\n");
			make_magic_data();
			puts ("Erasing Flash...");

			if (flash_sect_erase (magic_begin, magic_end))
				return 1;
			*(ulong *)(config_magic_data + FLAG_OFFSET) = 1;
			int rc = flash_write((char *)config_magic_data, magic_begin, 0x10000);
			if (rc != 0) {
				flash_perror (rc);
				return 1;
			} else {
				puts ("done\n");
				setenv ("bootcmd", "bootm 0x9f050000");
				setenv ("bootargs", "console=ttyS0,115200 root=31:03 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
			}
		}
	}
	else if(mark == 1){
		setenv ("bootcmd", "bootm 0x9f050000");
		setenv ("bootargs", "console=ttyS0,115200 root=31:10 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
	}else{
		printf("frist time startup,now write 1 to mark\n");
		make_magic_data();
		puts ("Erasing Flash...");
		
		if (flash_sect_erase (magic_begin, magic_end))
			return 1;
		*(ulong *)(config_magic_data + OS_TYPE_OFFSET) = 1;
		int rc = flash_write((char *)config_magic_data, magic_begin, 0x10000);
		if (rc != 0) {
			flash_perror (rc);
			return 1;
			} else {
			puts ("done\n");
			setenv ("bootcmd", "bootm 0x9f050000");
			setenv ("bootargs", "console=ttyS0,115200 root=31:10 rootfstype=jffs2 init=/sbin/init mtdparts=ath-nor0:256k(u-boot),64k(u-boot-env),896k(kernel1),6848k(rootfs1),896k(kernel2),6848k(rootfs2),448k(config),64k(productinfo),64k(ART)");
			}
		}
	return 0;
}


