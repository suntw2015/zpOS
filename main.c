#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"
#include "time.h"
#include "keyboard.h"
#include "page.h"
#include "gdt.h"
#include "memory.h"
#include "task.h"

void first_process_entry();
void second_process_entry();

int main(u32 stack) {
    clear_console();
    printsl("welcome to c language");
    kernel_stack = stack;
    print_number("start stack %x\n", stack);

    init_gdt();
    init_idt();
    init_time(50);
    // //一定要开始中断，进入保护模式前，cli禁用了所有的中断
    sti();    

    //-----------键盘-------------
    init_keyborad();
    init_memory();
    init_page();

    //-----------多进程---------
    printsl("init multi task");
    create_kernel_task(first_process_entry);
    create_kernel_task(second_process_entry);
    while(1) {};

    return 0;
}

void first_process_entry() {
    int i=0;
    while (1) {
        i++;
        if (i == 100) {
            printsl("first process");
            i=0;
        }
    }
}

void second_process_entry() {
    int i=0;
    while (1) {
        i++;
        if (i == 100) {
            printsl("second process");
            i=0;
        }
    }
}