#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads); //注册服务端套接字
    fd_max = serv_sock;

    while (1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        //使用 select 函数监视文件描述符集合，等待其中一个变为可读
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
        {
            break;
        }
        // 如果在超时时间内没有任何文件描述符变为可读，则继续等待
        if (fd_num == 0)
        {
            continue;
        }
        // 遍历所有可能的文件描述符，查找变为可读的文件描述符
        for (i = 0; i < fd_max; i++)
        {
            if (FD_ISSET(i,&cpy_reads))//查找发生变化的套接字文件描述符
            {
                if (i == serv_sock) //如果是服务端套接字时,受理连接请求
                {
                    adr_sz = sizeof(clnt_adr);//一般是用来这个地方
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
                    //注册一个clnt_sock
                    FD_SET(clnt_sock,&reads);// 将新的客户端套接字加入文件描述符集合
                    if (fd_max < clnt_sock)
                    {
                        fd_max = clnt_sock;// 更新最大文件描述符
                    }
                    printf("Connected client: %d \n", clnt_sock);
                }
                else// 处理已有连接的客户端数据
                {
                    str_len = read(i, buf, BUF_SIZE); //i指的是当前发起请求的客户端
                    if (str_len == 0) // 客户端关闭连接
                    {
                        FD_CLR(i,&reads); // 从文件描述符集合中移除该客户端
                        close(i);// 关闭客户端套接字
                        printf("closed client: %d \n", i);
                    }
                    else// 读取客户端数据并回送
                    {
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
