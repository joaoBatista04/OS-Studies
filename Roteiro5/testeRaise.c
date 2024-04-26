#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
void handler(int sig)
{
    // printf("Signal received : %d\n", sig);
}
int main()
{
    __pid_t pid = fork();

    if (pid == 0)
    {
        signal(SIGILL, handler);
        printf("Sending signal : %d\n", SIGILL);
        raise(SIGILL);
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        sleep(5);
        if (WIFSIGNALED(wstatus))
        {
            printf("[PARENT]: Child terminated with signal %d.\n", WTERMSIG(wstatus));
        }

        if (WSTOPSIG(wstatus))
        {
            printf("[PARENT]: Child stopped with signal %d.\n", WSTOPSIG(wstatus));
        }

        if (WIFEXITED(wstatus))
        {
            printf("[PARENT]: Child returned with code %d.\n",
                   WEXITSTATUS(wstatus));
        }
    }
    return 0;
}