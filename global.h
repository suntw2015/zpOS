#ifndef GLOBAL_H
#define GLOBAL_H

#define GDT_COUNT 255
#define IDT_COUNT 255

/* 中断向量 */
#define	INT_INDEX_DIVIDE		0x0 //非法除数
#define	INT_INDEX_DEBUG		0x1 //
#define	INT_INDEX_NMI			0x2
#define	INT_INDEX_BREAKPOINT		0x3
#define	INT_INDEX_OVERFLOW		0x4
#define	INT_INDEX_BOUNDS		0x5
#define	INT_INDEX_INVAL_OP		0x6
#define	INT_INDEX_COPROC_NOT		0x7
#define	INT_INDEX_DOUBLE_FAULT		0x8
#define	INT_INDEX_COPROC_SEG		0x9
#define	INT_INDEX_INVAL_TSS		0xA
#define	INT_INDEX_SEG_NOT		0xB
#define	INT_INDEX_STACK_FAULT		0xC
#define	INT_INDEX_PROTECTION		0xD
#define	INT_INDEX_PAGE_FAULT		0xE
#define	INT_INDEX_COPROC_ERR		0x10

//调用门
#define GATE_TYPE_CALL 0x8C
//中断门
#define GATE_TYPE_INTERRUPT 0x8E
//调用门
#define GATE_TYPE_TSS 0x89
//陷阱们
#define GATE_TYPE_TRAFE 0x8F

/* 权限 */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3


//定义基础数据类型
typedef __signed__ char s8;
typedef unsigned char u8;
typedef __signed__ short s16;
typedef unsigned short u16;
typedef __signed__ int s32;
typedef unsigned int u32;
typedef __signed__ long long s64;
typedef unsigned long long u64;

typedef struct __packed
{
	u64 addr;
	u64 length;
	u32 type;
} MemoryArds;

typedef struct KernelConfig
{
	int screenMaxRow;	//屏幕最大行
	int screenMaxCol;	//屏幕最大列
    int cursorCurrentRow; //光标目前行
    int cursorCurrentCol; //光标目前列
} KernelConfig;

extern KernelConfig kernelConfig;

//IDT结构
typedef struct {
    u16 lowOffset; //低16位
    u16 selector; //段选择子
    u8 dcount;
    u8 attr; //p(1) DPL(2) DT(1) TYPE(4)
    u16 highOffset; //高16位
} IDT;
extern IDT idt[IDT_COUNT];

//GDT结构
typedef struct
{
	u16	lowLimit;		/* Limit */
	u16	lowBase;		/* Base */
	u8	midBase;		/* Base */
	u8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
	u8	limitHighAttr2;	/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
	u8	highBase;		/* Base */
}GDT;
extern  GDT gdt[GDT_COUNT];

typedef	void (*interruptHandler) ();

/**
 * @brief 向某个端口输出一个字节
 * 
 * @param value 输出的值
 * @param port 输出端口
 */
static inline void outb(u8 value, u16 port)
{
    __asm__ volatile("outb %0,%1" : : "a" (value), "dN" (port));
}

static inline u8 inb(u16 port)
{
	u8 value;
	__asm__ volatile("inb %1,%0" : "=a" (value) : "dN" (port));
	return value;
}

#endif