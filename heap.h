#ifndef HEAP_H
#define HEAP_H

#include "global.h"
//堆大小1MB
#define HEAP_SIZE 0x100000

typedef struct {
    u32 start;
    u32 current;
    u32 end;
} heap_entry;

/**
 * 申请内存
 * size 内存大小
 * align 是否4字节对齐
*/
void* malloc(u32 size, u8 align);

void init_heap();

void print_heap();

#endif