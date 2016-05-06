gcc -o server main.c my_socket.c my_thread.c resource.c -lpthread -L/usr/lib64/mysql -lmysqlclient -g
