#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int tcp_sock, udp_sock;   // 声明两个套接字变量
    int sock_type;            // 存储套接字类型的变量
    socklen_t optlen;         // 存储选项长度的变量
    int state;                // 存储状态的变量


    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);//1
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);//2,分别代表套接字的类型

     // 获取 TCP 套接字的类型
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if (state)
        error_handling("getsockopt() error");
    printf("Socket type one: %d \n", sock_type);

     // 获取 UDP 套接字的类型
    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void *)&sock_type, &optlen);
    if (state)
        error_handling("getsockopt() error");
    printf("Socket type two: %d \n", sock_type);
    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}