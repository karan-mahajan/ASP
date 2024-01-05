#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int i = fork();
    if (i == 0)
    {
        exit(257);
    }
    else if (i < 0)
    {
    }
    else
    {
        int status;
        int cid = wait(&status);
        if (WIFEXITED(status))
        {
            printf("Normal Exit: %d \n", WEXITSTATUS(status));
        }
        else // WIFSIGNALED(status) is true
        {
            printf("Signalled Exit: %d\n", WTERMSIG(status));
        }
    }
}