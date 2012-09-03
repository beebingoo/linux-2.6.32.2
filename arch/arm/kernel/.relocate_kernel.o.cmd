cmd_arch/arm/kernel/relocate_kernel.o := arm-linux-gcc -Wp,-MD,arch/arm/kernel/.relocate_kernel.o.d  -nostdinc -isystem /usr/local/arm/4.3.2/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -Iinclude  -I/home/xfile/myProject/linux-2.6.32.2/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-s3c2410/include -Iarch/arm/mach-s3c2400/include -Iarch/arm/mach-s3c2412/include -Iarch/arm/mach-s3c2440/include -Iarch/arm/mach-s3c2442/include -Iarch/arm/mach-s3c2443/include -Iarch/arm/plat-s3c24xx/include -Iarch/arm/plat-s3c/include -D__ASSEMBLY__ -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables  -D__LINUX_ARM_ARCH__=4 -march=armv4t -mtune=arm9tdmi -include asm/unified.h -msoft-float -gdwarf-2       -c -o arch/arm/kernel/relocate_kernel.o arch/arm/kernel/relocate_kernel.S

deps_arch/arm/kernel/relocate_kernel.o := \
  arch/arm/kernel/relocate_kernel.S \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/kexec.h \
    $(wildcard include/config/kexec.h) \

arch/arm/kernel/relocate_kernel.o: $(deps_arch/arm/kernel/relocate_kernel.o)

$(deps_arch/arm/kernel/relocate_kernel.o):
