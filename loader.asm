; ==========================================
; pmtest2.asm
; 编译方法：nasm pmtest2.asm -o pmtest2.com
; ==========================================

%include	"pm.inc"	; 常量, 宏, 以及一些说明

org	10000h
	jmp	LABEL_BEGIN

LABEL_GDT:		dd	0,0
LABEL_DESC_CODE32:
	dw 0xFFFF		;segment limit first 0-15 bits
	dw 0x0000		;base first 0-15 bits
	db 0x00			;base 16-23 bits
	db 0b10011010	;access byte
	db 0b11001111	;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00			;base 24-31 bits
LABEL_DESC_DATA32:
	dw 0xFFFF		;segment limit first 0-15 bits
	dw 0x0000		;base first 0-15 bits
	db 0x00			;base 16-23 bits
	db 0b10010010	;access byte
	db 0b11001111	;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00			;base 24-31 bits
LABEL_DESC_VIDEO:
	dw 0xFFFF		;segment limit first 0-15 bits
	dw 0x0000		;base first 0-15 bits
	db 0x00			;base 16-23 bits
	db 0b10010010			;access byte
	db 0b11001111	;high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
	db 0x00			;base 24-31 bits

LABEL_GDT_END:

GdtPtr	dw	LABEL_GDT_END - LABEL_GDT - 1
	dd	LABEL_GDT

SelectorCode	equ	LABEL_DESC_CODE32 - LABEL_GDT
SelectorData	equ	LABEL_DESC_DATA32 - LABEL_GDT

SelectorVideo	equ LABEL_DESC_VIDEO - LABEL_GDT

[BITS 16]
LABEL_BEGIN:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov ax, 0x00
	mov	ss, ax
	mov	sp, 0x9000

	;打印日志
	mov	ax,	1301h
	mov	bx,	000fh
	mov	dx,	0200h		;row 2
	mov	cx,	12
	mov	bp,	StartLoaderMessage
	int	10h

	cli
	lgdt [GdtPtr]

	mov	eax,	cr0
	or	eax,	1
	mov	cr0,	eax

	jmp SelectorCode:LABEL_SEG_CODE32

	;=======	open address A20
	; in	al,	92h
	; or	al,	00000010b
	; out	92h,	al
	
	; 真正进入保护模式
	; jmp	dword SelectorCode32:0	; 执行这一句会把 SelectorCode32 装入 cs,
	; 				; 并跳转到 Code32Selector:0  处

[SECTION .s32]; 32 位代码段. 由实模式跳入.
[BITS	32]

LABEL_SEG_CODE32:
	mov	ax,	1301h
	mov	bx,	000fh
	mov	dx,	0600h		;row 2
	mov	cx,	12
	mov	bp,	StartLoaderMessage2
	int	10h

	mov	ax, SelectorVideo
	mov	gs, ax			; 视频段选择子(目的)

	mov	edi, (80 * 11 + 79) * 2	; 屏幕第 11 行, 第 79 列。
	mov	ah, 0Ch			; 0000: 黑底    1100: 红字
	mov	al, 'P'
	mov	[gs:edi], ax

	; 到此停止
	jmp	$

;=======	display messages

StartLoaderMessage:	db	"Start Loader"
StartLoaderMessage2:	db	"Start Loader2"