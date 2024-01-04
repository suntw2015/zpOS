;内核入口(保护模式)
global _start
[BITS 32]
[extern main]
;c语言处理中断入口

_start:
    mov	ax,	cs
    ;ds 是0x000,要指向数据段，直接给cs会导致不可写入
    add ax, 0x08
	mov	ds,	ax
    mov es, ax
    mov ss, ax
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

[extern exception_handler]
;中断特殊处理，并不是所有的中断都会往栈里写入中断号，需要特殊处理一下 http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_handle
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push byte %1
    jmp isr_common_handle
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18

isr_common_handle:
    pusha
    mov ax, ds
    push eax
    call exception_handler
    pop eax
    popa
    add esp, 8
    sti
    iret
    
HelloKernel db 'Kernel.asm', 0