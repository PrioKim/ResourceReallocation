#include <stdio.h>
#include <unistd.h>

void loop()
{
	int i = 0;
	int my_pid = getpid();
	while(1)
	{
		if(i == 100000000000)
			break;
		i++;
	}
}

int main(int argc, char * argv[])
{
	int process_num = 8;
	int pid;
	int i;

	if(argc != 2)
	{
		fprintf(stderr, "arg1 is process num. [default] 8\n");
	}
	else
	{
		fprintf(stdout, "process num : %d\n", atoi(argv[1]));
		process_num = atoi(argv[1]);
	}


	for(i = 0;i < process_num; i++)
	{
		pid = fork();
		
		if(pid == 0)
			loop();
	}
	if(pid > 0)	
		wait();

	return 0;
}
