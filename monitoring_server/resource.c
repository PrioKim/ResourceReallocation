#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"


int shell_command(char * cmd_buf, void * result_buf)
{
	FILE * fp = NULL;
	size_t readSize = 0;

	fp = popen(cmd_buf, "r");
	if(!fp)
	{
		printf("popen error\n");
		return -1;
	}

	readSize = fread((void*)result_buf, sizeof(char), 4096, fp);

	pclose(fp);

	if(readSize == 0)
	{
		printf("fread error in shell command\n");
		return -1;
	}

	((char *)result_buf)[readSize] = '\0';

	return 0;
}

void remove_space(char * str)
{
	int i = 0, j = 0;

	for(i = 0; i < strlen(str); i++)
	{
		if(	str[i] == ' ' || str[i] == '\n' )
			continue;

		str[j] = str[i];
		j++;
	}

	str[j] = '\0';
}

int _get_gross_vm_cnt()
{
	char result[1024];
	char CMD[1024];
	int cnt = 0;

	sprintf(CMD, "virsh list --all | wc -l");
	//sprintf(CMD, "virsh list --all | grep -v %s | wc -l", GOLD_IMG);

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell cmd failed\n");
		return -1;
	}

	sscanf(result, "%d", &cnt);

	return cnt - 3;
}

int _get_live_vm_cnt()
{
	char result[1024];
	char CMD[1024];
	int cnt;

	sprintf(CMD, "virsh list | grep -v %s | wc -l", GOLD_IMG);

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell cmd failed\n");
		return -1;
	}

	sscanf(result, "%d", &cnt);

	return cnt-3;
}


int _get_gross_cpu(int * cpus)
{}
int _get_capa_cpu(int * cpus)
{}
int _get_gross_mem(unsigned long long * mems)
{
	FILE * fp;
	char buffer[4096] = {0, };
	char * ptr;
	
	*mems = 0;

	fp = fopen("/proc/meminfo", "r");

	if(!fp)
	{
		printf("fopen error\n");
		return -1;
	}

	if(!fgets(buffer, sizeof(buffer), fp))
	{
		printf("fgets error\n");
		fclose(fp);
		return -1;
	}

	remove_space(buffer);
	ptr = strchr(buffer, ':');
	sscanf(++ptr, "%Lu", mems);
	
	*mems = *mems - 1 * MB; // meminfo implemented by KB

	printf("gross mem %Lu\n", *mems);

	fclose(fp);

	return 0;
}

int _get_vm_name(char vm_name[][64])
{
	char CMD[1024];
	char result[4096];
	char * s_ptr;
	char * e_ptr;
	int i, vm_cnt = _get_gross_vm_cnt();

	//sprintf(CMD, "virsh list --all | grep -v %s | grep -v Name | awk '{print $2}'", GOLD_IMG);
	sprintf(CMD, "virsh list --all | grep -v Name | awk '{print $2}'"); // test 

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell cmd failed\n");
		return -1;
	}

	s_ptr = strchr(result, '\n');
	s_ptr++;

	for(i = 0; i < vm_cnt; i++) // add vm name  line by line
	{
		e_ptr = strchr(s_ptr, '\n');
		*e_ptr = '\0';
		strcpy(vm_name[i], 	s_ptr);
		s_ptr = ++e_ptr;
	}

	return 0;
}

int _get_capa_mem(unsigned long long * mems)
{
	char vm_name[MAX_VM_CNT][64];
	char CMD[1024];
	char result[4096];
	char * ptr;
	int i ,vm_cnt = _get_gross_vm_cnt();
	unsigned long long tmp;

	*mems = 0;

	_get_vm_name(vm_name);

	for(i = 0; i < vm_cnt; i++)
	{
		sprintf(CMD, "virsh dumpxml %s | grep Memory", vm_name[i]);
		
		if(shell_command(CMD, (void *)result) < 0)
		{
			printf("shell cmd failed\n");
			return -1;
		}
		ptr = strchr(result, '>');
		sscanf(++ptr, "%Lu", &tmp);
			
		*mems = *mems + tmp;
	}

	_get_gross_mem(&tmp);
	
	printf("capa_mem : %Lu\n", tmp-*mems);

	return 0;
}

int _get_gross_disk(unsigned long long * disks)
{
	char result[1024];
	char CMD[1024];

	sprintf(CMD, "df -P | grep %s | awk '{print $2}'", MOUNT_DIR);

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell cmd failed\n");
		return -1;
	}

	sscanf(result, "%Lu", disks);

	return 0;
}

int _get_capa_disk(unsigned long long * disks)
{
	char result[1024];
	char CMD[1024];

	sprintf(CMD, "df -P | grep %s | awk '{print $4}'", MOUNT_DIR);

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell cmd failed\n");
		return -1;
	}

	sscanf(result, "%Lu", disks);

	return 0;
}

int init_host(s_host * host_info)
{
	strcpy(host_info->ip, HOST_IP);

	_get_gross_disk(&host_info->disk);
	_get_capa_disk(&host_info->capable_disk);
	_get_gross_cpu(&host_info->cpu);
	_get_capa_cpu(&host_info->capable_cpu);
	_get_gross_mem(&host_info->mem);
	_get_capa_mem(&host_info->capable_mem);
	
	host_info->gross_vm_cnt = _get_gross_vm_cnt();
	host_info->live_vm_cnt = _get_live_vm_cnt();
	host_info->max_vm_cnt = MAX_VM_CNT;

	return 0;
}

int monitor_host(s_host * host_info)
{
	s_host prev_host_info = *host_info;

	_get_gross_disk(&prev_host_info.disk);
	_get_capa_disk(&prev_host_info.capable_disk);
	_get_gross_cpu(&prev_host_info.cpu);
	_get_capa_cpu(&prev_host_info.capable_cpu);
	_get_gross_mem(&prev_host_info.mem);
	_get_capa_mem(&prev_host_info.capable_mem);
	
	prev_host_info.gross_vm_cnt = _get_gross_vm_cnt();
	prev_host_info.live_vm_cnt = _get_live_vm_cnt();
	
	
	if( prev_host_info.disk != host_info->disk ||
		prev_host_info.capable_disk != host_info->capable_disk ||
		prev_host_info.cpu != host_info->cpu ||
		prev_host_info.capable_cpu != host_info->capable_cpu ||
		prev_host_info.mem != host_info->mem ||
		prev_host_info.capable_mem != host_info->capable_mem ||
		prev_host_info.gross_vm_cnt != host_info->gross_vm_cnt ||
		prev_host_info.live_vm_cnt != host_info->live_vm_cnt)
	{
		return 1;
	}
	
	return 0;
}
