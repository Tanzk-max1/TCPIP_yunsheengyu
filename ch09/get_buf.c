#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    int snd_buf, rcv_buf, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *)&snd_buf, &len);
    //SOL_SOCKET 表示在套接字级别获取选项，SO_SNDBUF 表示获取发送缓冲区大小。
    //snd_buf 用于存储结果。getsockopt 获取发送缓冲区的大小
    //在底层完成定义，然后进行获取选项来完成选择
    if (state)
        error_handling("getsockopt() error");

    len = sizeof(rcv_buf);//如果没有这两句进行状态获取，会输出32765范围附近的
    //接收缓冲区大小为 32765，这个值是操作系统给你的套接字默认分配的缓冲区大小。在一些系统中，这个值可能会稍有不同，但通常会在同一个数量级范围内。
    //可以进行默认修改
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *)&rcv_buf, &len);
    //使用 getsockopt 获取接收缓冲区的大小。
    //SO_RCVBUF 表示获取接收缓冲区大小。rcv_buf 用于存储结果。

    //设置接收缓冲区大小（SO_RCVBUF）后，通过调用 getsockopt 获取缓冲区大小可能会发现其值比设置的值要大。
    //这是因为操作系统在处理缓冲区大小时会应用某些内部调整和优化，
    //内部调整机制
    //对齐和页大小：操作系统通常会基于内存页大小进行对齐，可能会分配略大于请求的缓冲区。
    //系统开销：操作系统可能会分配额外的空间来管理缓冲区或支持内核中的其他网络栈功能。

    if (state)
        error_handling("getsockopt() error");

    printf("Input buffer size: %d \n", rcv_buf);
    printf("Output buffer size: %d \n", snd_buf);

    return 0;
}
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}