#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int acpt_sock, recv_sock;
    struct sockaddr_in acpt_adr, recv_adr;
    int str_len, state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr *)&acpt_adr, sizeof(acpt_adr)) == -1)
        error_handling("bind() error");
    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&recv_adr, &recv_adr_sz);

    while (1)
    {
        //保证就算不存在待读取数据也不会阻塞
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
        //recv 函数尝试从 recv_sock 读取数据，使用了两个标志：
        // MSG_PEEK：使 recv 函数在读取数据后不从接收缓冲区中移除这些数据，相当于“窥探”数据。首先窥探数据，确保缓冲区中确实有数据。
        // MSG_DONTWAIT：使 recv 函数为非阻塞模式，如果没有数据可读则立即返回而不是等待。然后有数据之后就直接发送
        if (str_len > 0)
            break;
    }
    buf[str_len] = 0;
    printf("Buffering %d bytes : %s \n", str_len, buf);
    //再次调用 recv 函数，这一次没有设置任何可选项，所以可以直接从缓冲区读出
    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
   
    buf[str_len] = 0;
    printf("Read again: %s \n", buf);
    close(acpt_sock);
    close(recv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}