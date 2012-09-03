cmd_arch/arm/mach-s3c2410/sleep.o := arm-linux-gcc -Wp,-MD,arch/arm/mach-s3c2410/.sleep.o.d  -nostdinc -isystem /usr/local/arm/4.3.2/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -Iinclude  -I/home/xfile/myProject/linux-2.6.32.2/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-s3c2410/include -Iarch/arm/mach-s3c2400/include -Iarch/arm/mach-s3c2412/include -Iarch/arm/mach-s3c2440/include -Iarch/arm/mach-s3c2442/include -Iarch/arm/mach-s3c2443/include -Iarch/arm/plat-s3c24xx/include -Iarch/arm/plat-s3c/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=4 -march=armv4t -mtune=arm9tdmi -include asm/unified.h -msoft-float -gdwarf-2       -c -o arch/arm/mach-s3c2410/sleep.o arch/arm/mach-s3c2410/sleep.S

deps_arch/arm/mach-s3c2410/sleep.o := \
  arch/arm/mach-s3c2410/sleep.S \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/linkage.h \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/assembler.h \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/smp.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/hwcap.h \
  arch/arm/mach-s3c2410/include/mach/hardware.h \
    $(wildcard include/config/cpu/s3c2440.h) \
    $(wildcard include/config/cpu/s3c2412.h) \
    $(wildcard include/config/no/multiword/io.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/sizes.h \
  arch/arm/mach-s3c2410/include/mach/map.h \
  arch/arm/plat-s3c/include/plat/map-base.h \
  arch/arm/plat-s3c24xx/include/plat/map.h \
    $(wildcard include/config/cpu/s3c2413.h) \
    $(wildcard include/config/cpu/s3c2412/only.h) \
  arch/arm/mach-s3c2410/include/mach/regs-gpio.h \
    $(wildcard include/config/cpu/s3c2400.h) \
  arch/arm/mach-s3c2410/include/mach/gpio-nrs.h \
    $(wildcard include/config/s3c/gpio/space.h) \
  arch/arm/mach-s3c2410/include/mach/regs-clock.h \
    $(wildcard include/config/cpu/s3c2442.h) \
  arch/arm/mach-s3c2410/include/mach/regs-mem.h \
  arch/arm/plat-s3c/include/plat/regs-serial.h \

arch/arm/mach-s3c2410/sleep.o: $(deps_arch/arm/mach-s3c2410/sleep.o)

$(deps_arch/arm/mach-s3c2410/sleep.o):
