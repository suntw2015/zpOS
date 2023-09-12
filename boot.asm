; boot.asm
org	0x7c00
bootOtherSecCount equ 2
;内核基础地址
kernelBase equ 0x1000
;内核目标偏移地址
kernelOffset equ 0x000
;内核大小
kernelSecCount equ 10
jmp Label_Start

;GDT define
LABEL_GDT:              dd      0,0
LABEL_DESC_CODE32:
        dw 0xFFFF               ;segment limit first 0-15 bits
        dw 0x0000               ;base first 0-15 bits
        db 0x00                 ;base 16-23 bits
        db 0b10011010   ;access byte
        db 0b11001111   ;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
        db 0x00                 ;base 24-31 bits
LABEL_DESC_DATA32:
        dw 0xFFFF               ;segment limit first 0-15 bits
        dw 0x0000               ;base first 0-15 bits
        db 0x00                 ;base 16-23 bits
        db 0b10010010   ;access byte
        db 0b11001111   ;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
        db 0x00                 ;base 24-31 bits
LABEL_DESC_VIDEO:
        dw 0xFFFF               ;segment limit first 0-15 bits
        dw 0x8000               ;base first 0-15 bits
        db 0x0B                 ;base 16-23 bits
        db 0b10010010                   ;access byte
        db 0b11001111   ;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
        db 0x00                 ;base 24-31 bits

LABEL_GDT_END:

GdtPtr  dw      LABEL_GDT_END - LABEL_GDT - 1
        dd      LABEL_GDT

SelectorCode    equ     LABEL_DESC_CODE32 - LABEL_GDT
SelectorData    equ     LABEL_DESC_DATA32 - LABEL_GDT

SelectorVideo   equ LABEL_DESC_VIDEO - LABEL_GDT

Label_Start:
	mov	ax,	cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax

;=======	clear screen
	mov	ax,	0600h
	mov	bx,	0700h
	mov	cx,	 0 ;ch起始行数 cl起始列数
	mov	dx,	184fh ;dh 结束行数 dl结束列数
	int	10h
;======= print message
	mov ax, 0x1301
	mov bx, 0x000f
	mov cx, 10 ;长度
	mov dx, 0x0000 ;dh行 dl列
	mov bp, StartBootMessage
	int 10h

;======= 加载其他的boot功能
	mov ah, 2
	mov al, bootOtherSecCount
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, 0

	;数据存放es:bx
	mov bx, 0x7e00
	int 13h

;======= load kernel & jump
; int13h ah=02 读扇区 参数如下 https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=02h:_Read_Sectors_From_Drive  http://c.biancheng.net/view/3606.html
; al=扇区数 ch=柱面 cl=扇区 dh=磁头 dl=驱动器
; 出参 cf=0成功 ah=00, al=传输的扇区数 ah=其他，错误
	mov ah, 2
	mov al, kernelSecCount
	mov ch, 0
	mov cl, 2
	add cl, bootOtherSecCount
	mov dh, 0
	mov dl, 0

	;数据存放es:bx
	mov bx, kernelBase
	mov es, bx
	mov bx, kernelOffset
	int 13h

	;直接进到内核
	jmp 0x10000

;======= 干些其他的事情
	call Func_Boot_Entry

;======= 进入保护模式

	cli
	lgdt [GdtPtr]
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp dword SelectorCode:0x10000

;=======	display messages
StartBootMessage:	db	"Start boot"
DetectMemortMessage: db	"detect memory"

;=======	fill zero until whole sector

	times	510 - ($ - $$)	db	0
	dw	0xaa55

;======= second sector, do some bois function
MemoryLayout:
	db 0 ;ards cout
MemoryLayoutAddr:
	times 256 db 0 ; ards detail

Screen:
	db 0 ;width
	db 0 ;height

Func_Boot_Entry:
	mov ax, cs
	mov es, ax
	mov ax, 0x1301
	mov bx, 0x000f
	mov cx, 13 ;长度
	mov dx, 0x0100 ;dh行 dl列
	mov bp, DetectMemortMessage
	int 10h

	call Func_Detect_Memory
	ret

;======= detect memory e820 https://blog.csdn.net/weixin_42707324/article/details/108306596
;es:di写入ards
;
Func_Detect_Memory:
	mov ax, cs
	mov es, ax
	mov bx, 0
	mov cx, MemoryLayoutAddr
	mov di, cx

.loop:
	mov eax, 0xe820
	mov ecx, 20
	mov edx, 0x0534D4150
	; write ards to es:di
	int 15h
	jc MemoryDetectFail
	add di, 20
	mov ax, [MemoryLayout]
	inc ax
	mov [MemoryLayout], ax
	cmp ebx, 0
	jne .loop
	jmp MemoryDetectSuccess

MemoryDetectFail:
	mov byte [MemoryLayout], 0
MemoryDetectSuccess:
	ret

Func_Detect_Screen:
	