#include "page.h"
#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"
#include "memory.h"
#include "task.h"

void init_page()
{
    current_page_dir = kernel_page_dir = (page_directory_entry*)malloc(sizeof(page_directory_entry)*PAGE_DIRECTORY_TABLE_SIZE, 1);
    if (kernel_page_dir == NULL) {
        printsl("init page fail, no valid memory for kernel page");
        return;
    }

    page_table_entry *table = (page_table_entry*)malloc(sizeof(page_table_entry)*PAGE_SIZE, 1);
    if (table == NULL) {
        printsl("init page fail, no valid memory for kernel page");
        return;
    }
    memset(table, 0, sizeof(page_table_entry)*PAGE_SIZE);

    print_memory();

    //初始化物理帧使用情况
    memset(frame_use_map, 0, sizeof(frame_use_map));

    kernel_page_dir->address = (u32)table >> 12;
    kernel_page_dir->present = 1;
    kernel_page_dir->rw = 1;
    kernel_page_dir->user = 0;

    //把0x0到内核用到的内存映射关系，初始化到页表中
    int i,idx;
    //内核使用的帧数 todo 需要加1;
    int frame_count = get_kernel_end_addr() / 4096;
    
    for (i=0; i<frame_count;i++) {
        init_page_table(table);
        table++;
    }
    
    page_directory_entry*dir = kernel_page_dir;
    for (dir++, i = 1; i < PAGE_DIRECTORY_TABLE_SIZE; i++, dir++)
    {
        dir->address = 0;
        dir->present = 1;
        dir->rw = 1;
        dir->user = 0;
    }

    //注册异常处理
    register_customer_interrupt_handle(INT_INDEX_PAGE_FAULT, page_interrupt_handle);
    reload_page_directory(kernel_page_dir);
}

/**
 * 页错误中断
*/
void page_interrupt_handle(interrupt_info* info)
{
    // The faulting address is stored in the CR2 register.
    u32 faul_address;
    __asm__ volatile("mov %%cr2, %0" : "=r" (faul_address));

    static int t=0;
    if (t<2) {
        print_number("pid: %d page fault: ", get_pid());
        // prints("page fault: ");
        if (info->int_code & 0x1) {
            prints("present");
        } else if (info->int_code & 0x2) {
            prints("read only");
        } else if (info->int_code & 0x4) {
            prints("user model");
        } else if (info->int_code & 0x8) {
            prints("reserved");
        } else if (info->int_code & 0x10) {
            prints("read only");
        }
        print_number(" at %x\n", faul_address);
        t++;
    }
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
            continue;
        }

        print_number("dir %d", i);
        print_number(" address %x\n", (u32)dir);

        page_table_entry* table = (page_table_entry*)(dir->address << 12);
        for (j=0;j<PAGE_DIRECTORY_TABLE_SIZE;j++,table++) {
            if (table->address == 0) {
                continue;
            }
            print_number("    table %d", j);
            print_number(" address %x", (u32)table);
            print_number(" frame address %x\n", table->address << 12);
        }
    }
}

/**
 * 查找第一个可用的帧地址
*/
u32 find_valid_frame() {
    int i,j,index=-1;
    for (i=0;i<FRAME_MAX_SIZE;i++) {
        //依次判断8个帧
        if (frame_use_map[i] == 0xFF) {
            continue;
        }

        for (j=0;j<8;j++) {
            if (!(frame_use_map[i] & (1<<j))) {
                index = i*8+j;
                goto find;
            }
        }
    }

find:
    if (index != -1) {
        set_frame(index, 1);
        return index*4096;
    }
    return -1;
}

/**
 * 设置帧使用情况
*/
void set_frame(u32 index, u8 use) {
    u8 pos = index%8;
    if (use) {
        frame_use_map[index/8] |= 1 << pos;   
    } else {
        frame_use_map[index/8] &= ~(1 << pos);
    }
}

page_directory_entry* clone_page_dir(page_directory_entry* src) {
    page_directory_entry* dir = (page_directory_entry*)malloc(sizeof(page_directory_entry)*PAGE_DIRECTORY_TABLE_SIZE, 1);
    if (kernel_page_dir == NULL) {
        printsl("clone page fail, no valid memory for kernel page");
        return NULL;
    }

    memset(dir, 0, sizeof(page_directory_entry)*PAGE_DIRECTORY_TABLE_SIZE);

    page_table_entry* table;
    page_directory_entry* tmp = dir;

    for (int i=0;i<PAGE_SIZE;i++) {
        if (src->address == 0) {
            continue;
        }

        //内核区域不用拷贝，公用的
        if ((kernel_page_dir+i)->address == src->address) {
            memcpy(src, tmp, sizeof(page_table_entry));
        } else {
            table = (page_table_entry*)malloc(sizeof(page_table_entry), 1);
            memset(table, 0, sizeof(page_table_entry));
            init_page_table(table);
            //拷贝物理帧
            memcpy((void*)(src->address >> 12), (void*)(table->address >> 12), 4096);
            //绑定目录和页
            tmp->address = (u32)table >> 12;
            tmp->rw = 1;
            tmp->present = 1;
            tmp->user = 0;
        }
        src++;
        tmp++;
    }

    return dir;
}

void init_page_table(page_table_entry* table) {
    if (table->address) {
        return;
    }

    int idx = find_valid_frame();
    table->address = idx >> 12;
    table->present = 1;
    table->rw = 1;
    table->user = 0;
}

void alloc_frame(u32 address, page_directory_entry* dir) {
    address = address >> 12;
    page_table_entry *table = (dir->address << 12) + address % 1024;
    if (table->address == 0) {
        init_page_table(table);
    }
}