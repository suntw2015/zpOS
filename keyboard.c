#include "keyboard.h"
#include "console.h"
#include "global.h"
#include "string.h"

void keybord_interrupt_handle(interrupt_info info)
{
    char a[100];
    memset(a,0,100);

    u8 scan_code = inb(KEYBOARD_READ_PORT);
    // 最高位为1的话，表示松开按键
    if (scan_code & 0x80)
    {
        
    }
    else
    {
        printc(scan_codes[scan_code]);
    }
}

void init_keyborad() {
    register_customer_interrupt_handle(INT_INDEX_CUSTOMER33, keybord_interrupt_handle);
}