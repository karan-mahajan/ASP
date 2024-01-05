#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int p;
    p = fork();

    if (p == -1)
    {
        printf("There is an error while calling fork()");
    }
    if (p == 0)
    {
        printf("\nWe are in the child process\n");
        printf("\nPID - PPID %d %d", getpid(), getppid());
        printf("\nThe child process is now being replaced by another executable using exec()\n");
        int i = execl("/bin/ls", "bin/ls", "-la", NULL);
        printf("\n I = %d\n", i);

        printf("\nWe are in the child process\n");
        exit(0);
    }
    else
    {
        int status;
        int k = wait(&status);
        printf("\nWe are in the parent process %d\n", k);
        exit(0);
    }
    return 0;
}
