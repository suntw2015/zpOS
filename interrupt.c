#include "global.h"
#include "string.h"

//主片对饮端口为20h,21h,从片对应a0h,a1h
//初始化过程
//1.往20h/a0h写入icw1
//1.往21h/a1h写入icw2
//1.往21h/a1h写入icw3
//1.往21h/a1h写入icw4

#define MASTER_8295A_PORT1 0x20
#define MASTER_8295A_PORT2 0x21
#define SLAVE_8295A_PORT1 0xA0
#define SLAVE_8295A_PORT2 0xA1

void init_8259A() {
    ////icw1
	outb(MASTER_8295A_PORT1,	0x11);
	outb(SLAVE_8295A_PORT1,	0x11);
    //icw2
	outb(MASTER_8295A_PORT2,	0x20);
	outb(SLAVE_8295A_PORT2,	0x28);
    //icw3
	outb(MASTER_8295A_PORT2,	0x4);
	outb(SLAVE_8295A_PORT2,	0x2);
    //icw4
	outb(MASTER_8295A_PORT2,	0x1);
	outb(SLAVE_8295A_PORT2,	0x1);
    //ocw
	outb(MASTER_8295A_PORT2,	0xFF);
	outb(SLAVE_8295A_PORT2,	0xFF);
}

/**
 * @brief 初始化IDT
 * 
 * @param index 中断类型index
 * @param type 门类型
 * @param handler 处理方法
 * @param privilege 特权级别
 */
void initIDT(u8 index, u8 type, interruptHandler handler, u8 privilege) {
    u32 handlerAddr = (u32)handler;
    IDT* currentIdt = &idt[index];
    currentIdt->lowOffset = handlerAddr & 0xFFFF;
    currentIdt->highOffset = (handlerAddr >> 16) & 0xFFF;
    currentIdt->dcount = 0;
    currentIdt->attr = type | (privilege << 5);
    currentIdt->selector = 0x08; //loader里 code段是第二个,对应的0x08
}

void exceptionHandler (int index, int errorCode, int eip, int cs, int eflags) {
    char * err_msg[] = {"#DE Divide Error",
			    "#DB RESERVED",
			    "--  NMI Interrupt",
			    "#BP Breakpoint",
			    "#OF Overflow",
			    "#BR BOUND Range Exceeded",
			    "#UD Invalid Opcode (Undefined Opcode)",
			    "#NM Device Not Available (No Math Coprocessor)",
			    "#DF Double Fault",
			    "    Coprocessor Segment Overrun (reserved)",
			    "#TS Invalid TSS",
			    "#NP Segment Not Present",
			    "#SS Stack-Segment Fault",
			    "#GP General Protection",
			    "#PF Page Fault",
			    "--  (Intel reserved. Do not use.)",
			    "#MF x87 FPU Floating-Point Error (Math Fault)",
			    "#AC Alignment Check",
			    "#MC Machine Check",
			    "#XF SIMD Floating-Point Exception"};

    log("exception handler");
    log(err_msg[index]);
    log("");
    log("eflags:");
    logInt(eflags);
    log("cs:");
    logInt(cs);
    log("eip:");
    logInt(eip);

    if (errorCode != 0xFFFFFFFF) {
        log("errorCode:");
        log_int(errorCode);
    }
}

void init_interrupt() {
    init_8259A();
    initIDT(INT_INDEX_DIVIDE, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_DEBUG, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_NMI, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_BREAKPOINT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_USER);
    initIDT(INT_INDEX_OVERFLOW, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_USER);
    initIDT(INT_INDEX_BOUNDS, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_INVAL_OP, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_COPROC_NOT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_DOUBLE_FAULT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_COPROC_SEG, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_INVAL_TSS, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_SEG_NOT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_STACK_FAULT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_PROTECTION, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_PAGE_FAULT, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
    initIDT(INT_INDEX_COPROC_ERR, GATE_TYPE_INTERRUPT, exceptionHandler, PRIVILEGE_KRNL);
}

