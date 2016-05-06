#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

struct param
{
    int cfd;
    char ip[INET_ADDRSTRLEN];
};

void *function(void *_param)
{
    struct param *param = (struct param*)_param;

    while(1)
    {
        char buffer[1024] = {0, };
        int len;

        len = read(param->cfd, buffer, sizeof(buffer));

        if(-1 == len && errno == EAGAIN)
        {
            printf("[%s] timeout!\n", param->ip);

            close(param->cfd);

            return NULL;
        }

        if(0 == len)
        {
            printf("[%s] close!\n", param->ip);

            close(param->cfd);

            return NULL;
        }

        buffer[len] = '\0';
        printf("[%s]%s\n", param->ip, buffer);

        len = sprintf(buffer, "success!");

        int ret = write(param->cfd, buffer, len);

        if(-1 == ret && errno == EPIPE)
        {
            printf("[%s] break!\n", param->ip);

            close(param->cfd);

            return NULL;
        }

    }


    return NULL;
}

int main()
{
    int sfd, cfd;
    struct sockaddr_in saddr, caddr;
    int saddr_len, caddr_len;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(9001);
    
    saddr_len = sizeof(saddr);
    caddr_len = sizeof(caddr);

    bind(sfd, (struct sockaddr*)&saddr, saddr_len);

    listen(sfd, 5);

    while(1)
    {
        pthread_t thread_id;
        struct param param;

        cfd = accept(sfd, (struct sockaddr*)&caddr, &caddr_len);

        param.cfd = cfd;
        inet_ntop(AF_INET, &(caddr.sin_addr.s_addr), param.ip, INET_ADDRSTRLEN);

        struct timeval timeout = {3, 0};

        setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        pthread_create(&thread_id, NULL, function, (void*)&param);
    }


    close(sfd);

    return 0;
}
