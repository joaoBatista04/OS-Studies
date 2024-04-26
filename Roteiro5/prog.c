#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

volatile sig_atomic_t sigusr1_received_a = FALSE;
volatile sig_atomic_t sigusr1_received_b = FALSE;

void handle_sigusr1_a(int signal)
{
    sigusr1_received_a = TRUE;
    printf("[%d]-PING\n", getpid());
    sleep(1);
    kill(getppid(), SIGUSR1); // Envie o sinal SIGUSR1 para o Pai
}

void handle_sigusr1_b(int signal)
{
    sigusr1_received_b = TRUE;
    printf("[%d]-PONG\n", getpid());
    sleep(1);
    kill(getppid(), SIGUSR1); // Envie o sinal SIGUSR1 para o Pai
}

void handle_sigchld(int signal)
{
    pid_t pid;
    int status;

    // Esperar que um filho termine
    pid = wait(&status);
    if (pid > 0)
    {
        if (pid == getpid())
        { // Pai terminou
            if (sigusr1_received_a)
                kill(SIGKILL, pid); // Mata o filho A
            else if (sigusr1_received_b)
                kill(SIGKILL, pid); // Mata o filho B
        }
        else
        {
            // Filho terminou, então mata o outro filho e finaliza
            if (pid != getpid())         // Se o PID retornado for diferente do PID do processo atual
                kill(SIGKILL, getpid()); // Mata o processo atual (Pai)
        }
    }
}

int main()
{
    struct sigaction sa_a, sa_b, sa_chld;
    pid_t pid_a, pid_b;
    int status;

    // Configurando o tratador de SIGUSR1 para o Filho A
    sa_a.sa_handler = handle_sigusr1_a;
    sigemptyset(&sa_a.sa_mask);
    sa_a.sa_flags = 0;
    sigaction(SIGUSR1, &sa_a, NULL);

    // Configurando o tratador de SIGUSR1 para o Filho B
    sa_b.sa_handler = handle_sigusr1_b;
    sigemptyset(&sa_b.sa_mask);
    sa_b.sa_flags = 0;
    sigaction(SIGUSR1, &sa_b, NULL);

    // Configurando o tratador de SIGCHLD para o Pai
    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    sigaction(SIGCHLD, &sa_chld, NULL);

    // Criando o Filho A
    pid_a = fork();
    if (pid_a == 0)
    {
        // Código do Filho A
        while (!sigusr1_received_a)
        {
            // Fica bloqueado à espera do sinal SIGUSR1
            pause();
        }
        exit(EXIT_SUCCESS);
    }

    // Criando o Filho B
    pid_b = fork();
    if (pid_b == 0)
    {
        // Código do Filho B
        while (!sigusr1_received_b)
        {
            // Fica bloqueado à espera do sinal SIGUSR1
            pause();
        }
        exit(EXIT_SUCCESS);
    }

    // Código do Pai
    // Enviar sinal SIGUSR1 para o Filho A
    kill(pid_a, SIGUSR1);

    // Fica bloqueado aguardando que um dos filhos termine
    pause();

    return 0;
}
