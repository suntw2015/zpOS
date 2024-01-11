#ifndef TIME_H
#define TIME_H
#include "global.h"

#define PORT_CONTROL 0x43
#define PORT_CHANNEL_0 0x40
#define PORT_CHANNEL_1 0x41
#define PORT_CHANNEL_2 0x42

void init_time(u32 frequency);

#endif