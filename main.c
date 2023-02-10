#include "global.h"
#include "string.h"

KernelConfig kernelConfig;

void init_console();
void init_heap();
void init_kernel();
void init_memery();
void init_video();
void log(char *s);

int main() {
    char *video = (char*)0xB8000;

    *video = 'S';

    kernelConfig.cursorCurrentCol = 0;
    kernelConfig.cursorCurrentRow = 0;
    kernelConfig.screenMaxCol = 80;
    kernelConfig.screenMaxRow = 50;

    init_memery();
    init_interrrupt();

    int a =1/0;

    while (1) {
    };

    return 0;
}

void init_interrrupt()
{
    
}

void init_console()
{}

void init_heap()
{}

void init_kernel()
{}

void init_memery()
{
    char a[100];
    char b[100];
    u8 count = *((u8*)0x7e00);
    long long t;

    memset(a, 0, 100);
    strcat(a, "rds count:");
    itoa(count, b, 10);
    strcat(a,b);
    log(a);

    MemoryArds *ards = 0x7e01;
    for (int i=0;i<count;i++) {
        ards = 0x7e01 + i*20;
        memset(a, 0, 100);
        strcat(a, "base:");
        itoa(ards->addr, b, 16);
        strcat(a, b);
        strcat(a, ", length:");
        itoa(ards->length, b , 16);
        strcat(a, b);
        strcat(a, ", type:");
        itoa(ards->type, b , 10);
        strcat(a, b);
        log(a);
    }
}