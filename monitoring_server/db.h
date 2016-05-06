#pragma once

#include "struct.h"


#define HOST	"localhost"
#define USER	"root"
#define PASS	"realppp"
#define DB		"SVSA"

int init_conn(MYSQL * conn);
void close_conn(MYSQL * conn);
int get_data(MYSQL * conn, char * query, char * buf);
int set_data(MYSQL * conn, char * query, char * buf);
