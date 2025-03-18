#ifndef MEMORY_H
#define MEMORY_H

#include "global.h"

//堆大小1MB
#define HEAP_SIZE 0x100000

/**
 * 内存分布为
 * 内核代码 ... 内核堆栈 .... 内核结束
 */
typedef struct {
    u32 kernel_start_addr;      //内核内存开始地址
    u32 kernel_code_end_addr;   //内核内存代码结束地址
    u32 kernel_heap_start;      //内核内存堆栈开始地址
    u32 kernel_heap_current;    //内核内存堆栈当前地址
    u32 kernel_heap_end;        //内核内存堆栈结束地址
    u32 kernel_end_addr;        //内核总结束地址
    u32 total_end_addr;         //全部内存结束地址
} memory_entry;

memory_entry memory;

/**
 * 申请内存
 * size 内存大小
 * align 是否4字节对齐
*/
void* malloc(u32 size, u8 align);
void print_memory();
void init_memory();
u32 get_kernel_end_addr();

#endif