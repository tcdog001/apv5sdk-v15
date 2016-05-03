#ifndef _NAND_API_H
#define _NAND_API_H

struct nand_api {
	void (*_nand_init)(void);
	void (*_nand_load_fw)(void);
	int (*_nand_read)(unsigned, unsigned, unsigned);
	int (*_nand_param_page)(uint8_t *, unsigned);
	void (*_nand_read_id)(void);
};

void
nand_module_install(struct nand_api *api);

#define AR7240_NAND_FLASH_BASE	0x1b000000u
#define AR7240_NF_CMD		(AR7240_NAND_FLASH_BASE + 0x200u)
#define AR7240_NF_CTRL		(AR7240_NAND_FLASH_BASE + 0x204u)
#define AR7240_NF_RST_REG	(AR7240_NAND_FLASH_BASE + 0x208u)
#define AR7240_NF_ADDR0_0	(AR7240_NAND_FLASH_BASE + 0x21cu)
#define AR7240_NF_ADDR0_1	(AR7240_NAND_FLASH_BASE + 0x224u)
#define AR7240_NF_DMA_ADDR	(AR7240_NAND_FLASH_BASE + 0x264u)
#define AR7240_NF_DMA_COUNT	(AR7240_NAND_FLASH_BASE + 0x268u)
#define AR7240_NF_DMA_CTRL	(AR7240_NAND_FLASH_BASE + 0x26cu)
#define AR7240_NF_MEM_CTRL	(AR7240_NAND_FLASH_BASE + 0x280u)
#define AR7240_NF_PG_SIZE	(AR7240_NAND_FLASH_BASE + 0x284u)
#define AR7240_NF_RD_STATUS	(AR7240_NAND_FLASH_BASE + 0x288u)
#define AR7240_NF_TIMINGS_ASYN	(AR7240_NAND_FLASH_BASE + 0x290u)

#define BLK_SIZE		0x2	//Number of Pages per block; 0=32, 1=64, 2=128, 3=256
#define PAGE_SIZE		0x4	//No of bytes per page; 0=256, 1=512, 2=1024, 3=2048, 4=4096, 5=8182, 6= 16384, 7=0
#define CUSTOM_SIZE_EN		0x1	//1 = Enable, 0 = Disable
#define ADDR_CYCLES_NUM		0x5	//No of Address Cycles
#define TIMING_ASYN		0x11
#define READ_STATUS_MASK	0xc7
#define READ_STATUS_OK		0xc0
#define READ_PARAM_STATUS_OK	0x60

/*
 * Per Spec, This should be in byte no. 101. But because of
 * the byte swapping by the controller during DMA, it gets
 * moved to byte 102.
 */
#define ONFI_NUM_ADDR_CYCLES		102
#endif
