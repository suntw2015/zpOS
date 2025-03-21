#ifndef STRING_H
#define STRING_H
#include "global.h"

void memset(void* s, char a, int len);
void strcat(char *s1, char* s2);
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int count);
int strpos(const char *target, const char*searh);
/**
 * @brief 数值转成字符串
 * 
 * @param s 存放的字符串
 * @param value 
 */
void ntos(char*s, u32 value, u8 radix);

void memcpy(const void* src, void* dst, u32 size);

#endif