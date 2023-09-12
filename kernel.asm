;内核入口(保护模式)
global _start
[BITS 32]

_start:
        jmp entry
[extern main]
entry:
        ; 将字符串的字符逐个输出
        mov esi, helloKernel  ; 字符串地址
        mov edi, 0xB8000  ; 显存地址

print_loop:
        lodsb  ; 从esi指向的内存位置读取一个字节到AL寄存器，并将esi递增
        test al, al  ; 检查AL寄存器的值是否为0，即字符串是否结束
        jz print_end  ; 如果是0，跳转到调用main函数的标签

        ; 将字符和显示属性写入显存
        stosw  ; 将AX寄存器的内容写入di指向的内存位置，并将di递增
        jmp print_loop  ; 继续下一个字符的打印
print_end:
	call main
	jmp $

helloKernel: db 'Start Kenel',0
