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

    ;初始化堆栈
    mov eax, 0xFFFF
    mov ebp, eax
    mov esp, eax
    
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
    push esp
    call main
    jmp $

; c语言调用此方法采用call指令，相当于两个指令 push eip,jmp xxx。返回时ret相当于两个指令pop eip, add esp, xx
; 在此我们不直接使用ret，就可以得到eip的值了，放在eax中，eax此时代表两个含义，一个是eip的值，一个是调用方的返回值
[GLOBAL read_eip]
read_eip:
  pop eax
  push eax
  ret

[GLOBAL perform_task_switch]
perform_task_switch:
     cli;
     mov ecx, [esp+4]   ; EIP
     mov eax, [esp+8]   ; physical address of current directory
     mov ebp, [esp+12]  ; EBP
     mov esp, [esp+16]  ; ESP
     mov cr3, eax       ; set the page directory
     sti;
     jmp ecx

[extern exception_handler]
;中断特殊处理，并不是所有的中断都会往栈里写入中断号，需要特殊处理一下 http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
;主动push error_code后，需要把error_code pop出来 https://stackoverflow.com/questions/491613/do-i-have-to-pop-the-error-code-pushed-to-stack-by-certain-exceptions-before-ret
%macro ISR_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL isr%1]        ; %1 accesses the first parameter.
  isr%1:
    cli
    push 0
    push %1
    pusha
    jmp isr_common_handle
%endmacro

%macro ISR_ERRCODE 1
  [GLOBAL isr%1]
  isr%1:
    cli
    push %1
    pusha
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

;自定义
ISR_NOERRCODE 33
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47

; 32单独写一下，涉及到任务切换
[GLOBAL isr32]        ;
isr32:
    cli
    push 0
    push 32
    push eax;
    push ecx;
    push edx;
    push ebx;
    push esi;
    push edi;

    mov ax, [TaskCount]
    inc ax
    mov [TaskCount], ax
    cmp ax, 2
    jz debug
    call exception_handler ;返回下一个任务的esp, 放在eax里
    mov esp, eax

    jmp $

    pop edi;
    pop esi;
    pop ebx;
    pop edx;
    pop ecx;
    pop eax;
    
    add esp,8
    sti
    iret

debug:
    jmp $

isr_common_handle:
    call exception_handler
    popa
    add esp, 8
    sti
    iret
    
HelloKernel db 'Kernel.asm', 0

TaskCount db 0