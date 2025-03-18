#ifndef PAGE_H
#define PAGE_H
#include "global.h"
#include "interrupt.h"

#define PAGE_DIRECTORY_TABLE_SIZE 1024
#define PAGE_SIZE 1024
//没有实现内存分配之前，先固定下来最大进程的数量，用于确定页面的地址
#define THREAD_MAX_SIZE 20
//暂定一个目录只使用一个页 THREAD_MAX_SIZE * PAGE_DIRECTORY_TABLE_SIZE
#define PAGE_DIRECTORY_MAX_SIZE 20
#define PAGE_TABLE_MAX_SIZE 20480

//帧的最大值，内存暂定16MB
#define FRAME_MAX_SIZE 4096

// 页地址转为虚拟地址
#define page2virual(page) ((unsigned long)((page) << 12))

/**
 * 参考 https://wiki.osdev.org/Paging
 * 
 * 二级(64位多级)表结构 VS 一级结构
 * 
 * 虚拟地址是连续的，32位最大内存是4G，一个表项为4Byte
 * 一级结构始终需要1MB*4连续空间
 * 二级结构最大需要1024*1024*4，最小需要1024*1*4
 * 一个程序不可能用到全部的4GB空间，二级结构中第二级是弹性的，在小内存的机器中，优势很大
 * 
 * 页表构成：
 *    0: P
 *    1: r/w
 *    2: u/s
 *    3: pwt
 *    4: pcd
 *    5: a
 *    6: d
 *    7: pat
 *    8: g
 *    9-11: avl 
 *    12-31: address
 * 页目录构成
 *    0: P
 *    1: r/w
 *    2: u/s
 *    3: pwt
 *    4: pcd
 *    5: a
 *    6: avl
 *    7: ps
 *    8-11:avl
 *    12-31: address
*/

//页表
typedef struct
{
   u32 present    : 1;   // Page present in memory
   u32 rw         : 1;   // Read-only if clear, readwrite if set
   u32 user       : 1;   // Supervisor level only if clear
   u32 pwt        : 1;   // write through
   u32 pcd        : 1;   // cache dis
   u32 accessed   : 1;   // Has the page been accessed since last refresh?
   u32 dirty      : 1;   // Has the page been written to since last refresh?
   u32 pat        : 1;   // page attribute
   u32 g          : 1;   // page attribute
   u32 unused     : 3;   // Amalgamation of unused and reserved bits
   u32 address    : 20;
} page_table_entry;

//页目录
typedef struct
{
   u32 present    : 1;   // Page present in memory
   u32 rw         : 1;   // Read-only if clear, readwrite if set
   u32 user       : 1;   // Supervisor level only if clear
   u32 pwt        : 1;   // write through
   u32 pcd        : 1;   // cache dis
   u32 accessed   : 1;   // Has the page been accessed since last refresh?
   u32 unused     : 6;   // Amalgamation of unused and reserved bits
   u32 address    : 20;
} page_directory_entry;

//物理内存帧使用情况
//假设物理内存为16MB, 一帧是4KB, 需要需要4096字节存储，使用比特位的话需要 512字节即可
static char frame_use_map[512];

/**
 * 页错误中断
*/
void page_interrupt_handle(interrupt_info* info);

void set_frame(u32 index, u8 use);
u32 find_valid_frame();

/**
 * 加载页目录
*/
void reload_page_directory(page_directory_entry* dir);
void init_page();
void init_page_table(page_table_entry* table);
void print_page(page_directory_entry *dir);

void alloc_frame(u32 address, page_directory_entry* dir);

page_directory_entry* clone_page_dir(page_directory_entry* src);

page_directory_entry *kernel_page_dir, *current_page_dir;

#endif