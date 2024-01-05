#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

    int i, j;
    i = fork();
    // first child starts
    j = fork();
    // second and grand child
    if (i > 0 && j > 0)
    {
        printf("\nThe PID and PPID of the main process is : %d %d\n", getpid(), getppid());
    }
    if (i == 0 && j > 0)
    {
        printf("\nThe PID and PPID of the first child of the main process is : %d %d\n", getpid(), getppid());
    }
    if (i > 0 && j == 0)
    {
        printf("\nThe PID and PPID of the second child of the main process is : %d %d\n", getpid(), getppid());
    }
    if (i == 0 && j == 0)
    {
        printf("\nThe PID and PPID of the grand-child process is : %d %d\n", getpid(), getppid());
    }
    return 0;
}