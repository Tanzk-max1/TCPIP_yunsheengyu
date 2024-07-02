#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 信号处理函数，用于处理 SIGCHLD 信号
void read_childprovc(int sig)
{
    int status;
    // 使用 waitpid 获取已终止子进程的状态
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))  // 检查子进程是否正常终止
    {
        printf("Removed proc id: %d \n", id);             // 打印已终止子进程的 pid
        printf("Child send: %d \n", WEXITSTATUS(status)); // 打印子进程的返回值
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
     // 创建第一个子进程
    pid = fork();
    if (pid == 0)  // 子进程执行代码
    {
        puts("Hi I'm child process");
        sleep(10);  // 子进程休眠10秒
        return 12;  // 子进程返回12
    }
    else //父进程执行阶段
    {
        printf("Child proc id: %d\n", pid);
        // 创建第二个子进程
        pid = fork();
        if (pid == 0)  // 第二个子进程执行代码
        {
            puts("Hi! I'm child process");
            sleep(10);  // 第二个子进程休眠10秒
            exit(24);   // 第二个子进程退出并返回24
        }
        else  // 父进程执行代码
        {
            int i;
            printf("Child proc id: %d \n", pid);
            for (i = 0; i < 5; i++)  // 父进程等待子进程终止
            {
                puts("wait");
                sleep(5);  // 每次等待5秒
            }
        }
    }
    return 0;
}
