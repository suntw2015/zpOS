#include "memory.h"
#include "console.h"

//内核代码段结束地址，在ld文件中定义
extern u32 end_addr;
extern u32 start_addr;

void init_memory() {
    memory.kernel_start_addr = (u32)&start_addr;
    memory.kernel_code_end_addr = (u32)&end_addr;
    memory.total_end_addr = 1024*1024*16;

    //堆栈
    memory.kernel_heap_start = memory.kernel_heap_current = memory.kernel_code_end_addr;
    memory.kernel_heap_end = memory.kernel_heap_start + HEAP_SIZE;

    memory.kernel_end_addr = memory.kernel_heap_end;
}

void* malloc(u32 size, u8 align) {
    if (align) {
        memory.kernel_heap_current += 4096 - memory.kernel_heap_current % 4096;
    }
    if (memory.kernel_heap_current + size > memory.kernel_heap_end) {
        return NULL;
    }
    memory.kernel_heap_current += size;
    return (void*)(memory.kernel_heap_current-size);
}

u32 get_kernel_end_addr() {
    return memory.kernel_end_addr;
}

void print_memory() {
    print_number("kernel start: %x\n", memory.kernel_start_addr);
    print_number("kernel code end: %x\n", memory.kernel_code_end_addr);
    print_number("kernel end: %x\n", memory.kernel_end_addr);
    print_number("heap start: %x\n", memory.kernel_heap_start);
    print_number("heap current: %x\n", memory.kernel_heap_current);
    print_number("heap end: %x\n", memory.kernel_heap_end);
}