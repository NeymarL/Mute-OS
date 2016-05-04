
# 生成img镜像
default:
	nasm ipl.nas -o ipl.bin
	nasm liuheOS.nas -o liuheOS.sys
	cat ipl.bin liuheOS.sys > liuheOS.img

# 写入到u盘
usb:
	dd if=liuheOS.img of=/dev/disk1