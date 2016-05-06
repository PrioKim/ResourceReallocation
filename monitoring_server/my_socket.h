#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "struct.h"

int init_socket(int * fd, struct sockaddr_in * addr); 
// from socket to listen

int hi_client(int s_fd, s_thr_param * param, struct sockaddr_in * c_addr); 
// accept and get client ip, fd, set timeout time
// set is_run field

