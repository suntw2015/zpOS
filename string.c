#include "string.h"
#include "global.h"

int strlen(const char *s)
{
    int len=0;
    while (s[len] != '\0') {
        len++;
    }

    return len;
}

void memset(void* s, char a, int len)
{
    int i=0;
    char *str = (char*)s;
    while (i<len) {
        str[i++] = a;
    }
}

void strcat(char *s1, char* s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    for (int i=0;i<len2;i++) {
        s1[len1++] = s2[i];
    }

    s1[len1] = '\0';
}

/**
 * @brief 数值转成字符串
 * 
 * @param s 存放的字符串
 * @param value 
 */
void ntos(char*s, long value, u8 radix)
{
    char numbers[] = "0123456789ABCDEF";
    int t,start=0,end=0;
    if (value<0) {
        s[0]='-';
        value = value*-1;
        start=end=1;
    }
    while(value) {
        t=value%radix;
        s[end++]=numbers[t];
        value/=radix;
    }

    char c;
    for(end--;start<end;start++,end--) {
        c = s[start];
        s[start] = s[end];
        s[end] = c;
    }
}

int strcmp(const char *s1, const char *s2) 
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    return strncmp(s1, s2, len1>len2 ? len1 : len2);
}

int strncmp(const char *s1, const char *s2, int count)
{
    for (int i=0;i<count;i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

int strpos(const char *target, const char*searh)
{
    int lenTarget = strlen(target);
    int lenSearch = strlen(searh);
    if (lenTarget<lenSearch) {
        return -1;
    }

    for(int i=0,j=0;i<lenTarget;i++) {
        if (target[i] != searh[j]) {
            j=0;
            continue;
        }
        j++;
        if (j==lenSearch) {
            return i-j+1;
        }
    }

    return -1;
}