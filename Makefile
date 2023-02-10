BOOT_SEC_COUNT=3
LD=ld
ASM=nasm
CC=gcc
CC-FLAG=-std=c99 -m32

bootKernel: boot.bin kernel
	touch bootKernel
	dd if=boot.bin of=bootKernel bs=512 count=$(BOOT_SEC_COUNT) conv=notrunc
	dd if=kernel of=bootKernel bs=512 count=10 seek=$(BOOT_SEC_COUNT) conv=notrunc

# copyImg : boot.img
# 	dd if=boot.bin of=/Users/suntianwen/Downloads/bochs-2.7/boot.img bs=512 count=1 conv=notrunc
# 	dd if=kernel.bin of=/Users/suntianwen/Downloads/bochs-2.7/boot.img bs=512 count=10 seek=1 conv=notrunc

boot.img : boot.bin kernel

boot.bin : boot.asm
	$(ASM) $< -o $@
kernel :kernel.o main.o string.o
	$(LD) $? -o  $@ -Ttext 0x10000 -e _start -m elf_i386  --oformat binary
kernel.o : kernel.asm
	$(ASM) $? -f elf -o $@
main.o : main.c
	$(CC) -c $? -o $@ $(CC-FLAG)
string.o : string.c
	$(CC) -c $? -o $@ $(CC-FLAG)
interrupt.o : interrupt.c
	$(CC) -c $? -o $@ $(CC-FLAG)
clean :
	rm *.bin *.o kernel bootKernel
