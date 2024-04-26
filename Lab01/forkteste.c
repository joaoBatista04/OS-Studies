#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int c1, c2;

    c2 = 0;
    c1 = fork();

    if (c1 == 0)
    {
        c2 = fork();
    }

    fork();

    if (c2 > 0)
    {
        fork();
    }

    sleep(5);
    printf("NODE: %ld, c1: %d, c2: %d\n", (long)getpid(), c1, c2);
    exit(1);

    return 0;
}