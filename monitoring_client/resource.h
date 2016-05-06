#pragma once
#include "struct.h"

#define LOGTIME 3

typedef struct CPU_info{
	 char cpu_name[8];
     unsigned long long int user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
	 unsigned long long int prev_user, prev_nice, prev_sys, prev_idle, prev_iowait;
	 unsigned long long int prev_irq, prev_softirq, prev_steal, prev_guest, prev_guest_nice;
}s_CPU;

typedef struct MEM_info{
	unsigned long long int MemTotal, MemFree, Buffers, Cached, SwapCached;
}s_MEM;


int shell_command(char * cmd_buf, void * result_buf);
int get_usage(s_usage * buf, s_CPU * cpu_info);
void remove_space(char * result);
int init_cpu_info(s_CPU * cpu_info);
int _get_cpu_info(s_CPU * cpu_info);
int _cal_cpu_usage(s_CPU * cpu_info, float * usage);
float get_cpu_usage(s_CPU * cpu_info);
int _get_mem_info(s_MEM * mem_info);
int _cal_mem_usage(s_MEM * mem_info, float * usage);
float get_mem_usage();
float get_disk_usage();


