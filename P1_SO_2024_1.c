#include <stdio.h>
#include <unistd.h>

// Desenhar a árvore de processos gerada pelo código abaixo:

int main(void)
{
    fork();
    for (int i = 0; i < 3; i++)
    {
        __pid_t pid = fork();

        if (pid == 0)
        {
            printf("Filho criado com PID: %d\n", getpid());
            execlp("ls", "ls", "-l", (char *)NULL);
            printf("Troca de contexto executada com sucesso! (PID: %d)\n", getpid());
            fork();
        }
    }

    sleep(5);

    return 0;
}