cmd_arch/arm/boot/uImage := /bin/bash /home/xfile/myProject/linux-2.6.32.2/scripts/mkuboot.sh -A arm -O linux -T kernel -C none -a 0x30008000 -e 0x30008000 -n 'Linux-2.6.32.2' -d arch/arm/boot/zImage arch/arm/boot/uImage