#pragma once
#include "struct.h"

#define HOST_IP		"210.107.232.105"
#define MOUNT_DIR	"/home"
#define MAX_VM_CNT	8
#define GOLD_IMG	"gold_image"

int shell_command(char * cmd_buf, void * result_buf);
void remove_space(char * result);

int init_host(s_host * host_info);
int monitor_host(s_host * host_info);
int _get_vm_name(char vm_name[][64]);
int _get_gross_cpu(int * cpus); // from host machine
int _get_capa_cpu(int * cpus);  // from DB 
int _get_gross_mem(unsigned long long * mems);
int _get_capa_mem(unsigned long long * mems);
int _get_gross_disk(unsigned long long * disks);
int _get_capa_disk(unsigned long long * disks);

int _get_gross_vm_cnt();
int _get_live_vm_cnt();

int update_db(MYSQL * conn, s_host host_info);
