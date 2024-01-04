#include "global.h"
#include "string.h"
#include "interrupt.h"
#include "console.h"

idt_table_entry idt_table[IDT_TABLE_COUNT];
idt_descriptor_t idt;

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();

//主片对饮端口为20h,21h,从片对应a0h,a1h
//初始化过程
//1.往20h/a0h写入icw1
//1.往21h/a1h写入icw2
//1.往21h/a1h写入icw3
//1.往21h/a1h写入icw4

void init_8259A() {
    ////icw1
	outb(MASTER_8295A_PORT1,	0x11);
	outb(SLAVE_8295A_PORT1,	0x11);
    //icw2
	outb(MASTER_8295A_PORT2,	0x20);
	outb(SLAVE_8295A_PORT2,	0x28);
    //icw3
	outb(MASTER_8295A_PORT2,	0x4);
	outb(SLAVE_8295A_PORT2,	0x2);
    //icw4
	outb(MASTER_8295A_PORT2,	0x1);
	outb(SLAVE_8295A_PORT2,	0x1);
    //ocw
	outb(MASTER_8295A_PORT2,	0xFF);
	outb(SLAVE_8295A_PORT2,	0xFF);
}

void init_idt()
{
    init_idt_table();
    idt.base = (u32)idt_table;
    idt.limit = sizeof(idt_table_entry)*IDT_TABLE_COUNT-1;
    reload_idt(&idt);
}

/**
 * @brief 初始化IDTtable
 * 
 * @param index 中断类型index
 * @param type 门类型
 * @param handler 处理方法
 * @param privilege 特权级别
 */
void init_idt_table_entry(u8 index, u8 type, interruptHandler handler, u8 privilege) {
    u32 handlerAddr = (u32)handler;
    idt_table[index].offset_low = handlerAddr & 0xFFFF;
    idt_table[index].offset_high = (handlerAddr >> 16) & 0xFFF;
    idt_table[index].zero = 0;
    idt_table[index].type_attr = type | (privilege << 5);
    idt_table[index].selector = 0x08; //loader里 code段是第二个,对应的0x08
}

void exception_handler (interrupt_info info) {
    printsl("----exception handle----");
    prints("no:0x");
    char a[100];
    memset(a,0,100);
    ntos(a, info.int_no, 16);
    prints(a);
    prints(" code:0x");
    memset(a,0,100);
    ntos(a, info.err_code, 16);
    prints(a);
    printc('\n');
}

void init_idt_table() {
    init_8259A();
    init_idt_table_entry(INT_INDEX_DIVIDE, GATE_TYPE_INTERRUPT, isr0, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_DEBUG, GATE_TYPE_INTERRUPT, isr1, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_NMI, GATE_TYPE_INTERRUPT, isr2, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_BREAKPOINT, GATE_TYPE_INTERRUPT, isr3, PRIVILEGE_USER);
    init_idt_table_entry(INT_INDEX_OVERFLOW, GATE_TYPE_INTERRUPT, isr4, PRIVILEGE_USER);
    init_idt_table_entry(INT_INDEX_BOUNDS, GATE_TYPE_INTERRUPT, isr5, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_INVAL_OP, GATE_TYPE_INTERRUPT, isr6, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_COPROC_NOT, GATE_TYPE_INTERRUPT, isr7, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_DOUBLE_FAULT, GATE_TYPE_INTERRUPT, isr8, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_COPROC_SEG, GATE_TYPE_INTERRUPT, isr9, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_INVAL_TSS, GATE_TYPE_INTERRUPT, isr10, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_SEG_NOT, GATE_TYPE_INTERRUPT, isr11, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_STACK_FAULT, GATE_TYPE_INTERRUPT, isr12, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_PROTECTION, GATE_TYPE_INTERRUPT, isr13, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_PAGE_FAULT, GATE_TYPE_INTERRUPT, isr14, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_UNKNOW_EXCEPTION, GATE_TYPE_INTERRUPT, isr15, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_COPROC_ERR, GATE_TYPE_INTERRUPT, isr16, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_ALIGNMENT_CHECK_EXCEPTION, GATE_TYPE_INTERRUPT, isr17, PRIVILEGE_KRNL);
    init_idt_table_entry(INT_INDEX_MACHINE_CHECK_EXCEPTION, GATE_TYPE_INTERRUPT, isr18, PRIVILEGE_KRNL);
}