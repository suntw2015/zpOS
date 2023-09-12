BOOT_SEC_COUNT=3
LD=ld
ASM=nasm
CC=gcc
CC_FLAG=-std=c99 -m32

# copyImg: bootKernel
	# scp root@10.26.15.86:/root/program/zpOS/bootKernel /Users/suntianwen/Desktop
	# dd if=/Users/suntianwen/Desktop/bootKernel of=/Users/suntianwen/Downloads/bochs-2.7/boot.img bs=512 count=20 conv=notrunc

bootKernel: boot.bin kernel
	touch bootKernel
	dd if=boot.bin of=bootKernel bs=512 count=$(BOOT_SEC_COUNT) conv=notrunc
	dd if=kernel of=bootKernel bs=512 count=10 seek=$(BOOT_SEC_COUNT) conv=notrunc

boot.bin: boot.asm
	$(ASM) $< -o $@

kernel: kernel.o main.o string.o console.o
	$(LD) $? -o $@ -Ttext 0x90000 -e _start -m elf_i386 --oformat binary

kernel.o: kernel.asm
	$(ASM) $< -f elf -o $@

main.o: main.c
	$(CC) -c $< -o $@ $(CC_FLAG)

string.o: string.c
	$(CC) -c $< -o $@ $(CC_FLAG)

console.o: console.c
	$(CC) -c $< -o $@ $(CC_FLAG)

clean:
	rm -f *.bin *.o kernel bootKernel