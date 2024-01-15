#include "time.h"
#include "interrupt.h"
#include "console.h"
#include "string.h"

u32 tick = 0;

void timer_callback(interrupt_info info)
{
    tick++;
    // prints("tick: ");
    // char a[50];
    // memset(a, 0, 50);
    // ntos(a, tick, 10);
    // printsl(a);
}

void init_time(u32 frequency)
{
   // Firstly, register our timer callback.
   register_customer_interrupt_handle(INT_INDEX_CUSTOMER32, (customer_interrupt_handle)timer_callback);

   //PIT时钟产生中断，1193180 Hz
   u16 divisor = 1193180 / frequency;

   // Send the command byte.
   outb(PORT_CONTROL, 0x36);

   // Send the frequency divisor.
   outb(PORT_CHANNEL_0, divisor & 0xFF);
   outb(PORT_CHANNEL_0, (divisor>>8) & 0xFF);

   printsl("---init time-----");
}