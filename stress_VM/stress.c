#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MB	1048576

void *loop_func(void *param)
{
	int i = 0;
	fprintf(stdout, "[%d]thread start\n", *(int *)param);

	while(1)
		usleep(1);
}

struct memory_alloc_unit{
	char byte_unit[MB];
};

// argv[1] : thread num 
// argv[2] : memory allocation size (MB)

int main(int argc, char ** argv[])
{
	int thread_num;
	int memory_size;
	int i, j, status;
	pthread_t *tid;
	struct memory_alloc_unit *mem;


	if(argc != 3){
		fprintf(stderr, "arg1 is thread num. and arg2 is memory stress size by MB\n");
		exit(1);
	}
	else
	{
		thread_num = atoi(argv[1]);
		memory_size = atoi(argv[2]);	

		fprintf(stdout, "thread num : %d\n", thread_num);
		fprintf(stdout, "mem stress size : %dMB\n", memory_size);
	}
	
	tid = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
	mem = (struct memory_alloc_unit *)malloc(sizeof(struct memory_alloc_unit) * memory_size);	

	for(i = 0; i < memory_size; i++) {
		for(j = 0; j < MB; j++)
			mem[i].byte_unit[j] = 0;
	}

	for(i = 0; i < thread_num; i++){
		pthread_create(&tid[i], NULL, loop_func, (void *)&i);
	}

	for(i = 0; i < thread_num; i++) {
		pthread_join(tid[i], (void **)&status);
	}


	free(mem);
	fprintf(stdout, "main exit\n");

	return 0;
}
