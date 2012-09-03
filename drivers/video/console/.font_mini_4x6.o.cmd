cmd_drivers/video/console/font_mini_4x6.o := arm-linux-gcc -Wp,-MD,drivers/video/console/.font_mini_4x6.o.d  -nostdinc -isystem /usr/local/arm/4.3.2/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -Iinclude  -I/home/xfile/myProject/linux-2.6.32.2/arch/arm/include -include include/linux/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-s3c2410/include -Iarch/arm/mach-s3c2400/include -Iarch/arm/mach-s3c2412/include -Iarch/arm/mach-s3c2440/include -Iarch/arm/mach-s3c2442/include -Iarch/arm/mach-s3c2443/include -Iarch/arm/plat-s3c24xx/include -Iarch/arm/plat-s3c/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=4 -march=armv4t -mtune=arm9tdmi -msoft-float -Uarm -fno-stack-protector -fomit-frame-pointer -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(font_mini_4x6)"  -D"KBUILD_MODNAME=KBUILD_STR(font)"  -c -o drivers/video/console/font_mini_4x6.o drivers/video/console/font_mini_4x6.c

deps_drivers/video/console/font_mini_4x6.o := \
  drivers/video/console/font_mini_4x6.c \
  include/linux/font.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/xfile/myProject/linux-2.6.32.2/arch/arm/include/asm/posix_types.h \

drivers/video/console/font_mini_4x6.o: $(deps_drivers/video/console/font_mini_4x6.o)

$(deps_drivers/video/console/font_mini_4x6.o):
