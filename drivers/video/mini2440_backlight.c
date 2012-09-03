#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/regs-clock.h>
#include <plat/regs-timer.h>
#include <mach/regs-gpio.h>
#include <linux/cdev.h>
#undef DEBUG
//#define DEBUG
#ifdef DEBUG
	#define DPRINTK(x...) {printk(__FUNCTION__"(%d): ",__LINE__);printk(##x);}
#else
	#define DPRINTK(x...) (void)(0)
#endif
/* 定义背光驱动的名称为 backligh,将会出现在/dev/backlight */
#define DEVICE_NAME "backlight" 
/* 定义背光变量 bl_state,以记录背光的开关状态 */
static unsigned int bl_state;
/* 设置背光开关的函数,主要是翻转背光变量 bl_state */

static inline void set_bl(int state)
{
	bl_state = !!state; //翻转 bl_state 变量
	s3c2410_gpio_setpin(S3C2410_GPG(4), bl_state); //把结果写入背光所用的寄存器 GPG4
}

/* 获取背光状态 */
static inline unsigned int get_bl(void)
{
	return bl_state;
}

/* 从应用程序读取参数,并传递到内核中 */
static ssize_t dev_write(struct file *file, const char *buffer, size_t count, loff_t * ppos)
{
	unsigned char ch;
	int ret;
	if (count == 0)
	{	return count;	}
	//使用 copy_from_user 函数从用户层/应用层读取参数
	ret = copy_from_user(&ch, buffer, sizeof ch) ? -EFAULT : 0;
	if (ret) 
	{	return ret;		}
	ch &= 0x01; //判断奇数还是偶数
	set_bl(ch); //设置背光状态
	return count;
}

/* 把内核参数传递给用户层/应用层的读函数 */
static ssize_t dev_read(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{
	int ret;
	unsigned char str[] = {'0', '1' };
	if (count == 0) 
	{	return 0;	}
	//使用 copy_to_user 函数把内核参数传递到用户层/应用层
	ret = copy_to_user(buffer, str + get_bl(), sizeof(unsigned char) ) ? -EFAULT : 0;
	if (ret) 
	{	return ret;	}
	return sizeof(unsigned char);
}

/* 设备操作集 */
static struct file_operations dev_fops = {
	owner: THIS_MODULE,
	read: dev_read,
	write: dev_write,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

/* 设备初始化,内核启动时就有效 */
static int __init dev_init(void)
{
	int ret;
	ret = misc_register(&misc);
	printk (DEVICE_NAME"\tinitialized\n");
	//初始化背光所用的端口 GPG4 为输出
	s3c2410_gpio_cfgpin(S3C2410_GPG(4), S3C2410_GPIO_OUTPUT);
	//启动内核时打开背光
	set_bl(1);
	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc);
}
module_init(dev_init); //注册背光驱动模块
module_exit(dev_exit); //卸载背光驱动模块
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyARM Inc.");
