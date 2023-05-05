#include "global.h"
#include "string.h"

KernelConfig kernelConfig;
idt_table_entry idt_table[IDT_COUNT];
idt_descriptor_t idt_descriptor;
gdt_table_entry gdt_table[GDT_COUNT];
gdt_descriptor_t gdt_descriptor;

void init_console();
void init_heap();
void init_kernel();
void init_memery();
void init_video();
void log(char *s);
void init_gdt();
void init_idt();
void divide_error_handler();
void page_fault_handler();


int main() {
    char *video = (char*)0xB8000;

    *video = 'S';

    kernelConfig.cursorCurrentCol = 0;
    kernelConfig.cursorCurrentRow = 0;
    kernelConfig.screenMaxCol = 80;
    kernelConfig.screenMaxRow = 50;

    log("init gdt");
    init_gdt();
    log("init idt");
    init_idt();
    log("init idt finish");
    init_memery();

    log("div");
    int a =1/0;

    while (1) {
    };

    return 0;
}

void init_gdt()
{
    //空占位
    memset(&gdt_table[0], 0, 8);

    // 初始化代码段选择子
    gdt_table[1].limit_low = 0xffff;
    gdt_table[1].base_low = 0x0000;
    gdt_table[1].base_mid = 0x00;
    gdt_table[1].access = 0x9a;
    gdt_table[1].granularity = 0xcf;
    gdt_table[1].base_high = 0x00;

    // 初始化数据段选择子
    gdt_table[2].limit_low = 0xffff;
    gdt_table[2].base_low = 0x0000;
    gdt_table[2].base_mid = 0x00;
    gdt_table[2].access = 0x92;
    gdt_table[2].granularity = 0xcf;
    gdt_table[2].base_high = 0x00;

    // 初始化GDT表界限和基地址
    gdt_table[3].limit_low = sizeof(gdt_table) - 1;
    gdt_table[3].base_low = (u32)&gdt_table;
    gdt_table[3].base_mid = ((u32)&gdt_table) >> 16;
    gdt_table[3].access = 0x80;
    gdt_table[3].granularity = 0x00;
    gdt_table[3].base_high = ((u32)&gdt_table) >> 24;

   //初始化gdt描述符
    gdt_descriptor.limit = sizeof(gdt_table)-1;
    gdt_descriptor.base = (u32)&gdt_table;

    // 装载GDT表
    reload_gdt(&gdt_descriptor);
}

// 定义中断处理函数
void divide_error_handler() {
    log("divide_error_handler");
    iret();
}

void page_fault_handler() {
    // 处理页面错误异常
    log("page_fault_handler");
    iret();
}

void init_idt()
{
    // 初始化除0异常中断描述符
    idt_table[0].offset_low = (u32)divide_error_handler & 0xffff;
    idt_table[0].selector = 0x08;   // 代码段选择子
    idt_table[0].zero = 0;
    idt_table[0].type_attr = 0x8e;  // 中断类型为14（除0异常），特权级为0

    // 初始化页面错误异常中断描述符
    idt_table[14].offset_low = (u32)page_fault_handler & 0xffff;
    idt_table[14].selector = 0x08;  // 代码段选择子
    idt_table[14].zero = 0;
    idt_table[14].type_attr = 0x8e; // 中断类型为14（页面错误异常），特权级为0

    // 初始化IDT表界限和基地址
    idt_descriptor.limit = sizeof(idt_table) - 1;
    idt_descriptor.base = (u32)&idt_table;
    reload_idt(&idt_descriptor);
}

void init_console()
{}

void init_heap()
{}

void init_kernel()
{}

void init_memery()
{
    char a[100];
    char b[100];
    u8 count = *((u8*)0x7e00);
    long long t;

    memset(a, 0, 100);
    strcat(a, "rds count:");
    itoa(count, b, 10);
    strcat(a,b);
    log(a);

    MemoryArds *ards = 0x7e01;
    for (int i=0;i<count;i++) {
        ards = 0x7e01 + i*20;
        memset(a, 0, 100);
        strcat(a, "base:");
        itoa(ards->addr, b, 16);
        strcat(a, b);
        strcat(a, ", length:");
        itoa(ards->length, b , 16);
        strcat(a, b);
        strcat(a, ", type:");
        itoa(ards->type, b , 10);
        strcat(a, b);
        log(a);
    }
}