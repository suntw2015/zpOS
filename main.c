#include "global.h"
#include "string.h"
#include "console.h"

void init_gdt();
void init_gdtTable();
void initGdtTableEntry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity);
void print_gdt();

//gdt存储结构
gdt_table_entry gdtTable[3];
gdt_descriptor_t gdt;

int main() {
    clear_console();
    printsl("welcome to c language");
    char a[100];
    memset(a, 0, 100);
    ntos(a, 255, 16);
    printsl(a);

    printsl("start reload gdt");
    init_gdt();

    prints("base:");
    memset(a, 0, 100);
    ntos(a, gdt.base, 16);
    prints(a);
    prints(" ");
    prints("limit:");
    memset(a, 0, 100);
    ntos(a, gdt.limit, 10);
    printsl(a);

    prints(" ");
    memset(a, 0, 100);
    ntos(a, sizeof(gdt_table_entry), 10);
    printsl(a);

    printsl("reload gdt finish");
    print_gdt();

    while (1) {
    };

    return 0;
}

void init_gdt() {
    init_gdtTable();
    gdt.base = &gdtTable[0];
    gdt.limit = (sizeof(gdt_table_entry)*3)-1;
    reload_gdt(&gdt);
}
void init_gdtTable() {
    //empty
    initGdtTableEntry(&gdtTable[0], 0x00000000, 0x0000, 0x00, 0x00);
    //code
    initGdtTableEntry(&gdtTable[1], 0x00000000, 0xffff, 0b10011010, 0b11001111);
    //data
    initGdtTableEntry(&gdtTable[2], 0x00000000, 0xffff, 0b10010010, 0b11001111);
}
void initGdtTableEntry(gdt_table_entry* entry, u32 baseAddress, u16 limit, u8 access, u8 granularity) {
    entry->base_low = baseAddress & 0xffff;
    entry->base_mid = baseAddress>>16 & 0xff;
    entry->base_high = baseAddress>>24 & 0xff;
    entry->limit_low = limit;
    entry->access = access;
    entry->granularity = granularity;
}
void print_gdt() {
    char msg[100];
    int count = (gdt.limit+1)/sizeof(gdt_table_entry);

    printsl("===gdt info===");
    memset(msg,0,100);
    ntos(msg, count, 10);
    prints("gdt count:");
    printsl(msg);

    for(int i=0;i<count;i++) {
        prints("table ");
        memset(msg,0,100);
        ntos(msg, i, 10);
        prints(msg);
        printsl(":");

        prints("base address: 0x");
        memset(msg,0,100);
        ntos(msg, gdtTable[i].base_high<<16 | gdtTable[i].base_mid<<8 | gdtTable[i].base_low, 16);
        printsl(msg);
    }
}