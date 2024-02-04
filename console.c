#include "console.h"
#include "global.h"
#include "string.h"

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
    u8  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    u16 attribute = attributeByte << 8;
    u16 *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x)
    {
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_address + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
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

void printsl(char *s)
{
    for (int i=0;s[i]!='\0';i++) {
        printc(s[i]);
    }
    printc('\n');
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
    u16 blank = 0x20 /* space */ | (attributeByte << 8);

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

/**
 * 输出带有转义的字符串
 * %d 10进制整数
 * %x 16进制整数
*/
void print_number(char *s, u32 number) {
    char a[20] = {0};
    for (int i=0;s[i]!='\0';i++) {
        if (s[i] == '%') {
            if (s[i+1] == 'd') {
                ntos(a, number, 10);
                prints(a);
                i++;
                continue;
            } else if (s[i+1] == 'x') {
                prints("0x:");
                ntos(a, number, 16);
                prints(a);
                i++;
                continue;
            } else {
                printc(s[i]);    
            }
        } else {
            printc(s[i]);
        }
    }
}