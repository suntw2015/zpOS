#ifndef INTERRUPT_H
#define INTERRUPT_H

#define MASTER_8295A_PORT1 0x20
#define MASTER_8295A_PORT2 0x21
#define SLAVE_8295A_PORT1 0xA0
#define SLAVE_8295A_PORT2 0xA1

void init_interrupt();
#endif