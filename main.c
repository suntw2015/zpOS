#include "global.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"
#include "time.h"
#include "keyboard.h"
#include "page.h"
#include "heap.h"
#include "gdt.h"

int main() {
    clear_console();
    printsl("welcome to c language");

    init_gdt();
    init_idt();
    init_time(50);
    // //一定要开始中断，进入保护模式前，cli禁用了所有的中断
    sti();

    // 触发中断，测试中断处理函数
    // __asm__ volatile ("int $0x00"); // 触发 Divide Error 异常
    // __asm__ volatile ("int $0x01"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x10"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x20"); // 触发 Debug Exception
    // __asm__ volatile ("int $0x20"); // 触发 Debug Exception

    //-----------键盘-------------
    init_keyborad();
    init_heap();
    
    //-----------分页-----------
    printsl("init page");
    init_page();

    while (1) {
    };

    return 0;
}