#include <asm/addrspace.h>
#include <wasp_api.h>
#include <nand_api.h>
#include <apb_map.h>
#include <ar7240_soc.h>
#include <include/dv_dbg.h>

typedef union {
	uint8_t		byte_id[8];

	/*
	 * The following is applicable for numonyx and hynix. have to
	 * check for other manufacturers.
	 */
	struct {
		uint8_t	sa1	: 1,	// Serial access time (bit 1)
			org	: 1,	// Organisation
			bs	: 2,	// Block size
			sa0	: 1,	// Serial access time (bit 0)
			ss	: 1,	// Spare size per 512 bytes
			ps	: 2,	// Page Size

			wc	: 1,	// Write Cache
			ilp	: 1,	// Interleaved Programming
			nsp	: 2,	// No. of simult prog pages
			ct	: 2,	// Cell type
			dp	: 2,	// Die/Package

			did,		// Device id
			vid,		// Vendor id

			res1	: 2,	// Reserved
			pls	: 2,	// Plane size
			pn	: 2,	// Plane number
			res2	: 2;	// Reserved
	} id;
} ath_nand_id_t;

typedef struct {
	uint8_t		vid,
			did,
			b3,
			addrcyc,
			pgsz,
			blk,
			spare;
} ath_nand_vend_data_t;

ath_nand_vend_data_t ath_nand_arr[] = {
	{ 0x20, 0xda, 0x10,	5,	3,	1,	1 },	// NU2g3B2D
	{ 0x20, 0xf1, 0x00,	4,	3,	1,	1 },	// NU1g3B2C
	{ 0x20, 0xdc, 0x10,	5,	3,	1,	1 },	// NU4g3B2D
	{ 0x20, 0xd3, 0x10,	5,	4,	1,	1 },	// NU8g3F2A
	{ 0x20, 0xd3, 0x14,	5,	3,	2,	1 },	// NU8g3C2B
	{ 0xad, 0xf1, 0x00,	4,	3,	1,	1 },	// HY1g2b
	{ 0xad, 0xda, 0x10,	5,	3,	1,	1 }	// HY2g2b
};

#define NUM_ATH_NAND	(sizeof(ath_nand_arr) / sizeof(ath_nand_arr[0]))

#if defined(ROM_BUILD) || defined(EMUROM_BUILD)
static ath_nand_id_t __ath_nand_id;
static ath_nand_id_t *nand_id = &__ath_nand_id;
#else
static ath_nand_id_t *nand_id = (ath_nand_id_t *)CFG_SRAM_BASE;
#endif

int nand_param_page(uint8_t *, unsigned);

void nand_read_id(void)
{
	unsigned int	rddata;

	ar7240_reg_wr(AR7240_NF_DMA_ADDR,
			(unsigned int)PHYSADDR(nand_id));	// DMA Start Addr
	ar7240_reg_wr(AR7240_NF_ADDR0_0, 0x0);			// ADDR0_0 Reg Settings
	ar7240_reg_wr(AR7240_NF_ADDR0_1, 0x0);			// ADDR0_1 Reg Settings
	ar7240_reg_wr(AR7240_NF_DMA_COUNT, sizeof(*nand_id));	// DMA count
	ar7240_reg_wr(AR7240_NF_PG_SIZE, 0x8);			// Custom Page Size
	ar7240_reg_wr(AR7240_NF_DMA_CTRL, 0xcc);		// DMA Control Reg
	ar7240_reg_wr(AR7240_NF_CMD, 0x9061);			// READ ID

	rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
	while (rddata != 0xff) {
		rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
	}
}

void nand_flash_init(void)
{
	unsigned int	rddata;
	uint8_t		buf[128];

	// Pull Nand Flash and switch analog out of reset
#if 0
	rddata = ar7240_reg_rd(AR7240_RESET);
	rddata &= (~RST_RESET_NANDF_RESET_MASK);
	ar7240_reg_wr(AR7240_RESET, rddata);

	ar7240_reg_rmw_set(AR7240_RESET, RST_RESET_ETH_SWITCH_ARESET_SET(1));
	udelay(100);     // 100 usecs
#endif
	// Pull switch analog, GE0 & MDIOs out of reset
	ar7240_reg_rmw_clear(ATHR_RESET, RST_RESET_ETH_SWITCH_ARESET_MASK);
	udelay(250);

	ar7240_reg_rmw_clear(AR7240_RESET, RST_RESET_NANDF_RESET_MASK);
	udelay(100);

	rddata = ar7240_reg_rd(AR7240_NF_CTRL);

	// Reset Command
	ar7240_reg_wr(AR7240_NF_CMD, 0xff00);

	// Try to see the flash id
	A_NAND_READ_ID();

	if (rddata == 0) {
		int		i;

		// TIMINGS_ASYN Reg Settings
		ar7240_reg_wr(AR7240_NF_TIMINGS_ASYN, TIMING_ASYN);
#define ATH_TEST_ONFI_DBG_ONLY	0
#if ATH_TEST_ONFI_DBG_ONLY
		i = NUM_ATH_NAND;
#else
		for (i = 0; i < NUM_ATH_NAND; i++) {
#if 0
			A_PRINTF("---->0x%2x 0x%2x\n\t0x%2x 0x%2x\n\t0x%2x 0x%2x\n",
					nand_id->id.vid, ath_nand_arr[i].vid,
					nand_id->id.did, ath_nand_arr[i].did,
					nand_id->byte_id[1], ath_nand_arr[i].b3);
#endif
			if ((nand_id->id.vid == ath_nand_arr[i].vid) &&
			    (nand_id->id.did == ath_nand_arr[i].did) &&
			    (nand_id->byte_id[1] == ath_nand_arr[i].b3)) {
#if 1
				A_PRINTF("Table[%d]: Control setting = 0x%x\n", i,
						(ath_nand_arr[i].addrcyc) |
						(ath_nand_arr[i].blk << 6) |
						(ath_nand_arr[i].pgsz << 8) |
						(CUSTOM_SIZE_EN << 11));
#endif
				// Control Reg Setting
				ar7240_reg_wr(AR7240_NF_CTRL,
						(ath_nand_arr[i].addrcyc) |
						(ath_nand_arr[i].blk << 6) |
						(ath_nand_arr[i].pgsz << 8) |
						(CUSTOM_SIZE_EN << 11));
				break;
			}
		}
#endif	/* ATH_TEST_ONFI_DBG_ONLY */
		if (i == NUM_ATH_NAND) {
			if (A_NAND_PARAM_PAGE(buf, sizeof(buf)) == 0) {
				unsigned bs;

				rddata = buf[ONFI_NUM_ADDR_CYCLES];
				rddata = ((rddata >> 4) & 0xf) + (rddata & 0xf);

				bs = (64 << 10) << nand_id->id.bs;
				bs = bs / ((1 << 10) << nand_id->id.ps);
				switch (bs / 32) {
					case 1: bs = 0; break;
					case 2: bs = 1; break;
					case 4: bs = 2; break;
					case 8: bs = 3; break;
					default:bs = 1;
				}

#if 1
				A_PRINTF("ONFI: Control setting = 0x%x\n",
						(rddata) |
						(bs << 6) |
						((nand_id->id.ps + 2) << 8) |
						(CUSTOM_SIZE_EN << 11));
#endif
				// Control Reg Setting
				ar7240_reg_wr(AR7240_NF_CTRL,
						(rddata) |
						(bs << 6) |
						((nand_id->id.ps + 2) << 8) |
						(CUSTOM_SIZE_EN << 11));
			} else {
				DV_DBG_RECORD_LOCATION(NAND_C);
				A_PRINTF("Unknown Nand device: 0x%x %x %x %x %x\n",
						nand_id->byte_id[0], nand_id->byte_id[1],
						nand_id->byte_id[2], nand_id->byte_id[3],
						nand_id->byte_id[4]);
				while (1);
			}
		} else {
			DV_DBG_RECORD_LOCATION(NAND_C);
		}

	}
	// NAND Mem Control Reg
	ar7240_reg_wr(AR7240_NF_MEM_CTRL, 0xff00);
}


int
nand_param_page(uint8_t *buf, unsigned count)
{
	unsigned int	rddata;

	//A_PRINTF("%s: reading %p %p\n", __func__, nand_addr, buf);

	// ADDR0_0 Reg Settings
	ar7240_reg_wr(AR7240_NF_ADDR0_0, 0x0);

	// ADDR0_1 Reg Settings
	ar7240_reg_wr(AR7240_NF_ADDR0_1, 0x0);

	// DMA Start Addr
	ar7240_reg_wr(AR7240_NF_DMA_ADDR, PHYSADDR(buf));

	// DMA count
	ar7240_reg_wr(AR7240_NF_DMA_COUNT, count);

	// Custom Page Size
	ar7240_reg_wr(AR7240_NF_PG_SIZE, count);

	// DMA Control Reg
	ar7240_reg_wr(AR7240_NF_DMA_CTRL, 0xcc);

	// READ PARAMETER PAGE
	ar7240_reg_wr(AR7240_NF_CMD, 0xec62);

	rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
	while (rddata != 0xff) {
		rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
	}

	// READ STATUS
	ar7240_reg_wr(AR7240_NF_CMD, 0x07024);
	rddata = ar7240_reg_rd(AR7240_NF_RD_STATUS);
	if (rddata != READ_PARAM_STATUS_OK) {
		A_PRINTF("\nParam Page Failure: 0x%x", rddata);
		DV_DBG_RECORD_LOCATION(NAND_C);
		return 1;
	}

	if (buf[3] == 'O' && buf[2] == 'N' && buf[1] == 'F' && buf[0] == 'I') {
		DV_DBG_RECORD_LOCATION(NAND_C);
		return 0;
	}

	DV_DBG_RECORD_LOCATION(NAND_C);
	return 1;

}

int
nand_read(unsigned nand_addr, unsigned count, unsigned buf)
{
	unsigned int rddata, repeat = 0, tries = 0;

	//A_PRINTF("%s: reading %p %p\n", __func__, nand_addr, buf);
#define MAX_TRY		1000
#define MAX_REPEAT	3

	while (repeat < MAX_REPEAT) {
		repeat ++;

		// ADDR0_0 Reg Settings
		ar7240_reg_wr(AR7240_NF_ADDR0_0, nand_addr);

		// ADDR0_1 Reg Settings
		ar7240_reg_wr(AR7240_NF_ADDR0_1, 0x0);

		// DMA Start Addr
		ar7240_reg_wr(AR7240_NF_DMA_ADDR, PHYSADDR(buf));

		// DMA count
		ar7240_reg_wr(AR7240_NF_DMA_COUNT, count);

		// Custom Page Size
		ar7240_reg_wr(AR7240_NF_PG_SIZE, count);

		// DMA Control Reg
		ar7240_reg_wr(AR7240_NF_DMA_CTRL, 0xcc);

		// READ PAGE
		ar7240_reg_wr(AR7240_NF_CMD, 0x30006a);
		udelay(100);
		rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
			while (rddata != 0xff && tries++ < MAX_TRY) {
				udelay(50);
			rddata = ar7240_reg_rd(AR7240_NF_RST_REG);
		}

		if (tries >= MAX_TRY) {
			ar7240_reg_rmw_set(ATHR_RESET, RST_RESET_ETH_SWITCH_ARESET_MASK | RST_RESET_NANDF_RESET_MASK);
			udelay(250);

			A_NAND_INIT();

			tries = 0;
			A_PRINTF("\nretry", rddata);
			continue;
		}

		// READ STATUS
		ar7240_reg_wr(AR7240_NF_CMD, 0x07024);
		rddata = ar7240_reg_rd(AR7240_NF_RD_STATUS);
		if ((rddata & READ_STATUS_MASK) != READ_STATUS_OK) {
			A_PRINTF("\nRead Failure: 0x%x", rddata);
		} else {
			break;
		}
	}
	return rddata;
}

void
nand_dump_blk(unsigned fa, unsigned char *b, int count)
{
	int		i, j;

	for (i = 0; i < count; ) {
		A_PRINTF("%p:", i);
		for (j = 0;j < 16 && i < count; j++, i++) {
			A_PRINTF(" %02x", b[i]);
		}
		A_UART_PUTS("\n");
	}
}

/* f/w addr in flash */
#if 0
const uint32_t img_fa[] = { 0x80000000u, 0x80050000u };
#define NUM_FW	(sizeof(img_fa) / sizeof(img_fa[0]))
#else
#define NUM_FW	2
#endif

typedef struct {
	uint32_t	ep,	/* entry point */
			la,	/* load addr */
			sz,	/* firmware size */
			cs;	/* firmware crc checksum */
} nf_fw_hdr_t;

void
nand_load_fw(void)
{
	unsigned	j, i,
			nbs = 65536,	// for nand addr formation
			dbs,
			fa = 0x00000000u;
	nf_fw_hdr_t	hdr, *tmp = (nf_fw_hdr_t *)0xbd004000;

	extern	void	call_fw(int, const unsigned int);
	extern __u32	checksum(__u32, __u32);

	dbs = (1 << 10) << nand_id->id.ps;		// page size

	for (j = 0; j < NUM_FW; j++) {

		/*
		 * The reads happen to uncached addresses, to avoid
		 * cache invalidation etc...
		 */
		A_NAND_READ(fa, dbs, (unsigned)tmp);
		hdr = *tmp;

		A_PRINTF("hdr: [0x%x : 0x%x : 0x%x : 0x%x]\n",
				hdr.ep, hdr.la, hdr.sz, hdr.cs);

		A_PRINTF("%s: read %u pages\n", __func__, (hdr.sz / dbs));

		A_MEMCPY((A_UINT8 *)hdr.la, tmp + 1, dbs - sizeof(hdr));

		for (i = dbs, fa += nbs; i < hdr.sz; fa += nbs, i += dbs) {
			A_PRINTF("%s: 0x%x 0x%x 0x%x\n", __func__, fa, dbs, hdr.la + i - sizeof(hdr));
			A_NAND_READ(fa, dbs, hdr.la + i - sizeof(hdr));
		}

		if ((i = checksum(hdr.la, hdr.sz - sizeof(hdr))) != hdr.cs) {
			A_PRINTF("Checksum mismatch. 0x%x != 0x%x\n",
					hdr.cs, i);

			while(1);	// What to do

		} else {
			A_PRINTF("f/w %u read complete, jumping to %p\n",
					j, hdr.ep);
		}

		call_fw(HIF_NAND, hdr.ep);

		A_PRINTF("f/w %u execution complete\n", j);
	}
	while (1);
}

void nand_module_install(struct nand_api *api)
{
	api->_nand_init		= nand_flash_init;
	api->_nand_load_fw	= nand_load_fw;
	api->_nand_read		= nand_read;
	api->_nand_param_page	= nand_param_page;
	api->_nand_read_id	= nand_read_id;
}
