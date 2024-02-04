#include "page.h"
#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"
#include "heap.h"

static page_directory_entry *kernel_page_dir, *current_page_dir;

void init_page()
{
    current_page_dir = kernel_page_dir = (page_directory_entry*)malloc(sizeof(page_directory_entry)*PAGE_DIRECTORY_TABLE_SIZE, 1);
    if (kernel_page_dir == -1) {
        printsl("init page fail, no valid memory for kernel page");
        return;
    }

    page_table_entry *table = (page_table_entry*)malloc(sizeof(page_table_entry)*PAGE_SIZE, 1);
    if (table == -1) {
        printsl("init page fail, no valid memory for kernel page");
        return;
    }

    kernel_page_dir->address = (u32)table >> 12;
    kernel_page_dir->present = 1;
    kernel_page_dir->rw = 1;
    kernel_page_dir->accessed = 1;

    //如果从地址不是0开始映射，那么在cr0开启分页之后，下一行执行的代码地址，会发生映射变化，需要注意
    int i;
    int address = 0;
    for (i=0;i<PAGE_SIZE;i++, table++) {
        table->address = address>>12;
        table->present = 1;
        table->rw = 1;
        table->user = 1;
        address+= 0x1000;
    }
    
    page_directory_entry*dir = kernel_page_dir;
    for (dir++, i = 1; i < PAGE_DIRECTORY_TABLE_SIZE; i++, dir++)
    {
        dir->address = 0;
        dir->present = 0;
        dir->rw = 1;
    }

    //注册异常处理
    register_customer_interrupt_handle(INT_INDEX_PAGE_FAULT, page_interrupt_handle);

    print_page(kernel_page_dir);
    reload_page_directory(kernel_page_dir);
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

void print_page(page_directory_entry *dir)
{
    int i,j,t;
    char a[100];
    for (i=0;i<PAGE_DIRECTORY_TABLE_SIZE;i++,dir++) {
        if (dir->address == 0) {
            break;
        }

        print_number("dir %d", i);
        print_number(" address %x\n", dir);

        page_table_entry* table = (page_table_entry*)(dir->address << 12);
        for (j=0;j<PAGE_DIRECTORY_TABLE_SIZE;j++) {
            if (j == 10) {
                break;
            }
            print_number("    table %d", j);
            print_number(" address %x", table);
            print_number(" frame address %x\n", table->address << 12);
            table++;
        }
    }
}