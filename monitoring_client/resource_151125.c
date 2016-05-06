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


int _get_cpu_info(s_CPU * cpu_info)
{
	FILE * fp;
	char buffer[4096] = {0, };
	int ret = 0;

	fp = fopen("/proc/stat", "r");

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

	ret = sscanf(buffer, "%s %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
				cpu_info->cpu_name,
				&cpu_info->user, &cpu_info->nice, &cpu_info->sys, &cpu_info->idle,
				&cpu_info->iowait, &cpu_info->irq, &cpu_info->softirq, &cpu_info->steal,
				&cpu_info->guest, &cpu_info->guest_nice);

	fclose(fp);
	
	printf("cpu_name : %s \n", cpu_info->cpu_name);

	if(ret < 4)
		return -1;
	

	return 0;
}

int _cal_cpu_usage(s_CPU * cpu_info, float * usage)
{
	unsigned long long int Prev_idle, Prev_non_idle, Prev_total;
	unsigned long long int Idle, Non_idle, Total;
	
	Prev_idle = cpu_info->prev_idle + cpu_info->prev_iowait;
	Prev_non_idle = cpu_info->prev_user + cpu_info->prev_nice + cpu_info->prev_sys
					+ cpu_info->prev_irq + cpu_info->prev_softirq + cpu_info->prev_steal;
	Prev_total = Prev_idle + Prev_non_idle;

	Idle = cpu_info->idle + cpu_info->iowait;
	Non_idle = cpu_info->user + cpu_info->nice + cpu_info->sys + cpu_info->irq 
				+ cpu_info->softirq + cpu_info->steal;
	Total = Idle + Non_idle;
	
	*usage = (((Total - Prev_total) - (Idle - Prev_idle)) / (float)(Total - Prev_total)) * 100;


	cpu_info->prev_user = cpu_info->user;
	cpu_info->prev_nice = cpu_info->nice;
	cpu_info->prev_sys = cpu_info->sys;
	cpu_info->prev_idle = cpu_info->idle;
	cpu_info->prev_iowait = cpu_info->iowait;
	cpu_info->prev_irq = cpu_info->irq;
	cpu_info->prev_softirq = cpu_info->softirq;
	cpu_info->prev_steal = cpu_info->steal;
	cpu_info->prev_guest = cpu_info->guest;
	cpu_info->prev_guest_nice = cpu_info->guest_nice;


	return 0;
}

float get_cpu_usage(s_CPU * cpu_info)
{
	float usage;

	_get_cpu_info(cpu_info);
	_cal_cpu_usage(cpu_info, &usage);

	return usage;
}

int _get_mem_info(s_MEM * mem_info)
{
	FILE * fp;
	char buffer[4096] = {0, };
	unsigned long long int fields[5];
	int i = 0;
	char * ptr;

	fp = fopen("/proc/meminfo", "r");

	if(!fp)
	{
		printf("fopen error\n");
		return -1;
	}

	while(1)
	{
		if(!fgets(buffer, sizeof(buffer), fp))
		{
			printf("fgets error\n");
			fclose(fp);
			return -1;
		}

		remove_space(buffer);
		ptr = strchr(buffer, ':');
		*ptr = '\0';			// meminfo 파일 구조가 다를수있음		
		
		if(strcmp(buffer, "MemAvailable") == 0)
			continue;

		sscanf(++ptr, "%Lu", &fields[i]);
		i++;

		if(strcmp(buffer, "SwapCached") == 0)
			break;
	}

	fclose(fp);

	mem_info->MemTotal = fields[0];
	mem_info->MemFree = fields[1];
	mem_info->Buffers = fields[2];
	mem_info->Cached = fields[3];
	mem_info->SwapCached = fields[4];

	return 0;
}

int _cal_mem_usage(s_MEM * mem_info, float * usage)
{
	float non_used, total;

	total = mem_info->MemTotal;
	non_used = mem_info->MemFree + mem_info->Buffers + mem_info->Cached + mem_info->SwapCached;

	*usage = ((total - non_used) / total) * 100;
		
	return 0;
}

float get_mem_usage()
{
	float usage;
	s_MEM mem_info;

	_get_mem_info(&mem_info);
	_cal_mem_usage(&mem_info, &usage);

	return usage;

}

float get_disk_usage()
{
	float usage;
	char result[1024];
	char CMD[1024] = "DISK_TOTAL=`df -P | grep -v ^Filesystem | awk '{sum += $2} END { print sum; }'`;DISK_USED=`df -P | grep -v ^Filesystem | awk '{sum += $3} END { print sum; }'`;DISK_PER=`echo \"100*$DISK_USED/$DISK_TOTAL\" | bc -l`;echo \"$DISK_PER\"";

	if(shell_command(CMD, (void *)result) < 0)
	{
		printf("shell_commnad failed in get_disk_usage\n");
		return -1;
	}

	sscanf(result, "%f", &usage);

	return usage;
}

int init_cpu_info(s_CPU * cpu_info)
{
	cpu_info->prev_user = 0;
	cpu_info->prev_nice = 0;
	cpu_info->prev_sys = 0;
	cpu_info->prev_idle = 0;
	cpu_info->prev_iowait = 0;
	cpu_info->prev_irq = 0;
	cpu_info->prev_softirq = 0;
	cpu_info->prev_steal = 0;
	cpu_info->prev_guest = 0;
	cpu_info->prev_guest_nice = 0;

	return 0;
}


int get_usage(s_usage * buf, s_CPU *cpu_info)
{
	int i = 0;
	float cpu = 0;
	float mem = 0;
//	float disk = 0;
	
	while(i != LOGTIME)
	{
		cpu += get_cpu_usage(cpu_info);
		mem += get_mem_usage();
//		disk += get_disk_usage();
		
		sleep(1);
		i++;
	}


	buf->cpu_usage = cpu / (float)LOGTIME;
	buf->mem_usage = mem / (float)LOGTIME;
//	buf->disk_usage = disk / (float)LOGTIME;

	return 0;
}



