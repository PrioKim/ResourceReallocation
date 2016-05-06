#include <stdlib.h>

#include "my_socket.h" 
// include stdio, sys/socket, netinet/in, struct

#include "my_thread.h" 
// include stdio struct, pthread, errno
#include "db.h"
#include <unistd.h>

int main()
{
	int s_fd;
	struct sockaddr_in s_addr, c_addr;

	s_thr_param param;
	pthread_t thread_id;
	
	if(init_socket(&s_fd, &s_addr) < 0) 
	{
		printf("init_socket failed \n");
	}

//	if(init_conn(&param.conn) < 0)
//	{
//		printf("init_conn failed \n");
//	}

//	pthread_create(&thread_id, NULL, thr_mon_server, (void *)&parma.conn);	
	
	while(1)
	{
		hi_client(s_fd, &param, &c_addr);

		pthread_create(&thread_id, NULL, thread_main, (void *)&param);
	}

	close(s_fd);
	close_conn(&param.conn);
	
	return 0;
}
