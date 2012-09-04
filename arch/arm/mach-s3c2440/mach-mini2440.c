/* linux/arch/arm/mach-s3c2440/mach-mini2440.c
 *
 * Copyright (c) 2004,2005 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * http://www.fluff.org/ben/mini2440/
 *
 * Thanks to Dimity Andric and TomTom for the loan of an SMDK2440.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <plat/regs-serial.h>
#include <mach/regs-gpio.h>
#include <mach/regs-lcd.h>

#include <mach/idle.h>
#include <mach/fb.h>
#include <plat/iic.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <linux/dm9000.h>

#include <plat/s3c2410.h>
#include <plat/s3c2440.h>
#include <plat/clock.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <plat/nand.h>

#include <plat/common-smdk.h>

#include <linux/dm9000.h>

/* DM9000AEP 10/100 ethernet controller */
#define MACH_MINI2440_DM9K_BASE (S3C2410_CS4 + 0x300)

// NEC 3.5”LCD 的配置和参数设置 
#ifdef CONFIG_FB_S3C2410_N240320
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 100000
#define LCD_RIGHT_MARGIN 36
#define LCD_LEFT_MARGIN 19
#define LCD_HSYNC_LEN 5
#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1
// 夏普 8”LCD 的配置和参数设置 
#elif defined(CONFIG_FB_S3C2410_TFT640480)
#define LCD_WIDTH 640
#define LCD_HEIGHT 480
#define LCD_PIXCLOCK 80000
#define LCD_RIGHT_MARGIN 67
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31
#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1
// 统宝 3.5”LCD 的配置和参数设置 
#elif defined(CONFIG_FB_S3C2410_T240320)
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 146250//170000
#define LCD_RIGHT_MARGIN 25
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 4
#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1
// X3.5"LCD 的配置和参数设置 
#elif defined(CONFIG_FB_S3C2410_X240320)
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 170000
#define LCD_RIGHT_MARGIN 25
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 4
#define LCD_UPPER_MARGIN 0
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 9
#define LCD_CON5 (S3C2410_LCDCON5_FRM565 | S3C2410_LCDCON5_INVVDEN | \
S3C2410_LCDCON5_INVVFRAME | S3C2410_LCDCON5_INVVLINE | \
S3C2410_LCDCON5_INVVCLK | S3C2410_LCDCON5_HWSWP )
// 群创 7”LCD 的配置和参数设置
#elif defined(CONFIG_FB_S3C2410_TFT800480)
#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define LCD_PIXCLOCK 11463//40000
#define LCD_RIGHT_MARGIN 67
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31
#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1
// LCD2VGA(分辨率为 1024x768)模块的配置和参数设置 
#elif defined(CONFIG_FB_S3C2410_VGA1024768)
#define LCD_WIDTH 1024
#define LCD_HEIGHT 768
#define LCD_PIXCLOCK 80000
#define LCD_RIGHT_MARGIN 15
#define LCD_LEFT_MARGIN 199
#define LCD_HSYNC_LEN 15
#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 1
#define LCD_VSYNC_LEN 1
#define LCD_CON5 (S3C2410_LCDCON5_FRM565 | S3C2410_LCDCON5_HWSWP)
#endif

/* DM9000AEP 10/100 ethernet controller */
#define MACH_MINI2440_DM9K_BASE (S3C2410_CS4 + 0x300)

/**** The new DM9000 modify for mini2440*****************/
static struct resource mini2440_dm9k_resource[] = {
	[0] = {
		.start = MACH_MINI2440_DM9K_BASE,
		.end = MACH_MINI2440_DM9K_BASE + 3,
		.flags = IORESOURCE_MEM
	},
	[1] = {
		.start = MACH_MINI2440_DM9K_BASE + 4,
		.end = MACH_MINI2440_DM9K_BASE + 7,
		.flags = IORESOURCE_MEM
	},
	[2] = {
		.start = IRQ_EINT7,
		.end = IRQ_EINT7,
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE,
	}
};
/*
* * * The DM9000 has no eeprom, and it's MAC address is set by
* * * the bootloader before starting the kernel.
*
*
*/
static struct dm9000_plat_data mini2440_dm9k_pdata = {
	.flags = (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM),
};

static struct platform_device mini2440_device_eth = {
	.name	= "dm9000",
	.id	= -1,
	.num_resources = ARRAY_SIZE(mini2440_dm9k_resource),
	.resource	= mini2440_dm9k_resource,
	.dev	={
		.platform_data = &mini2440_dm9k_pdata,
	},
};

static struct map_desc mini2400_iodesc[] __initdata = {
	/* ISA IO Space map (memory space selected by A24) */

	{
		.virtual	= (u32)S3C24XX_VA_ISA_WORD,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_WORD + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE,
		.pfn		= __phys_to_pfn(S3C2410_CS2),
		.length		= 0x10000,
		.type		= MT_DEVICE,
	}, {
		.virtual	= (u32)S3C24XX_VA_ISA_BYTE + 0x10000,
		.pfn		= __phys_to_pfn(S3C2410_CS2 + (1<<24)),
		.length		= SZ_4M,
		.type		= MT_DEVICE,
	}
};

#define UCON S3C2410_UCON_DEFAULT | S3C2410_UCON_UCLK
#define ULCON S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB
#define UFCON S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE

static struct s3c2410_uartcfg mini2440_uartcfgs[] __initdata = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	[1] = {
		.hwport	     = 1,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x03,
		.ufcon	     = 0x51,
	},
	/* IR port */
	[2] = {
		.hwport	     = 2,
		.flags	     = 0,
		.ucon	     = 0x3c5,
		.ulcon	     = 0x43,
		.ufcon	     = 0x51,
	}
};

// 以下蓝色部分为说明文字
static struct mtd_partition mini2440_default_nand_part[] = {
	[0] = {
		.name = "supervivi", // 这里是 bootloader 所在的分区,可以放置 u-boot, supervivi 等内容,对应/dev/mtdblock0
		.size = 0x00040000,
		.offset = 0,
	},
	[1] = {
		.name = "param", // 这里是 supervivi 的参数区,其实也属于 bootloader 的一部分,如果 u-boot 比较大,可以把此区域覆盖掉,不会影响系统启动,对应/dev/mtdblock1
		.offset = 0x00040000,
		.size = 0x00020000,
	},
	[2] = {
		.name = "Kernel", // 内核所在的分区,大小为 5M,足够放下大部分自己定制的巨型内核了,比如内核使用了更大的 Linux Logo 图片等,对应/dev/mtdblock2
		.offset = 0x00060000,
		.size = 0x00500000,
	},
	[3] = {
		.name = "root", //文件系统分区,友善之臂主要用来存放 yaffs2 文件系统内容,对应/dev/mtdblock3
		.offset = 0x00560000,
		.size = 1024 * 1024 * 1024, //
	},
	[4] = {
		.name = "nand", // 此区域代表了整片的 nand flash,主要是预留使用,比如以后可以通过应用程序访问读取/dev/mtdblock4 就能实现备份整片 nand flash 了。
		.offset = 0x00000000,
		.size = 1024 * 1024 * 1024, //
	}
};
// 这里是开发板的 nand flash 设置表,因为板子上只有一片,因此也就只有一个表
static struct s3c2410_nand_set mini2440_nand_sets[] = {
	[0] = {
		.name = "NAND",
		.nr_chips = 1,
		.nr_partitions = ARRAY_SIZE(mini2440_default_nand_part),
		.partitions = mini2440_default_nand_part,
	},
};
// 这里是 nand flash 本身的一些特性,一般需要对照 datasheet 填写,大部分情况下按照以下参数填写即可
static struct s3c2410_platform_nand mini2440_nand_info = {
	.tacls = 20,
	.twrph0 = 60,
	.twrph1 = 20,
	.nr_sets = ARRAY_SIZE(mini2440_nand_sets),
	.sets = mini2440_nand_sets,
	.ignore_unset_ecc = 1,
};

/* DM9000 */
static struct resource mini2440_dm9k_resource[] = {
	[0] = {
		.start = MACH_MINI2440_DM9K_BASE,
		.end = MACH_MINI2440_DM9K_BASE + 3,
		.flags = IORESOURCE_MEM
	},
	[1] = {
		.start = MACH_MINI2440_DM9K_BASE + 4,
		.end = MACH_MINI2440_DM9K_BASE + 7,
		.flags = IORESOURCE_MEM
	},
	[2] = {
		.start = IRQ_EINT7,
		.end = IRQ_EINT7,
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHEDGE,
	}
};
/*
* * * The DM9000 has no eeprom, and it's MAC address is set by
* * * the bootloader before starting the kernel.
*
*
*/
static struct dm9000_plat_data mini2440_dm9k_pdata = {
	.flags= (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM),
};

static struct platform_device mini2440_device_eth = {
	.name	= "dm9000",
	.id	= -1,
	.num_resources = ARRAY_SIZE(mini2440_dm9k_resource),
	.resource	= mini2440_dm9k_resource,
	.dev	={
	.platform_data = &mini2440_dm9k_pdata,
	},
};


/* LCD driver info */

// #if defined (LCD_WIDTH)
static struct s3c2410fb_display mini2440_lcd_cfg __initdata = {
#if !defined (LCD_CON5)
	.lcdcon5 = S3C2410_LCDCON5_FRM565 | S3C2410_LCDCON5_INVVLINE | S3C2410_LCDCON5_INVVFRAME | S3C2410_LCDCON5_PWREN | S3C2410_LCDCON5_HWSWP,
#else
	.lcdcon5 = LCD_CON5,
#endif
	.type = S3C2410_LCDCON1_TFT,
	.width = LCD_WIDTH,
	.height = LCD_HEIGHT,
	.pixclock = LCD_PIXCLOCK,
	.xres = LCD_WIDTH,
	.yres = LCD_HEIGHT,
	.bpp = 16,
	.left_margin = LCD_LEFT_MARGIN + 1,
	.right_margin = LCD_RIGHT_MARGIN + 1,
	.hsync_len = LCD_HSYNC_LEN + 1,
	.upper_margin = LCD_UPPER_MARGIN + 1,
	.lower_margin = LCD_LOWER_MARGIN + 1,
	.vsync_len = LCD_VSYNC_LEN + 1,
};

static struct s3c2410fb_mach_info mini2440_fb_info __initdata = {
	.displays = &mini2440_lcd_cfg,
	.num_displays = 1,
	.default_display = 0,
	.gpccon = 0xaa955699,
	.gpccon_mask = 0xffc003cc,
	.gpcup = 0x0000ffff,
	.gpcup_mask = 0xffffffff,
	.gpdcon = 0xaa95aaa1,
	.gpdcon_mask = 0xffc0fff0,
	.gpdup = 0x0000faff,
	.gpdup_mask = 0xffffffff,
	.lpcsel = 0xf82,
};
//#endif

static struct platform_device *mini2440_devices[] __initdata = {
	&s3c_device_usb,
	&s3c_device_lcd,
	&s3c_device_wdt,
	&s3c_device_i2c0,
	&s3c_device_iis,
	&s3c_device_nand,
	&s3c_device_rtc,
	&mini2440_device_eth,
};

static void __init mini2440_map_io(void)
{
	s3c_device_nand.dev.platform_data = &mini2440_nand_info;
	s3c24xx_init_io(mini2440_iodesc, ARRAY_SIZE(mini2440_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(mini2440_uartcfgs, ARRAY_SIZE(mini2440_uartcfgs));
//	s3c_device_nand.dev.platform_data = &mini2440_nand_info;
}

static void __init mini2440_machine_init(void)
{
	s3c24xx_fb_set_platdata(&mini2440_fb_info);
	s3c_i2c0_set_platdata(NULL);

	platform_add_devices(mini2440_devices, ARRAY_SIZE(mini2440_devices));
//	smdk_machine_init();
}

MACHINE_START(MINI2440, "mini2440")
	/* Maintainer: Ben Dooks <ben@fluff.org> */
	.phys_io	= S3C2410_PA_UART,
	.io_pg_offst	= (((u32)S3C24XX_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S3C2410_SDRAM_PA + 0x100,

	.init_irq	= s3c24xx_init_irq,
	.map_io		= mini2440_map_io,
	.init_machine	= mini2440_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
