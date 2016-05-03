/****************************
 *
 * to use in the kernel space 
 * the module of acc(for LTEFI_V2)
 * autelan
 * 2014.04.13
 *
 ****************************/
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/slab.h>
#include <linux/ioport.h>
#include <linux/fcntl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/delay.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/smp_lock.h>
#include <linux/wait.h>
#include <linux/irq.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/netlink.h>
#include <linux/spinlock.h>
#include <net/sock.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include "check_acc.h"
#include "ar9350.h"

#define ACC_MINOR 134
#define BUTTON_WAIT 3 //rbk add for button irq wait
#if defined (LTEFI_V2)
#define ACC_GPIO  11
#define SHUT_GPIO 12
#endif
#if defined (LTEFI)
#define ACC_GPIO  21
#define SHUT_GPIO 22
#endif
#define ACC_TIMER 267 // cat /proc/cpuinfo
struct timer_list acc_timer;
struct timer_list sig_timer;
struct tasklet_struct acc_tasklet;
int acc_flag=0;
int acc_time=10;
int total_time=0;
int total_time1=300;
int sem_ret=0;
struct semaphore acc_sem;
struct fasync_struct *acc_async;
acc_op_args acc_ioc_op_args;
static int acc_is_open;

void sig_send(int val)
{
	int ret;

	ret = ar7100_reg_rd(AR9350_GPIO_OE);
	printk("0x%x:0x%08x\n", AR9350_GPIO_OE, ret);
	ret = ar7100_reg_rd(AR9350_GPIO_IN);
	printk("0x%x:0x%08x\n", AR9350_GPIO_IN, ret);
	ret = ar7100_reg_rd(AR9350_GPIO_OUT);
	printk("0x%x:0x%08x\n", AR9350_GPIO_OUT, ret);

	printk("SHUT_GPIO by check_acc.ko!\n");
	ar7100_gpio_config_output(SHUT_GPIO); //SDA output
	ar7100_gpio_out_val(SHUT_GPIO, 0x1); // SHUT_GPIO=1
}
void shut_gpio_set(int val)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;

	for(i=0;i<BUTTON_WAIT;i++){
		mdelay(99);
		reg_val = ar7100_gpio_in_val(ACC_GPIO);
		a[i] = reg_val;
		// 	 printk("acc_gpio_1 a[i]=%d\n",a[i]);
		if(a[i] != 1)
			button_flag = 0;
	}
	if(0 == button_flag){
		printk("ACC_TIMER stop!\n");
		// 	 printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		up(&acc_sem);
		// 	 printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		acc_flag = 0;
	}
	else if(total_time >= total_time1){
		printk("check_acc.ko send a SIGIO!\n");
		kill_fasync(&acc_async, SIGIO, POLL_IN);

		sig_timer.function = sig_send;
		sig_timer.expires = jiffies + acc_time*ACC_TIMER;
		add_timer(&sig_timer);
		// 	 printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		up(&acc_sem);
		// 	 printk("%s %d, unlock=%d\n\n", __func__, __LINE__, acc_sem);
		acc_flag = 0;
	}
	else
	{
		acc_timer.expires = jiffies+acc_time*ACC_TIMER;
		total_time += acc_time;
		add_timer(&acc_timer);
		printk("ACC_TIMER restart! ----%d\n", total_time/acc_time);
	}
}
void tasklet_action(unsigned long arg)
{
	acc_flag = 1;
	del_timer(&acc_timer);
	acc_timer.function = shut_gpio_set;
	acc_timer.expires = jiffies+acc_time*ACC_TIMER;
	total_time += acc_time;
	printk("ACC_TIMER start!\n");
	add_timer(&acc_timer);
}
static irqreturn_t acc_gpio_set(int cpl, void *dev_id, struct pt_regs *regs)
{
	unsigned int reg_val;
	int i;
	char a[BUTTON_WAIT];
	char button_flag = 1;

	for(i=0;i<BUTTON_WAIT;i++){
		mdelay(99);
		reg_val = ar7100_gpio_in_val(ACC_GPIO);
		a[i] = reg_val;
		// 	 printk("acc_gpio a[i]=%d\n",a[i]);
		if(a[i] != 1)
			button_flag = 0;
	}
	if(button_flag){
		// 	 printk("%s %d, lock=%d\n\n", __func__, __LINE__, acc_sem);
		sem_ret = down_trylock(&acc_sem);
		// 	 printk("%s %d, lock=%d, sem_ret=%d\n\n", __func__, __LINE__, acc_sem, sem_ret);
		if(sem_ret == 0){
			total_time = 0;
			tasklet_schedule(&acc_tasklet);
		}
	}
	return IRQ_HANDLED;
}
 
int  set_ap83_gpio_irq(void)
{
	unsigned int ret = ar7100_reg_rd(AR9350_GPIO_INT_ENABLE);
	ret |= 1<<ACC_GPIO;
	ar7100_reg_wr(AR9350_GPIO_INT_ENABLE, ret);

	ar7100_gpio_config_input(ACC_GPIO);
	tasklet_init(&acc_tasklet, tasklet_action, 789);
	int req1 = request_irq(AR7100_GPIO_IRQn(ACC_GPIO), acc_gpio_set, IRQF_SAMPLE_RANDOM, "acc_gpio_set", NULL);
	if(req1 !=0){
		printk("irq1 set is wrong and errno is %d\n", req1);
		return -EINVAL;
	}
	return 0;
}
 
static int
acc_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	if (_IOC_TYPE(cmd) != ACC_IOC_MAGIC) return -ENOTTY;
	if (_IOC_NR(cmd) > ACC_IOC_MAXNR) return -ENOTTY;
	memset(&acc_ioc_op_args,0,sizeof(acc_op_args));

switch (cmd) {
	case SET_ACC_GPIO_TIMER:
		if(0 == acc_flag)
		{
			copy_from_user(&acc_ioc_op_args,(acc_op_args *)arg,sizeof(acc_op_args));
			total_time1 =	acc_ioc_op_args.num;		 
			printk("\nmodify the acc_time=%d(sec) now\n", total_time1);
		}
		else
		{
			printk("\nCannot be modified, Kill check_lte first!\n");
		}
		break;
	case GET_ACC_GPIO_TIMER:
		if(0 == acc_flag)
		{
			printk("\nacc_time=%d(sec)\n", total_time1);
		}
		else
		{
			printk("\nacc_time=%d(sec), Cannot be modified\n", total_time1);
		}
		break;
	default:
		return -1;
		break;
}
return 0;
}

 static int acc_open(struct inode* inode, struct file* file)
 {
	 switch (MINOR(inode->i_rdev)) {
		 case ACC_MINOR:
			 if (acc_is_open)
				 return -EBUSY;
			 acc_is_open = 1;
			 return 0;
		 default:
			 return -ENODEV;
	 }
 }
static int acc_close(struct inode* inode, struct file* file)
{
	if (MINOR(inode->i_rdev) == ACC_MINOR) {
		acc_is_open = 0;
	}
	return 0;
}
static int acc_fasync(int fd, struct file* filp, int mode)
{
	return fasync_helper(fd, filp, mode, &acc_async);
}

static struct file_operations acc_fops =
{
	owner:		THIS_MODULE,
	ioctl:		acc_ioctl,
	open:		acc_open,
	release:	acc_close,
	fasync:		acc_fasync,
};
static struct miscdevice acc_miscdev = {
	ACC_MINOR,
	"acc",
	&acc_fops
};
 
static int __init
acc_init(void)
{
	printk("\nin acc_mod init\n");
	misc_register(&acc_miscdev);

	sema_init(&acc_sem, 1);
	init_timer(&acc_timer);
	init_timer(&sig_timer);
#if defined (LTEFI)
	ar7100_gpio_out_val(12, 1);//for LTEFI Ver.D --- 0 disable LTE 1;
	ar7100_gpio_out_val(15, 1);//for LTEFI Ver.D --- 0 reset LTE 1;
	ar7100_gpio_out_val(16, 1);//for LTEFI Ver.D --- 0 disable LTE 2;
	ar7100_gpio_out_val(20, 1);//for LTEFI Ver.D --- 0 reset LTE 2;
#endif
#if defined (LTEFI_V2)
	ar7100_gpio_out_val(16, 0);//for LTEFI_V2 --- 1 disable LTE 1 & 2;
#endif
	ar7100_gpio_config_output(SHUT_GPIO);
	ar7100_gpio_out_val(SHUT_GPIO, 0);
	ar7100_gpio_config_input(ACC_GPIO);

	set_ap83_gpio_irq();

	return 0;
}
static void __exit
acc_exit(void)
{
	del_timer(&acc_timer);
	del_timer(&sig_timer);
	free_irq(AR7100_GPIO_IRQn(ACC_GPIO), NULL);
	misc_deregister(&acc_miscdev);
	printk(KERN_INFO "\nacc_mod exit	MAJOR:10 MINOR:134\n");
}
	
module_init(acc_init);
module_exit(acc_exit);

MODULE_AUTHOR("liuhj@autelan");
MODULE_DESCRIPTION("Support for Atheros WiSoC Register");
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif
		

