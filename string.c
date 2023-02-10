#include "string.h"
#include "global.h"

int strlen(char *s)
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
        *(str++) = a;
        i++;
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

void itoa(long long num,char* str,int radix)
{
    char index[]="0123456789ABCDEF";
    unsigned unum;/*中间变量*/
    int i=0,j,k;
    /*确定unum的值*/
    if(radix==10&&num<0)/*十进制负数*/
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num;
    /*转换*/
    do{
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
       }while(unum);
    str[i]='\0';
    /*逆序*/
    if(str[0]=='-')
        k=1;/*十进制负数*/
    else
        k=0;
     
    for(j=k;j<=(i-1)/2;j++)
    {       char temp;
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
}

void log(char *s)
{
    char *video = (char*)0xB8000 + (kernelConfig.cursorCurrentRow*kernelConfig.screenMaxCol+kernelConfig.cursorCurrentCol)*2;
    
    for (int i=0; s[i] != '\0'; i++) {
        *video = s[i];
        video += 2;
    }
    kernelConfig.cursorCurrentRow++;
}

void logInt(int v) {
    char tmp[30];
    memset(tmp, 0, 30);
    int i=0;
    while (v) {
        tmp[i++] = v%10;
        v /=10;
    }

    char t;
    for (int j=0;j<i;j++,i--) {
        t=tmp[i];
        tmp[i]=tmp[j];
        tmp[j]=t;
    }
    log(tmp);
}