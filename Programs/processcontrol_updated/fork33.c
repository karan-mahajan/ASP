#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// fork33.c

int main(int argc, char *argv[])
{
    int pid;
    pid = fork();
    pid = fork();
    pid = fork();

    if (pid == 0)
    {
        for (;;)
            ;
    }
    else if (pid < 0)
    {
        printf("Error Forking");
    }
    else
    {
        for (;;)
            ;
    }
}
