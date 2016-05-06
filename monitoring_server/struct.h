#pragma once

#include "/usr/include/mysql/mysql.h"

#define KB	1024
#define MB	(1024*1024)
#define GB	(1024*1024*1024)

#define TRUE	1
#define FALSE	0
#define bool	int


typedef struct VM{
	char name[64];
	char ip[128];
	int port;
	unsigned long long alloc_disk;
	unsigned long long req_disk;
	int vec_disk;
	int alloc_cpu;
	int req_cpu;
	int vec_cpu;
	unsigned long long alloc_mem;
	unsigned long long req_mem;
	int vec_mem;	 
	bool is_run;
}s_vm;

typedef struct HOST{
	char ip[128];
	unsigned long long disk;
	unsigned long long capable_disk;
	int cpu;
	int capable_cpu;
	unsigned long long mem;
	unsigned long long capable_mem;
	int gross_vm_cnt;
	int live_vm_cnt;
	int max_vm_cnt;
}s_host;

typedef struct DATETIME{
	int year, month, day;
	int hour, minute, second;
}s_datetime;

typedef struct VM_usage{
//	float disk_usage;
	float cpu_usage;
	float mem_usage;
}s_usage;

typedef struct LOG{
	int log_num;
	char name[64];
	int sequence;
	s_datetime date_time;
	s_usage usage;
}s_log;

typedef struct t_param{
	int fd;
	char ip[128];
	MYSQL conn;
}s_thr_param;

