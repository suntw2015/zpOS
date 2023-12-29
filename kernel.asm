;内核入口(保护模式)
global _start
[BITS 32]
[extern main]

_start:
    mov	ax,	cs
    ;ds 是0x000,要指向数据段，直接给cs会导致不可写入
    add ax, 0x08
	mov	ds,	ax
    ; 将字符串的字符逐个输出
    mov edx, HelloKernel ; 字符串地址
    mov edi, 0xB8000    ; 显存地址
    ;在第二行，屏幕25*80
    add edi, 160

print_loop:
    mov eax, [edx]       ; 读取字符串中的一个字符
    mov ah, 0x0F        ; 字符属性
    mov word [edi], ax  ; 将字符和属性存储到显存中
    add edi, 2          ; 递增显存地址
    add edx, 1          ; 递增字符串地址
    test al, al         ; 检查AL寄存器的值是否为0，即字符串是否结束
    jz print_end        ; 如果是0，跳转到调用main函数的标签
    jmp print_loop      ; 继续下一个字符的打印

print_end:
    call main
    jmp $

HelloKernel db 'Kernel.asm', 0