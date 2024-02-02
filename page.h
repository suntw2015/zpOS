#ifndef PAGE_H
#define PAGE_H
#include "global.h"
#include "interrupt.h"

#define PAGE_DIRECTORY_TABLE_SIZE 1024
#define PAGE_SIZE 4096
//页表属性位
#define PAGE_TABLE_ATTR_P 0
#define PAGE_TABLE_ATTR_RW 1
#define PAGE_TABLE_ATTR_US 2
#define PAGE_TABLE_ATTR_PWT 3
#define PAGE_TABLE_ATTR_PCD 4
#define PAGE_TABLE_ATTR_A 5
#define PAGE_TABLE_ATTR_D 6
#define PAGE_TABLE_ATTR_PAT 7
#define PAGE_TABLE_ATTR_G 8
#define PAGE_TABLE_ATTR_AVL 9
//页表属性位
#define PAGE_DIRECTORY_ATTR_P 0
#define PAGE_DIRECTORY_ATTR_RW 1
#define PAGE_DIRECTORY_ATTR_US 2
#define PAGE_DIRECTORY_ATTR_PWT 3
#define PAGE_DIRECTORY_ATTR_PCD 4
#define PAGE_DIRECTORY_ATTR_A 5
#define PAGE_DIRECTORY_ATTR_AVL_ONE 6
#define PAGE_DIRECTORY_ATTR_PS 7
#define PAGE_DIRECTORY_ATTR_AVL_TWO 8

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

/**
 * 页错误中断
*/
void page_interrupt_handle(interrupt_info info);

/**
 * 加载页目录
*/
void reload_page_directory(page_directory_entry* dir);
void print_page();
void init_page();

#endif