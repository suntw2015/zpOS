#include "task.h"
#include "global.h"
#include "page.h"
#include "memory.h"
#include "console.h"
#include "string.h"
#include "interrupt.h"

//任务列表
task_entry *task = NULL;
//当前任务
task_entry *current_task = NULL;
//进程pid
u32 pid = 1;

extern page_directory_entry *kernel_page_dir;

//获取下一个要执行的任务
task_entry* find_next_run_task() {
    if (task == NULL || task->next == NULL) {
        return NULL;
    }

    task_entry* tmp = task;
    while (tmp && tmp->status == TASK_STATUS_STOP && tmp->next) {
        tmp = tmp->next;
    }

    if (tmp->next) {
        return tmp->next;
    } else {
        return task;
    }
}

u32 task_switch(interrupt_info* info) {
    //保存info到current_task
    //找到下一个task
    //把下一个task的信息直接修改到info里，这样等中断处理结束后，就会还原之前的操作

    if (task == NULL) {
        return info->sp;
    }

    if (current_task != NULL) {
        current_task->esp = info->sp;
    }

    task_entry *next_task = find_next_run_task();
    if (!next_task || next_task == current_task) {
        return info->sp;
    }

    //切换当前task
    if (current_task != NULL) {
        current_task->status = TASK_STATUS_STOP;
    }
    next_task->status = TASK_STATUS_START;
    current_task = next_task;
    // print_number("cur esp: %x ", info->sp);
    print_interrupt(info);
    print_number("next esp: %x \n", next_task->esp);
    return next_task->esp;
}

int fork() {
    // task_entry* t = (task_entry*)malloc(sizeof(task_entry), 0);
    // t->parent_dir = current_task->dir;
    // t->dir = clone_page_dir(t->parent_dir);
    // t->parent = current_task;
    // t->status = TASK_STATUS_STOP;
    // t->pid = pid++;

    // u32 frame = find_valid_frame();
    // if (frame == -1) {
    //     return 0;
    // }
    // t->stack = frame + 4095; //栈从高往低走
    // add_task(t);

    // //拷贝栈
    // memcpy(t->parent->stack - 4095, t->stack - 4095, 4096);

    // if (current_task == t->parent) {
    //     //父进程
    //     return t->pid;
    // } else {
    //     //子进程
    //     return 0;
    // }
}

int get_pid() {
    return current_task ? current_task->pid : 0;
}

void add_task(task_entry* t) {
    if (task == NULL) {
        task = t;
        return;
    }

    task_entry *tmp = task;
    while (tmp->next) {
        tmp = tmp->next;
    }
    tmp->next = t;
}

void print_task(task_entry *t) {
    print_number("task pid: %d", t->pid);
    print_number(" parent pid: %d\n", t->parent ? t->parent->pid : 0);
    print_number("ebp: %x", t->ebp);
    print_number(" esp: %x", t->esp);
    print_number(" eip: %x\n", t->eip);
    print_number("dir %x\n", (u32)t->dir);
    
    if (t->status == TASK_STATUS_START) {
        printsl("status: START\n");
    } else {
        printsl("status: STOP\n");
    }
}

void create_kernel_task(task_entry_handle handle) {
    task_entry* t = (task_entry*)malloc(sizeof(task_entry), 0);
    t->parent = NULL;
    t->status = TASK_STATUS_STOP;
    t->pid = pid++;
    t->next = NULL;
    t->dir = kernel_page_dir;
    t->parent_dir = NULL;
    t->entry = (u32) handle;

    u32 frame = (u32)malloc(4096, 1);
    t->stack = frame; //栈基址

    interrupt_info context;
    context.cs = 8;
    context.int_code = 0;
    context.int_no = 0;
    context.eax = 0;
    context.ebx = 0;
    context.ecx = 0;
    context.edx = 0;
    context.edi = 0;
    context.esi = 0;
    context.ss = 0x10;
    context.sp = t->stack + 4096 - sizeof(interrupt_info) -1;
    context.eip = t->entry;
    context.flags = 0x202; //中断和df

    t->esp = context.sp;

    //往栈里插入最开始的中断帧
    // print_number("stack %x ", t->stack);
    // print_number("src %x, ", &context);
    // print_number("dst %x\n", t->stack - sizeof(interrupt_info) +1);
    // print_interrupt(&context);
    memcpy(&context, (void*)(context.sp), sizeof(interrupt_info));
    // print_interrupt(t->esp+1);
    add_task(t);
}

int create_kernel_normal_task(task_entry_handle handle) {
}