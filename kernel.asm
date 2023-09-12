;内核入口(保护模式)
global _start
[BITS 32]
_start:
        jmp entry

extern main
entry:
	call main
	jmp $
