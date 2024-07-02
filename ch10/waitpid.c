#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();  // 创建子进程

    if (pid == 0) {  // 子进程,运行子进程
        sleep(15);  // 休眠15秒
        return 24;  // 子进程结束，返回值24
    } else {  // 父进程
        // 非阻塞等待子进程结束
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);  // 每3秒检查一次子进程状态
            puts("sleep 3 sec.");
        }
        if (WIFEXITED(status))  // 检查子进程是否正常终止
            printf("Child send %d \n", WEXITSTATUS(status));  // 获取子进程返回值
    }
    return 0;
}
