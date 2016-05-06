#include "/usr/include/mysql/mysql.h"
#include "db.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_conn(MYSQL * conn)
{
	mysql_init(conn);

	if(!mysql_real_connect(conn, HOST, USER, PASS, NULL, 3306, (char *)NULL, 0))
	{
		printf("%s\n", mysql_error(conn));
		return -1;
	}

	return 0;	
}

void close_conn(MYSQL * conn)
{
	mysql_close(conn);
}


