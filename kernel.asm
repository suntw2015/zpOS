;内核入口(保护模式)
global _start
[BITS 32]
_start:
        jmp entry

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

extern main
entry:
	mov ax, SelectorData
	mov ds,ax
	mov es,ax
    	mov	ax, SelectorVideo
	mov	gs, ax			; 视频段选择子(目的)

	mov	edi, (80 * 11 + 10) * 2	; 屏幕第 11 行, 第 10 列。
	mov	ah, 0Ch			; 0000: 黑底    1100: 红字
	mov	al, 'P'
	mov	[gs:edi], ax

	; 到此停止
	call main
	jmp $
