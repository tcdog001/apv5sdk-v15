typedef struct acc_op_args_s {
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
} acc_op_args;

#define ACC_IOC_MAGIC 10 
#define ACC_IOC_RESET	_IO(ACC_IOC_MAGIC,0)
#define SET_ACC_GPIO_TIMER _IOWR(ACC_IOC_MAGIC,1,acc_op_args)/*SET_ACC_GPIO_TIMER*/
#define GET_ACC_GPIO_TIMER _IOWR(ACC_IOC_MAGIC,2,acc_op_args)/*GET_ACC_GPIO_TIMER*/
#define ACC_IOC_MAXNR 2


