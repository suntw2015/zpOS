#include "console.h"
#include "global.h"

u16 *video_address = (u16*)0xB8000;
u8 cursor_x = 0;
u8 cursor_y = 0;
u8 screen_width = 80;
u8 screen_height = 25;

//空白定义
u16 blank = 0x0f20;

void printc(char c)
{
    // 背景色黑色，文字白色
    // 颜色枚举0:black, 1:blue, 2:green, 3:cyan, 4:red, 5:magenta, 6:brown, 7:light grey, 8:dark grey, 9:light blue, 10:light green, 11:light cyan, 12:light red, 13:light magneta, 14: light brown, 15: white
    u8 backColour = 0;
    u8 foreColour = 15;

    //2个字节表示一个字符，0-7 ascll码值，8-11前景色，12-15背景色
    u16 charAttr = (backColour << 4 | foreColour) | c;

    if (c == '\n') {
        //换行
        cursor_x++;
        cursor_y=0;
    } else if (c == 0x7f) {
        //删除
        if (cursor_x==0 && cursor_y==0) {
            return;
        }
        //当前位置用空白字符
        *(video_address+cursor_x*80+cursor_y) = blank;
        //设置坐标
        if (cursor_y > 0) {
            cursor_y--;
        } else {
            cursor_y = screen_width-1;
            if (cursor_x) {
                cursor_x--;
            }
        }
    } else if (c>=0x20 && c <0x7f) {
        //可打印字符
        *(video_address+cursor_x*80+cursor_y) = charAttr;
    }

    scroll_console();
    move_cursor();
}

void prints(char *s)
{
    for (int i=0;s[i]!='\0';i++) {
        printc(s[i]);
    }
}

void clear_console()
{
    cursor_x = 0;
    cursor_y = 0;
    for (int i=0;i<screen_height;i++) {
        for (int j = 0; j < screen_width; j++)
        {
            video_address[i*screen_width+j] = blank;
        }
    }
    move_cursor();
}

void move_cursor()
{
    u16 location = cursor_y*screen_width+cursor_x;
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, location >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, location);      // Send the low cursor byte.
}

void scroll_console()
{
    // Get a space character with the default colour attributes.
    u8 attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= screen_height)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*screen_width; i < screen_height*screen_width; i++)
        {
            video_address[i] = video_address[i+screen_width];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = (screen_height-1)*screen_width; i < screen_height*screen_width; i++)
        {
            video_address[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = screen_height-1;
    }
}