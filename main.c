// #include "global.h"
// #include "string.h"
// #include "console.h"

int main() {


    // prints("Hello world");
    // prints("Zo OS\n");
    // prints("aaa");

    char* video = 0xB8000;
    video += 80*2*2;

    char content[] = "main.c";
    char c=0;
    for (int i=0;content[i] != '\0';i++) {
        *video = content[i];
        *(video+1) = 0x0f;
        video+=2;
    }

    while (1) {
    };

    return 0;
}