#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"
#include "time.h"
#include "keyboard.h"
#include "page.h"

void init_gdt();
void init_gdt_table();
void init_gdt_table_entry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity);
void print_gdt();
void init_keyborad();

//gdt存储结构
gdt_table_entry gdt_table[GDT_TABLE_COUNT];
gdt_descriptor_t gdt;

int main() {
    clear_console();
    printsl("welcome to c language");

    init_gdt();
    init_idt();
    init_time(50);
    // //一定要开始中断，进入保护模式前，cli禁用了所有的中断
    sti();

    // 触发中断，测试中断处理函数
    // __asm__ volatile ("int $0x00"); // 触发 Divide Error 异常
    // __asm__ volatile ("int $0x01"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x10"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x20"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x20"); // 触发 Debug Exception

    //-----------键盘-------------
    init_keyborad();

    //-----------分页-----------
    printsl("init page");
    init_page();
    u32 *ptr = (u32*)0x10000000;
    u32 do_page_fault = *ptr;
    printsl("init page finish");

    while (1) {
    };

    return 0;
}

void init_gdt() {
    init_gdt_table();
    gdt.base = (u32)gdt_table;
    gdt.limit = (sizeof(gdt_table_entry)*3)-1;
    reload_gdt(&gdt);
}
void init_gdt_table() {
    //empty
    init_gdt_table_entry(&gdt_table[0], 0x00000000, 0x0000, 0x00, 0x00);
    //code
    init_gdt_table_entry(&gdt_table[1], 0x00000000, 0xffff, 0b10011010, 0b11001111);
    //data
    init_gdt_table_entry(&gdt_table[2], 0x00000000, 0xffff, 0b10010010, 0b11001111);
}
void init_gdt_table_entry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity) {
    entry->base_low = baseAddress & 0xffff;
    entry->base_mid = baseAddress>>16 & 0xff;
    entry->base_high = baseAddress>>24 & 0xff;
    entry->limit_low = limit;
    entry->access = access;
    entry->granularity = granularity;
}
void print_gdt() {
    char msg[100];
    int count = (gdt.limit+1)/sizeof(gdt_table_entry);

    printsl("===gdt info===");
    memset(msg,0,100);
    ntos(msg, count, 10);
    prints("gdt count:");
    printsl(msg);

    for(int i=0;i<count;i++) {
        prints("table ");
        memset(msg,0,100);
        ntos(msg, i, 10);
        prints(msg);
        printsl(":");

        prints("base address: 0x");
        memset(msg,0,100);
        ntos(msg, gdt_table[i].base_high<<16 | gdt_table[i].base_mid<<8 | gdt_table[i].base_low, 16);
        printsl(msg);
    }
}

void init_keyborad() {
    register_customer_interrupt_handle(INT_INDEX_CUSTOMER33, keybord_interrupt_handle);
}