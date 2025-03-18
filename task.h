#ifndef TASK_H
#define TASK_H

#include "global.h"
#include "page.h"
#include "interrupt.h"

#define TASK_STATUS_STOP 1
#define TASK_STATUS_START 2

typedef struct task
{
    u32 pid;
    u32 esp;
    u32 ebp;
    u32 eip;
    u32 entry; //入口方法
    u32 stack; //堆栈地址
    page_directory_entry* parent_dir;
    page_directory_entry* dir;
    u8 status;
    struct task *parent;
    struct task *next;
    u32 count;
} task_entry;

void init_task();

void add_task(task_entry* t);

//切换进程任务
void task_switch_detail(task_entry *current, interrupt_info *current_context ,task_entry* next);

//获取下一个要执行的任务
task_entry* find_next_run_task();

u32 task_switch(interrupt_info* info);

int fork();

void move_stack(void *new_stack_start, u32 size);

int get_pid();

void print_task(task_entry *t);

//任务入口方法
typedef int (*task_entry_handle)();

void create_kernel_task(task_entry_handle handle);

int create_kernel_normal_task(task_entry_handle handle);

#endif