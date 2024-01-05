#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
// f44.c

int main()
{

    int pid1, pid2;
    pid1 = fork();
    pid2 = fork();

    if (pid1 > 0 && pid2 > 0)
    {
        printf("\nMain Process will now go into wait: %d %d\n", getpid(), getppid());
        int status;
        int child_pid = waitpid(pid1, &status, 0);
        printf("\nThe main process resumed execution and was waiting for : %d\n", child_pid);

        if (WIFEXITED(status))
        {
            printf("Normal Exit: %d \n", WEXITSTATUS(status));
        }
        else // WIFSIGNALED(status) is true
        {
            printf("Signalled Exit: %d\n", WTERMSIG(status));
        }
    }

    if (pid1 == 0 && pid2 > 0)
    {
        printf("\nFirst child: %d %d\n", getpid(), getppid());
        for (int i = 0; i < 50; i++)
        {
            sleep(1);
            printf("\nMessage from the first child\n");
        }
    }

    if (pid1 > 0 && pid2 == 0)
    {
        printf("\nSecond child: %d %d\n", getpid(), getppid());
    }

    if (pid1 == 0 && pid2 == 0)
    {
        printf("\nGrandchild Process:%d %d\n", getpid(), getppid());
    }

    return 0;
}
