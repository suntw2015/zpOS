;内核入口(保护模式)
global _start
[BITS 32]
[extern main]

_start:
        ; 将字符串的字符逐个输出
        mov dx, helloKernel  ; 字符串地址
        mov bx, 0xB8000  ; 显存地址

print_loop:
        mov al, dl
        mov ah, 0x0F
        mov [bx], ax
        add bx, 2
        add dx, 2
        test al, al  ; 检查AL寄存器的值是否为0，即字符串是否结束
        jz print_end  ; 如果是0，跳转到调用main函数的标签
        jmp print_loop  ; 继续下一个字符的打印
print_end:
	call main
	jmp $

helloKernel: db 'Start Kenel',0
