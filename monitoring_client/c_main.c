#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "resource.h"

int main()
 {
	int fd, ret;
	struct sockaddr_in s_addr;
	s_usage send_buf;
	char rcv_buf[128] = {0, };
	s_CPU cpu_info;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	init_cpu_info(&cpu_info);

	if(fd < 0)
	{
		printf("Client : socket failed\n");
		return 0;
	}

	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = inet_addr("192.168.122.1");
	s_addr.sin_port = htons(9000);

	ret = connect(fd, (struct sockaddr*)&s_addr, sizeof(s_addr));

	if(ret < 0)
	{
		printf("Client : connect failed\n");
		close(fd);
		return 0;
	}

	while(1)
	{
		int len;

		get_usage(&send_buf, &cpu_info);

		ret = write(fd, &send_buf, sizeof(send_buf));
		
		if(ret < 0)
		{
			printf("Client : write error \n");
			break;
		}

		len = read(fd, rcv_buf, sizeof(rcv_buf));

		if(len < 0)
		{
			printf("Client : read error \n");
			break;
		}

		rcv_buf[len] = '\0';
	}

	close(fd);

	return 0;
 }
