

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <libgen.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MAXLEN 1024
#define LISTENQ 5
#define OPEN_MAX 32
#define INFTIM -1

int bind_and_listen(int port)
{
    int servfd;
    struct sockaddr_in my_addr;
    if ((servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }
    printf("socket ok!\n");

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(servfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        return -2;
    }
    printf("bind ok!\n");

    if (listen(servfd, LISTENQ) == -1)
    {
        perror("listen");
        return -3;
    }
    printf("listen ok!\n");
    return servfd;

}

void do_poll(int listenfd)
{
    int connfd, sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    struct pollfd client_fds[OPEN_MAX];
    int max = 0;
    int nready;

    client_fds[0].fd = listenfd;
    client_fds[0].events = POLLIN;
    for (int i = 1; i < OPEN_MAX; ++i)
        client_fds[i].fd = -1;

    while (true)
    {
        nready = poll(client_fds, max+1, INFTIM);
        if (nready == -1)
        {
            perror("poll error");
            exit(1);
        }

        // 有新连接时，获得新fd，并存入client_fd数组中
        if (client_fds[0].revents & POLLIN)
        {
            client_addr_len = sizeof(client_addr);
            // 建立连接
            if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1)
            {
                if (errno == EINTR)
                    continue;
                else  
                {
                    perror("accept error");
                    exit(1);
                }
            }

            // 打印新连接的IP + port
            fprintf(stdout, "accept a new client : %s:%d \n", 
                inet_ntoa(client_addr.sin_addr), 
                client_addr.sin_port);
            // 将新的描述符加到数组中（选取client_fds[0]之外第一个未使用的）
            for (int i = 1; i < OPEN_MAX; ++i)
            {
                if (client_fds[i].fd < 0)
                {
                    max = i > max ? i : max;
                    if (max == OPEN_MAX)
                    {
                        fprintf(stderr, "too many client. \n");
                        exit(1);
                    }
                    printf("current connection num = %d\n", max);
                    client_fds[i].fd = connfd;
                    client_fds[i].events = POLLIN;
                    printf("already add.\n");
                    break;
                }
            }
            // 记录连接套接字个数
            if (--nready <= 0)
                continue;
        }
        char buff[MAXLEN];
        memset(buff, 0, MAXLEN);
        int readlen = 0;
        for(int i = 1; i <= max; i++)
        {
            // 寻找集合中已存在描述符并测试客户端描述符是否准备好
            if(client_fds[i].fd < 0)
                continue;
            if(client_fds[i].revents & POLLIN)
            {
                // 客户端准备好则接受客户端发送的信息
                readlen = read(client_fds[i].fd, buff, MAXLEN);
                // 返回0表示EOF，结束读取，清除该描述符
                if(readlen == 0){
                    close(client_fds[i].fd);
                    client_fds[i].fd = -1;
                    continue;
                }
                // 向客户端发送buff
                write(STDOUT_FILENO, buff, readlen);
                write(client_fds[i].fd, buff, readlen);
            }
        }
    }
}
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("usage: %s ip port\n", basename(argv[0]));
        return 1;
    }

    int port = atoi(argv[2]);
    int listenfd = bind_and_listen(port);
    if (listenfd < 0)
        return 0;
    do_poll(listenfd);
    return 0;
}