#ifndef GDT_H
#define GDT_H

#include "global.h"

//GDT描述符结构
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) gdt_descriptor_t;

//GDT结构
typedef struct
{
	u16 limit_low;     // GDT表界限低16位
    u16 base_low;      // GDT表基地址低16位
    u8 base_mid;       // GDT表基地址中间8位
    u8 access;         // GDT描述符访问标志
    u8 granularity;    // GDT描述符粒度标志
    u8 base_high;      // GDT表基地址高8位
}__attribute__((packed)) gdt_table_entry;

void init_gdt();
void init_gdt_table();
void init_gdt_table_entry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity);

#endif