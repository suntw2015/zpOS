#include "heap.h"
#include "global.h"
#include "string.h"
#include "console.h"

static heap_entry heap;
static char heap_memory[HEAP_SIZE];

void* malloc(u32 size, u8 align) {
    if (align) {
        heap.current += 4096 - heap.current % 4096;
    }
    if (heap.current + size > heap.end) {
        return (void*)-1;
    }
    heap.current += size;
    return (void*)(heap.current-size);
}

void init_heap() {
    heap.start = heap.current = (u32)heap_memory;
    heap.end = heap.start+HEAP_SIZE;
}

void print_heap() {
    print_number("heap start: %x", heap.start);
    print_number(" current: %x", heap.current);
    print_number(" end: %x\n", heap.end);
}