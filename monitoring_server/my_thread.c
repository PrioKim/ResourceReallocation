#include "my_thread.h"
#include "resource.h"

void * thread_main(void * _param)
{
	s_thr_param param = *(s_thr_param *)_param;

	while(1)
	{
		s_usage rcv_buf;
		char send_buf[128];
		int len;

		len = read(param.fd, &rcv_buf, sizeof(s_usage));
		
		if(-1 == len && errno == EAGAIN) // is_run update
		{
			printf("[%s] timeout!\n", param.ip);
			close(param.fd);
			return NULL;
		}

		if(0 == len) // is_run update
		{
			printf("[%s] close!\n", param.ip);
			close(param.fd);
			return NULL;
		}

		printf("[%s] %f, %f \n", param.ip, rcv_buf.cpu_usage, rcv_buf.mem_usage);

		len = sprintf(send_buf, "server recieved!");
		int ret = write(param.fd, send_buf, len);

		if(-1 == ret && errno == EPIPE)
		{
			printf("[%s] socket broken\n", param.ip);
			close(param.fd);
			return NULL;
		}
	}
}

void * thr_mon_server(void* _param)
{
	s_host host_info;

	MYSQL * conn = (MYSQL *)_param;

	init_host(&host_info);

	while(1)
	{
		if(monitor_host(&host_info))
//			update_db(conn, host_info);
		
		sleep(1);
	}
	
}




