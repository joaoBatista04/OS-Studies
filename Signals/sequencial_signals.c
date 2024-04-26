#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void trata_SIGUSR1(int sig)
{
    printf("Tratando SIGUSR1.\n");
}

void trata_SIGUSR2(int sig)
{
    printf("Tratando SIGUSR2.\n");
    raise(SIGUSR2);
    printf("Fim do SIGUSR2.\n");
}

int main(void)
{

    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, trata_SIGUSR2);

    raise(SIGUSR2);

    sleep(2);
    char *const *args = "ls";
    execvp("ls", args);
    return 0;
}
