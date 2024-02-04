#ifndef CONSOLE_H
#define CONSOLE_H
#include "global.h"

void printc(char c);

void prints(char *s);

void printsl(char *s);

void print_number(char *s, u32 number);

void clear_console();

void move_cursor();

void scroll_console();
#endif