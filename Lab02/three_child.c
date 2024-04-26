#include <stdio.h>
#include <unistd.h> // For the syscall functions.
#include <errno.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid1 = fork();
    pid_t pid2 = fork();
    pid_t pid3 = fork();

    if (pid1 == 0 || pid2 == 0 || pid3 == 0)
    {
        sleep(2);
    }

    else
    {
        while (1)
        {
            sleep(2);

            pid_t pid = waitpid(-1, NULL, WNOHANG);

            if (pid > 0)
            {
                printf("Meu filho pid=%d terminou.\n", pid);
            }
            else
            {
                if (errno == ECHILD)
                    break;
                else
                    printf("Nenhum filho terminou.\n");
            }
        }
    }

    return 0;
}