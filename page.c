#include "page.h"
#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"

//物理内存页数
u32 physics_page_count;
//物理内存使用情况
u32* physics_page_map;

//一定要4kb对齐
static __attribute__((aligned(4096))) page_directory_entry page_dir[PAGE_DIRECTORY_TABLE_SIZE];
static __attribute__((aligned(4096))) page_table_entry page_table[PAGE_DIRECTORY_TABLE_SIZE];

static u32* current_page_dir;

void init_page()
{
    int i;
    //如果从地址不是0开始映射，那么在cr0开启分页之后，下一行执行的代码地址，会发生映射变化，需要注意
    int address = 0;
    for (i=0;i<PAGE_DIRECTORY_TABLE_SIZE;i++) {
        page_table[i].address = address>>12;
        page_table[i].present = 1;
        page_table[i].rw = 1;
        page_table[i].user = 1;
        address+= 0x1000;
    }

    page_dir[0].address = (u32)&page_table[0] >> 12;
    page_dir[0].present = 1;
    page_dir[0].rw = 1;
    page_dir[0].accessed = 1;
    
    for (i = 1; i < PAGE_DIRECTORY_TABLE_SIZE; i++)
    {
        page_dir[i].address = 0;
        page_dir[i].present = 0;
        page_dir[i].rw = 1;
    }

    //注册异常处理
    register_customer_interrupt_handle(INT_INDEX_PAGE_FAULT, page_interrupt_handle);

    print_page();
    reload_page_directory(&page_dir[0]);
}

/**
 * 页错误中断
*/
void page_interrupt_handle(interrupt_info info)
{
    // The faulting address is stored in the CR2 register.
    u32 faul_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faul_address));

    int present = !(info.err_code & 0x1); // Page not present
    int rw = info.err_code & 0x2;           // Write operation?
    int us = info.err_code & 0x4;           // Processor was in user-mode?
    int reserved = info.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = info.err_code & 0x10;          // Caused by an instruction fetch?

    prints("page fault: ");
    if (info.err_code & 0x1) {
        prints("present");
    } else if (info.err_code & 0x2) {
        prints("read only");
    } else if (info.err_code & 0x4) {
        prints("user model");
    } else if (info.err_code & 0x8) {
        prints("reserved");
    } else if (info.err_code & 0x10) {
        prints("read only");
    }
    prints(" ar 0x");
    char a[20];
    memset(a,0,20);
    ntos(a, faul_address, 16);
    printsl(a);
}

/**
 * 加载页目录
*/
void reload_page_directory(page_directory_entry* dir)
{
    __asm__ volatile("mov %0, %%cr3":: "r"(dir));
    u32 cr0;
    __asm__ volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    __asm__ volatile("mov %0, %%cr0":: "r"(cr0));
}

void print_page()
{
    int i,j,t;
    char a[100];
    for (i=0;i<PAGE_DIRECTORY_TABLE_SIZE;i++) {
        if (page_dir[i].address == 0) {
            break;
        }
        memset(a,0,100);
        prints("dir ");
        ntos(a, i, 10);
        prints(a);
        prints(" address: 0x");
        memset(a,0,100);
        ntos(a, &page_dir[i], 16);
        printsl(a);

        page_table_entry* table = (page_table_entry*)(page_dir[i].address << 12);

        for (j=0;j<PAGE_DIRECTORY_TABLE_SIZE;j++) {
            if (j == 10) {
                break;
            }
            memset(a,0,100);
            prints("    table ");
            ntos(a, j, 10);
            prints(a);

            prints(" address: 0x");
            memset(a,0,100);
            ntos(a, (u32)table, 16);
            prints(a);

            prints(" frame address : 0x");
            memset(a,0,100);
            ntos(a, table->address << 12, 16);
            printsl(a);
            table++;
        }
    }
}