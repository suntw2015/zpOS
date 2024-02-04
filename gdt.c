#include "gdt.h"
#include "global.h"

//gdt存储结构
static gdt_table_entry gdt_table[GDT_TABLE_COUNT];
static gdt_descriptor_t gdt;

void init_gdt() {
    init_gdt_table();
    gdt.base = (u32)gdt_table;
    gdt.limit = (sizeof(gdt_table_entry)*3)-1;
    reload_gdt((u32)&gdt);
}
void init_gdt_table() {
    //empty
    init_gdt_table_entry(&gdt_table[0], 0x00000000, 0x0000, 0x00, 0x00);
    //code
    init_gdt_table_entry(&gdt_table[1], 0x00000000, 0xffff, 0b10011010, 0b11001111);
    //data
    init_gdt_table_entry(&gdt_table[2], 0x00000000, 0xffff, 0b10010010, 0b11001111);
}
void init_gdt_table_entry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity) {
    entry->base_low = baseAddress & 0xffff;
    entry->base_mid = baseAddress>>16 & 0xff;
    entry->base_high = baseAddress>>24 & 0xff;
    entry->limit_low = limit;
    entry->access = access;
    entry->granularity = granularity;
}