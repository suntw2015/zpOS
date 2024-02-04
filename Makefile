BOOT_SEC_COUNT=1
LD=ld
ASM=nasm
CC=gcc
CC_FLAG=-std=c99 -m32 -fno-builtin
KERNEL_LD=kernel.ld

# copyImg: bootKernel
	# scp root@10.26.15.86:/root/program/zpOS/bootKernel /Users/suntianwen/Desktop
	# dd if=/Users/suntianwen/Desktop/bootKernel of=/Users/suntianwen/Downloads/bochs-2.7/boot.img bs=512 count=20 conv=notrunc

bootKernel: boot.bin kernel
	touch bootKernel
	dd if=boot.bin of=bootKernel bs=512 count=$(BOOT_SEC_COUNT) conv=notrunc
	dd if=kernel of=bootKernel bs=512 count=30 seek=$(BOOT_SEC_COUNT) conv=notrunc

boot.bin: boot.asm
	$(ASM) $< -o $@

kernel: kernel.o main.o string.o console.o interrupt.o time.o keyboard.o page.o heap.o
	$(LD) -m elf_i386 -T $(KERNEL_LD) --oformat=binary $? -o $@

kernel.o: kernel.asm
	$(ASM) $< -f elf -o $@

main.o: main.c
	$(CC) $(CC_FLAG) -c $< -o $@

string.o: string.c
	$(CC) $(CC_FLAG) -c $< -o $@

console.o: console.c
	$(CC) $(CC_FLAG) -c $< -o $@

interrupt.o: interrupt.c
	$(CC) $(CC_FLAG) -c $< -o $@
time.o: time.c
	$(CC) $(CC_FLAG) -c $< -o $@
keyboard.o: keyboard.c
	$(CC) $(CC_FLAG) -c $< -o $@
page.o: page.c
	$(CC) $(CC_FLAG) -c $< -o $@
heap.o: heap.c
	$(CC) $(CC_FLAG) -c $< -o $@
clean:
	rm -f *.bin *.o kernel bootKernel