#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/regs-clock.h>
#include <plat/regs-timer.h>
#include <plat/regs-adc.h>
#include <mach/regs-gpio.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
//自己定义的头文件,因原生内核并没有包含
#include "s3c24xx-adc.h"
#undef DEBUG
//#define DEBUG
#ifdef DEBUG
#define DPRINTK(x...) {printk(__FUNCTION__"(%d): ",__LINE__);printk(##x);}
#else
#define DPRINTK(x...) (void)(0)
#endif
//定义 ADC 转换设备名称,将出现在/dev/adc
#define DEVICE_NAME "adc"
static void __iomem *base_addr;

//定义 ADC 设备结构
typedef struct {
wait_queue_head_t wait;
	int channel;
	int prescale;
}ADC_DEV;
//声明全局信号量,以便和触摸屏驱动程序共享 A/D 转换器
DECLARE_MUTEX(ADC_LOCK);
//ADC 驱动是否拥有 A/D 转换器资源的状态变量
static int OwnADC = 0;
static ADC_DEV adcdev;
static volatile int ev_adc = 0;
static int adc_data;
static struct clk *adc_clock;
//定义 ADC 相关的寄存器
#define ADCCON (*(volatile unsigned long *)(base_addr + S3C2410_ADCCON)) //ADC control
#define ADCTSC (*(volatile unsigned long *)(base_addr + S3C2410_ADCTSC)) //ADC touch screen control
#define ADCDLY (*(volatile unsigned long *)(base_addr + S3C2410_ADCDLY)) //ADC start or Interval Delay
#define ADCDAT0 (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT0)) //ADC conversion data 0
#define ADCDAT1 (*(volatile unsigned long *)(base_addr + S3C2410_ADCDAT1)) //ADC conversion data 1
#define ADCUPDN (*(volatile unsigned long *)(base_addr + 0x14)) //Stylus Up/Down interrupt status
#define PRESCALE_DIS (0 << 14)
#define PRESCALE_EN (1 << 14)
#define PRSCVL(x) ((x) << 6)
#define ADC_INPUT(x) ((x) << 3)
#define ADC_START (1 << 0)
#define ADC_ENDCVT (1 << 15)

//定义“开启 AD 输入”宏,因为比较简单,故没有做成函数
#define START_ADC_AIN(ch, prescale) do{\
ADCCON = PRESCALE_EN | PRSCVL(prescale) | \
ADC_INPUT((ch));\
ADCCON |= ADC_START;}while(0)

//ADC 中断处理函数
static irqreturn_t adcdone_int_handler(int irq, void *dev_id)
{
	//如果 ADC 驱动拥有“A/D 转换器”资源,则从 ADC 寄存器读取转换结果
	if (OwnADC) 
	{
		adc_data = ADCDAT0 & 0x3ff;
		ev_adc = 1;
		wake_up_interruptible(&adcdev.wait);
	}
	return IRQ_HANDLED;
}
//ADC 读函数,一般对应于用户层/应用层的设备读函数(read)
static ssize_t s3c2410_adc_read(struct file *filp, char *buffer, size_t count, loff_t *ppos)
{
	char str[20];
	int value;
	size_t len;
	//判断“A/D 转换器”资源是否可用
	if (down_trylock(&ADC_LOCK) == 0) 
	{
		OwnADC = 1; //标记“A/D 转换器”资源状态为可用
		START_ADC_AIN(adcdev.channel, adcdev.prescale); //开始转换
		wait_event_interruptible(adcdev.wait, ev_adc); //通过终端的方式等待转换结果
		ev_adc = 0;
		DPRINTK("AIN[%d] = 0x%04x, %d\n", adcdev.channel, adc_data, ADCCON & 0x80 ? 1:0);
		//把转换结果赋予 value,以便传递到用户层/应用层
		value = adc_data;
		//释放“A/D 转换器”资源
		OwnADC = 0;
		up(&ADC_LOCK);
	}
	else 
	{		//没有“A/D 转换器”资源,赋值为“-1”
		value = -1;
	}
	len = sprintf(str, "%d\n", value);
	if (count >= len) 
	{	//把转换结果传递到用户层/应用层
		int r = copy_to_user(buffer, str, len);
		return r ? r : len;
	}
	else 
	{
		return -EINVAL;
	}
}

//打开 ADC 设备的函数,一般对应于用户态程序的 open
static int s3c2410_adc_open(struct inode *inode, struct file *filp)
{
	//初始化中断队列
	init_waitqueue_head(&(adcdev.wait));
	//缺省通道为“0”
	adcdev.channel=0;
	adcdev.prescale=0xff;
	DPRINTK( "adc opened\n");
	return 0;
}

static int s3c2410_adc_release(struct inode *inode, struct file *filp)
{
	DPRINTK( "adc closed\n");
	return 0;
}

static struct file_operations dev_fops = {
	owner: THIS_MODULE,
	open: s3c2410_adc_open,
	read:s3c2410_adc_read,
	release: s3c2410_adc_release,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;
	base_addr=ioremap(S3C2410_PA_ADC,0x20);
	if (base_addr == NULL) 
	{
		printk(KERN_ERR "Failed to remap register block\n");
		return -ENOMEM;
	}
	adc_clock = clk_get(NULL, "adc");
	if (!adc_clock) 
	{
		printk(KERN_ERR "failed to get adc clock source\n");
		return -ENOENT;
	}
	clk_enable(adc_clock);
	/* normal ADC */
	ADCTSC = 0;
	//注册中断
	ret = request_irq(IRQ_ADC, adcdone_int_handler, IRQF_SHARED, DEVICE_NAME, &adcdev);
	if (ret) 
	{
		iounmap(base_addr);
		return ret;
	}
	//注册设备
	ret = misc_register(&misc);
	printk (DEVICE_NAME"\tinitialized\n");
	return ret;
}

static void __exit dev_exit(void)
{
	//释放中断
	free_irq(IRQ_ADC, &adcdev);
	iounmap(base_addr);
	if (adc_clock) 
	{
		clk_disable(adc_clock);
		clk_put(adc_clock);
		adc_clock = NULL;
	}
	misc_deregister(&misc);
}
//导出信号量“ADC_LOCK”
//以便触摸屏驱动使用
EXPORT_SYMBOL(ADC_LOCK);
module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FriendlyARM Inc.");
