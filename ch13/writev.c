#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    int str_len;

    // 初始化 iovec 结构体数组
    vec[0].iov_base = buf1;  // 指定第一个缓冲区的基地址
    vec[0].iov_len = 3;      // 指定第一个缓冲区的长度为 3 字节
    vec[1].iov_base = buf2;  // 指定第二个缓冲区的基地址
    vec[1].iov_len = 4;      // 指定第二个缓冲区的长度为 4 字节

    // 使用 writev 将数据写入文件描述符 1 (stdout)
    str_len = writev(1, vec, 2);
    //因为是标准输出函数，所以一般文件标识符都是1，而不是0，向标准输入写入数据，这通常是没有意义的，因为标准输入是用于读操作的。
    //文件描述符（file descriptors, FD）不仅限于标准输入（0）、标准输出（1）和标准错误输出（2）。
    puts("");  // 输出一个空行，确保输出整洁
    printf("Write bytes: %d \n", str_len);  // 打印实际写入的字节数
    return 0;
}
