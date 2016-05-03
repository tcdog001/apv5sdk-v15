typedef struct wrcpu_op_args_s {
	unsigned long  op_addr;
	unsigned long  op_value; // ignore on read in arg, fill read value on write return value
	unsigned int offset;
	unsigned short op_len;
	unsigned short op_ret; // 0 for success, other value indicate different failure.
	unsigned int num;//which gpio
	unsigned int status;/*0 off 1 on*/
	unsigned int factory_reset_get;
	volatile unsigned char ap83Mac[13]; //ghy for ap83
	unsigned char wnapSN[32];
	unsigned char hwver[10];
	unsigned char ProductId[64]; //ghy for ap83
	unsigned char oem_data[512];//20090927
	int product_test_flag_app;
	int temperature;
} wrcpu_op_args;

typedef struct network_ioctl_s{
	unsigned int ioctl_magic;
	unsigned int ioctl_ipaddr;
	unsigned int ioctl_submask;
	unsigned int ioctl_gw;
	unsigned int ioctl_dns;
	unsigned int ioctl_vlan; 
	unsigned int ioctl_dhcp; 
}network_ioctl;

#define BM_MINOR_NUM 131
#define BM_MAJOR_NUM 10
#define WRCPU_IOC_MAGIC 10 
#define WRCPU_IOC_RESET	_IO(WRCPU_IOC_MAGIC,0)
#define ROOTFS_BEGIN_ADDR 0xBFC30000
int product_test_flag_app;
unsigned long value;
unsigned char *value_char;
//zhengkun add new struction for apv6 to transmit parameter
struct hlds_ctrl_t {
	int ctrl;	/* HLDS_LED_CTRL above */
	int device; 	/* Device index, like from HLDS_LED_IDX */
	int reset;
};
struct hlds_ctrl_t  wrcpu_args;

//zhengkun add end
/*
 *  * ST Microelectronics Opcodes for Serial Flash
 *   */

#if defined(XHAP93)||defined(XJAP93)|| defined ( XH9344 )|| defined ( XJ9344 )
#define PRO_BUF 64
#define HW_BUF 10
#define MAC_BUF 13
#define SN_BUF 32

#define PRO_OFFSET 0
#define HW_OFFSET 64
#define MAC_OFFSET 74
#define SN_OFFSET 87
#else
#ifdef XJAP93_02
#define PRO_BUF 32
#define HW_BUF 10
#define MAC_BUF 6
#define SN_BUF 32

#define PRO_OFFSET 127
#define HW_OFFSET 64
#define MAC_OFFSET 4
#define SN_OFFSET 83
#else
#ifdef NGWNAP350
#define PRO_BUF 8
#define HW_BUF 3
#define MAC_BUF 12
#define SN_BUF 13
/*in WNAP350 nou use the offset*/
#define PRO_OFFSET 0
#define HW_OFFSET 32
#define MAC_OFFSET 48
#define SN_OFFSET 80
#else
#ifdef AP2400
#define PRO_BUF 23   //update by wangjr for ap mode length
#define MAC_BUF 6
#define SN_BUF 22
#define HW_BUF 3
#define ENTRY_BUF 4

#define ENTRY_OFFSET 796
#define PRO_OFFSET 128
#define MAC_OFFSET 0
#define SN_OFFSET 24
#define HW_OFFSET 3
#else  
/*product info and ENTRY addr*/
#if defined(XJPB44_02) || defined(TDPB44_02)
#define PRO_BUF 32   //update by wangjr for ap mode length
#define MAC_BUF 6
#define SN_BUF 32
#define HW_BUF 3
#define ENTRY_BUF 4	

#define ENTRY_OFFSET 1052
#define PRO_OFFSET 127
#define MAC_OFFSET 4
#define SN_OFFSET 83
#define HW_OFFSET 34
#else  
#ifdef WNAP550
#define PRO_BUF 54
#define MAC_BUF 6
#define SN_BUF 64
#define HW_BUF 3

#define PRO_OFFSET 57644
#define MAC_OFFSET 102
#define SN_OFFSET 57344
#define HW_OFFSET 57600
#else  
#define PRO_BUF 19
#define HW_BUF 13
#define MAC_BUF 12
#define SN_BUF 22

#define PRO_OFFSET 0
#define HW_OFFSET 64
#define MAC_OFFSET 48
#define SN_OFFSET 87
#endif
#endif
#endif
#endif
#endif
#endif


#ifdef WNAP550
#define OEM_OFFSET 112
#define OEM_BUF 128
#else
#ifdef XJAP93_02
#define OEM_OFFSET 160
#define OEM_BUF 128
#else
#define OEM_OFFSET 119
#define OEM_BUF 512
#endif
#endif
#define max_mtd 65536

#ifdef WNAP210
#define AP83_BUTTON 21
#else 
#ifdef XHAP93
#define AP83_BUTTON 12
#else
#ifdef XJAP93
#define AP83_BUTTON 11
#else
#ifdef XJAP93_02
#define AP83_BUTTON 0
#else
#ifdef AQ2000N
#define AP83_BUTTON 12
#else
#ifdef XH9344 
#define AP83_BUTTON 15
#else
#ifdef XJ9344
#define AP83_BUTTON 15
#else
#if defined( PC018 )&& !defined( XH9344 )&& !defined( XJ9344 ) && !defined (AQ2000_X)
#define AP83_BUTTON 4
#else
#ifdef AE5000ENT
#define AP83_BUTTON 12
#else
#if defined (AQ2000_X)
#define AP83_BUTTON 17
#else
#define AP83_BUTTON 8
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
#endif
/* wangyu add for product informations */
#if defined(AQ2000N)|| defined (AQ2000_X)||defined( PC018 ) || defined (AE5000ENT) && !defined( XH9344 )&& !defined ( XJ9344 )
#define   product_mtdblock	"/dev/mtdblock7"
#endif
#if defined(XJAP93)||defined(XHAP93) || defined( XJAP93_02 )|| defined( XH9344 ) || defined ( XJ9344 )
#define   product_mtdblock	"/dev/mtdblock5"
#endif
#if defined( ARMADA370 ) || defined (AP2400)
#define   product_mtdblock	"/dev/mtdblock4"
#endif
#if defined(XJPB44_02) || defined(TDPB44_02)
#define   product_mtdblock	"/dev/mtdblock1"
#endif
#ifdef WNAP550
#define   product_mtdblock	"/dev/mtdblock8"
#endif
#ifdef APM82181
#define   product_mtdblock	"/dev/mtdblock3"
#endif
/* wangyu add for product informations end */

#define STM_OP_WR_ENABLE       0x06     /* Write Enable */
#define STM_OP_WR_DISABLE      0x04     /* Write Disable */
#define STM_OP_RD_STATUS       0x05     /* Read Status */
#define STM_OP_WR_STATUS       0x01     /* Write Status */
#define STM_OP_RD_DATA         0x03     /* Read Data */
#define STM_OP_FAST_RD_DATA    0x0b     /* Fast Read Data */
#define STM_OP_PAGE_PGRM       0x02     /* Page Program */
#define STM_OP_SECTOR_ERASE    0xd8     /* Sector Erase */
#define STM_OP_BULK_ERASE      0xc7     /* Bulk Erase */
#define STM_OP_DEEP_PWRDOWN    0xb9     /* Deep Power-Down Mode */
#define STM_OP_RD_SIG          0xab     /* Read Electronic Signature */

#define STM_STATUS_WIP       0x01       /* Write-In-Progress */
#define STM_STATUS_WEL       0x02       /* Write Enable Latch */
#define STM_STATUS_BP0       0x04       /* Block Protect 0 */
#define STM_STATUS_BP1       0x08       /* Block Protect 1 */
#define STM_STATUS_BP2       0x10       /* Block Protect 2 */
#define STM_STATUS_SRWD      0x80       /* Status Register Write Disable */
/*mipsidt334a*/
// Device properties

#define CYGNUM_FLASH_INTERLEAVE	(1)
#define CYGNUM_FLASH_SERIES	(1)
#define CYGNUM_FLASH_WIDTH      (8)
#define CYGNUM_FLASH_BASE 	(0xbfc00000u)
/*
 ** S means "Set" through a ptr,
 ** T means "Tell" directly with the argument value
 ** G means "Get": reply by setting through a pointer
 ** Q means "Query": response is on the return value
 ** X means "eXchange": switch G and S atomically
 ** H means "sHift": switch T and Q atomically
 **/
#define WRCPU_IOC_G_  _IOWR(WRCPU_IOC_MAGIC,1,wrcpu_op_args) // read values
#define WRCPU_IOC_X_ _IOWR(WRCPU_IOC_MAGIC,2,wrcpu_op_args) // write and readout wrote value
#define WRCPU_IOC_S_ _IOWR(WRCPU_IOC_MAGIC,3,wrcpu_op_args)//set the select
#define WRCPU_IOC_D_ _IOWR(WRCPU_IOC_MAGIC,4,wrcpu_op_args)//down the blind timer
#define WRCPU_IOC_U_ _IOWR(WRCPU_IOC_MAGIC,5,wrcpu_op_args)/*up the blinding*/ 
#define WRCPU_IOC_B_ _IOWR(WRCPU_IOC_MAGIC,6,wrcpu_op_args)//blue led on 
#define WRCPU_IOC_I_ _IOWR(WRCPU_IOC_MAGIC,7,wrcpu_op_args)/*get the sysinf*/
#define WRCPU_IOC_O_ _IOWR(WRCPU_IOC_MAGIC,13,wrcpu_op_args)/*open avr*/
#define WRCPU_FLASH_W_ _IOWR(WRCPU_IOC_MAGIC,8,wrcpu_op_args)/*write flash*/
#define WRCPU_WATCHDOG _IOWR(WRCPU_IOC_MAGIC,9,wrcpu_op_args)/*write watchdog*/
#define FACTORY_RESET _IOWR(WRCPU_IOC_MAGIC,10,wrcpu_op_args)/*open factory reset*/
#define WRCPU_IOC_L_ _IOWR(WRCPU_IOC_MAGIC,11,wrcpu_op_args)/*listen factory reset*/
#define FREE_IRQ _IOWR(WRCPU_IOC_MAGIC,12,wrcpu_op_args)/*button factory reset*/
#define NETGEAR_DOWN_LED _IOWR(WRCPU_IOC_MAGIC,14,wrcpu_op_args)/*NETGEAR_DOWN_LED*/
#define SET_IRQ _IOWR(WRCPU_IOC_MAGIC,16,wrcpu_op_args)/*NETGEAR_YELLOW_ON*/
#define NETGEAR_LED_BLINK _IOWR(WRCPU_IOC_MAGIC,15,wrcpu_op_args)/*NETGEAR_BLINK*/
#define WOC_GET_TEMPUTER _IOWR(WRCPU_IOC_MAGIC,17,wrcpu_op_args)/*get the temperature*/
#define WRCPU_POWER_IN _IOWR(WRCPU_IOC_MAGIC,18,wrcpu_op_args)/*check the POE*/
#define WRCPU_POWER_OUT _IOWR(WRCPU_IOC_MAGIC,19,wrcpu_op_args)/*check the PA power*/
/*begin:wangyu add for apv5 temperature control*/
#define SET_TEMP_UP _IOWR(WRCPU_IOC_MAGIC,21,wrcpu_op_args)/*temperature rise*/
#define SET_TEMP_DOWN _IOWR(WRCPU_IOC_MAGIC,20,wrcpu_op_args)/*temperature down*/
/*end:wangyu add for apv5 temperature control*/
/*Begin:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
#define TURN_OFF_ALL_LED _IOWR(WRCPU_IOC_MAGIC,22,wrcpu_op_args)/*turn off all the linghts for AE5000*/
#define TURN_ON_ALL_LED _IOWR(WRCPU_IOC_MAGIC,23,wrcpu_op_args)/*turn off all the linghts for AE5000*/
/*End:Add by zhangisyu for AE5000 turning off all the lights 2012-12-10*/
/*zhengkun add new IO interface for apv6*/
#define WRCPU_IOC_V6_ _IOWR(WRCPU_IOC_MAGIC,1,wrcpu_args)
/*End adding by zhengkun*/
#if defined (LTEFI)
#define SET_ACC_GPIO_TIMER _IOWR(WRCPU_IOC_MAGIC,24,wrcpu_op_args)/*SET_ACC_GPIO_TIMER*/
#define GET_ACC_GPIO_TIMER _IOWR(WRCPU_IOC_MAGIC,25,wrcpu_op_args)/*GET_ACC_GPIO_TIMER*/
#define WRCPU_IOC_MAXNR 26
#else
#define WRCPU_IOC_MAXNR 24/*Add by zhangsiyu for AE5000ENT turn off all lights 2012-12-12*/
#endif
//#define WRCPU_IOC_MAXNR 22
//-----------------------------------------------------------------------------
//// 32 bit access.hal_io.h
//// Individual and vectorized access to 32 bit registers.
#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int *)(_register_)))

#define HAL_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))

#define HAL_READ_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )    \
{                                                                       \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        (_buf_)[_i_] = ((volatile unsigned int *)(_register_))[_j_];      \
}

#define HAL_WRITE_UINT32_VECTOR( _register_, _buf_, _count_, _step_ )   \
{                                                                       \
    cyg_count32 _i_,_j_;                                                \
    for( _i_ = 0, _j_ = 0; _i_ < (_count_); _i_++, _j_ += (_step_))     \
        ((volatile unsigned int *)(_register_))[_j_] = (_buf_)[_i_];      \
}
