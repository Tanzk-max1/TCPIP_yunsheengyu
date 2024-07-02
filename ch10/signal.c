#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) //信号处理器
{
    if (sig == SIGALRM)
        puts("Time out!");
    alarm(2); //为了每隔 2 秒重复产生 SIGALRM 信号，在信号处理器中调用 alarm 函数
}
void keycontrol(int sig) //信号处理器
{
    if (sig == SIGINT)
        puts("CTRL+C pressed");
}
int main(int argc, char *argv[])
{
    int i;
    signal(SIGALRM, timeout); //注册信号及相应处理器
    signal(SIGINT, keycontrol);
    alarm(2); //预约 2 秒候发生 SIGALRM 信号

    for (i = 0; i < 3; i++)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}

//简单来说就是三种触发方式不同
// 程序开始执行 main 函数。
// 调用 signal(SIGALRM, timeout); 注册 SIGALRM 的处理函数。
// 调用 signal(SIGINT, keycontrol); 注册 SIGINT 的处理函数。
// 调用 alarm(2); 设置定时器在 2 秒后发送 SIGALRM 信号。
// 进入 for 循环并执行 puts("wait..");，立即输出 "wait.."。
// 调用 sleep(100); 使程序进入睡眠状态，但在 2 秒后会被 SIGALRM 信号中断。
// 2 秒后，SIGALRM 信号触发，调用 timeout 函数，输出 "time out!"，然后再次设置 alarm(2);。