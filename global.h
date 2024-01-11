#ifndef GLOBAL_H
#define GLOBAL_H

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

#define GDT_TABLE_COUNT 3

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

//IDT结构
typedef struct {
    u16 offset_low;    // 中断处理函数地址低16位
    u16 selector;      // 中断处理函数代码段选择子
    u8 zero;           // 保留位，必须为0
    u8 type_attr;      // 中断描述符属性
    u16 offset_high;   // 中断处理函数地址高16位
} __attribute__((packed)) idt_table_entry;

//IDT描述符结构
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_descriptor_t;

//GDT描述符结构
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) gdt_descriptor_t;

//GDT结构
typedef struct
{
	u16 limit_low;     // GDT表界限低16位
    u16 base_low;      // GDT表基地址低16位
    u8 base_mid;       // GDT表基地址中间8位
    u8 access;         // GDT描述符访问标志
    u8 granularity;    // GDT描述符粒度标志
    u8 base_high;      // GDT表基地址高8位
}__attribute__((packed)) gdt_table_entry;

typedef	void (*interruptHandler) ();

/**
 * @brief 向某个端口输出一个字节
 * 
 * @param port 输出端口
 * @param value 输出的值
 */
static inline void outb(u16 port, u8 value)
{
    __asm__ volatile("outb %0,%1" : : "a" (value), "dN" (port));
}

/**
 * @brief 从某个端口读取一个字节
 * 
 * @param port 
 * @return u8 
 */
static inline u8 inb(u16 port)
{
	u8 value;
	__asm__ volatile("inb %1,%0" : "=a" (value) : "dN" (port));
	return value;
}

/**
 * @brief 从某个端口读取两个字节
 * 
 * @param port 
 * @return u16 
 */
static inline u16 inw(u16 port)
{
    u16 value;
	__asm__ volatile("inw %1,%0" : "=a" (value) : "dN" (port));
	return value;
}

static inline void reload_gdt(gdt_descriptor_t* descriptor)
{
	__asm__ volatile (
        "lgdt (%0)"
        :
        : "r"(descriptor)
    );
}

static inline void reload_idt(idt_descriptor_t* descriptor)
{
	__asm__ volatile (
        "lidt (%0)"
        :
        : "r"(descriptor)
    );
}

static inline void iret()
{
	__asm__ volatile(
		"iret"
	);
}

static inline void sti()
{
    __asm__ volatile ("sti");
}

#endif