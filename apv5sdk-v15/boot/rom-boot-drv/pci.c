#include <asm/types.h>
#include <asm/addrspace.h>
#include <ar934x_soc.h>
#include "dt_defs.h"
#include "rom.addrs.h"

static int  ar7240_local_write_config(int where, int size, uint32_t value);

static int
ar7240_local_write_config(int where, int size, uint32_t value)
{
	ar7240_reg_wr((AR7240_PCI_CRP + where),value);
	return 0;
}


int pci_init_board (void)
{
	uint32_t cmd;

	if ((ar7240_reg_rd(WASP_BOOTSTRAP_REG) & WASP_REF_CLK_25) == 0) {
		ar7240_reg_wr_nf(AR934X_PCIE_PLL_DITHER_DIV_MAX,
			PCIE_PLL_DITHER_DIV_MAX_EN_DITHER_SET(0) |
			PCIE_PLL_DITHER_DIV_MAX_USE_MAX_SET(1) |
			PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_INT_SET(0x20) |
			PCIE_PLL_DITHER_DIV_MAX_DIV_MAX_FRAC_SET(0));
	}

	ar7240_reg_rmw_clear(AR7240_RESET, AR7240_RESET_PCIE_PHY);

	if ((ar7240_reg_rd(RST_REVISION_ID_ADDRESS) & 0xf) == 0) {
		ar7240_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
			PCIE_PLL_CONFIG_REFDIV_SET(1) |
			PCIE_PLL_CONFIG_BYPASS_SET(1) |
			PCIE_PLL_CONFIG_PLLPWD_SET(1));
		udelay(10000);
		ar7240_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
			PCIE_PLL_CONFIG_REFDIV_SET(1) |
			PCIE_PLL_CONFIG_BYPASS_SET(1) |
			PCIE_PLL_CONFIG_PLLPWD_SET(0));
		udelay(1000);
		ar7240_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
			ar7240_reg_rd(AR934X_PCIE_PLL_CONFIG) &
			(~PCIE_PLL_CONFIG_BYPASS_SET(1)));
		udelay(1000);
	} else {
		ar7240_reg_wr_nf(AR934X_PCIE_PLL_CONFIG,
			PCIE_PLL_CONFIG_REFDIV_SET(1) |
			PCIE_PLL_CONFIG_BYPASS_SET(0) |
			PCIE_PLL_CONFIG_PLLPWD_SET(1));
		if ((ar7240_reg_rd(WASP_BOOTSTRAP_REG) & WASP_REF_CLK_25) == 0) {
			ar7240_reg_wr_nf(0xb8116c00, (0x5 << 27) | (160 << 18) | 0);
		} else {
			ar7240_reg_wr_nf(0xb8116c00, (0x8 << 27) | (160 << 18) | 0);
		}
		ar7240_reg_wr_nf(0xb8116c04, (0x1 << 30) | (0x4 << 26) | (0x10 << 19) | (1 << 16) | (3 << 13) | (0x1e << 7));
		ar7240_reg_wr_nf(0xb8116c08, (6 << 23));
		ar7240_reg_wr_nf(0xb8116c04, (0x1 << 30) | (0x4 << 26) | (0x10 << 19) | (3 << 13) | (0x1e << 7));
	}

	ar7240_reg_rmw_clear(AR7240_RESET, AR7240_RESET_PCIE);

	cmd = PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER | PCI_COMMAND_INVALIDATE |
	      PCI_COMMAND_PARITY|PCI_COMMAND_SERR|PCI_COMMAND_FAST_BACK;

	ar7240_local_write_config(PCI_COMMAND, 4, cmd);
	ar7240_local_write_config(0x20, 4, 0x1ff01000);
	ar7240_local_write_config(0x24, 4, 0x1ff01000);

	ar7240_reg_wr(0x180f0000, 0x1ffc1);

	udelay(100000);

	/*
	 * Check if the WLAN PCI-E H/W is present, If the
	 * WLAN H/W is not present, skip the PCI platform
	 * initialization code and return
	 */

	if (((ar7240_reg_rd(AR7240_PCI_LCL_RESET)) & 0x1) == 0x0) {
		serial_puts("*** Warning *** : PCIe WLAN Module not found !!!\n");
		return;
	}

        ar7240_reg_wr((AR7240_PCI_DEV_CFGBASE + 0x10), 0xffff);

	return 0;
}

