/***
 * 参考内容
 * http://www.osdever.net/bkerndev/Docs/irqs.htm
 * http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
*/
#ifndef INTERRUPT_H
#define INTERRUPT_H
#include "global.h"

#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_COMMAND 0xA0
#define SLAVE_DATA 0xA1

#define IDT_TABLE_COUNT 256

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
#define INT_INDEX_UNKNOW_EXCEPTION  0xF
#define	INT_INDEX_COPROC_ERR		0x10
#define INT_INDEX_ALIGNMENT_CHECK_EXCEPTION 0x11
#define INT_INDEX_MACHINE_CHECK_EXCEPTION 0x12
//自定义中断
#define INT_INDEX_CUSTOMER32 32
#define INT_INDEX_CUSTOMER33 33
#define INT_INDEX_CUSTOMER34 34
#define INT_INDEX_CUSTOMER35 35
#define INT_INDEX_CUSTOMER36 36
#define INT_INDEX_CUSTOMER37 37
#define INT_INDEX_CUSTOMER38 38
#define INT_INDEX_CUSTOMER39 39
#define INT_INDEX_CUSTOMER40 40
#define INT_INDEX_CUSTOMER41 41
#define INT_INDEX_CUSTOMER42 42
#define INT_INDEX_CUSTOMER43 43
#define INT_INDEX_CUSTOMER44 44
#define INT_INDEX_CUSTOMER45 45
#define INT_INDEX_CUSTOMER46 46
#define INT_INDEX_CUSTOMER47 47

typedef struct {
    u32 data_segment; //数据段选择子
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32 int_no;     //中断号
    u32 err_code;   //中断错误码
    u32 eip, cs, eflags, useresp, ss; //中断发生后，cpu自动处理的
}__attribute__((packed)) interrupt_info;

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

//自定义中断处理
typedef void (*customer_interrupt_handle)(interrupt_info);
customer_interrupt_handle customer_interrupt_handle_table[IDT_TABLE_COUNT];

void init_idt();
void init_idt_table();
void exception_handler (interrupt_info info);
void init_idt_table_entry(u8 index, u8 type, interruptHandler handler, u8 privilege);
void init_8259A();
void register_customer_interrupt_handle(u8 index, customer_interrupt_handle handle);

#endif