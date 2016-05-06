#pragma once
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include "struct.h"


void * thread_main(void * _param);
void * thr_mon_server(void * _param);





