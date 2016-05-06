#include "my_socket.h"

int init_socket(int * fd, struct sockaddr_in * addr)
{
	int ret;
 
	*fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(-1 == *fd)
	{
		printf("Server : socket func failed\n");
		return -1;
	}
	
	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = htonl(INADDR_ANY);
	addr->sin_port = htons(9000);

    ret = bind(*fd, (struct sockaddr*)addr, sizeof(*addr));

	if(ret < 0)
	{
		printf("Server : bind failed \n");
		return -1;
	}

	ret = listen(*fd, 5);

	if(ret < 0)
	{
		printf("Server : listen failed\n");
		return -1;
	}
	
	return 0;
}

int hi_client(int s_fd, s_thr_param * param, struct sockaddr_in * c_addr)
{
	int len;
	
	len = sizeof(*c_addr);
	param->fd = accept(s_fd, (struct sockaddr*)c_addr, &len);

	if( param->fd < 0 )
	{
		printf("Server accept failed \n");
		return -1;
	}

	inet_ntop(AF_INET, &(c_addr->sin_addr.s_addr), param->ip, sizeof(param->ip));

	struct timeval timeout = {60, 0};
	
	setsockopt(param->fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	
	return 0;
}





