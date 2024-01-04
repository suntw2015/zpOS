#ifndef INTERRUPT_H
#define INTERRUPT_H

#define MASTER_8295A_PORT1 0x20
#define MASTER_8295A_PORT2 0x21
#define SLAVE_8295A_PORT1 0xA0
#define SLAVE_8295A_PORT2 0xA1

#define IDT_TABLE_COUNT 256

typedef struct {
    u32 data_segment; //数据段选择子
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32 int_no;     //中断号
    u32 err_code;   //中断错误码
    u32 eip, cs, eflags, useresp, ss; //中断发生后，cpu自动处理的
}__attribute__((packed)) interrupt_info;

void init_idt();
void init_idt_table();
void exception_handler (interrupt_info info);
void init_idt_table_entry(u8 index, u8 type, interruptHandler handler, u8 privilege);
void init_8259A();

#endif