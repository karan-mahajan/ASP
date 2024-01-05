#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int i;
    i = fork();
    if (i == 0)
    {
        // Child process
        int input;
        printf("\nEnter the input between 1 - 3 : ");
        scanf("%d", &input);
        if (input == 1)
        {
            printf("\nThe PID and PPID of the child is %d %d\n", getpid(), getppid());
        }
        else if (input == 2)
        {
            printf("\nThe PID and PPID of the child is %d %d\n", getpid(), getppid());
            exit(7);
        }
        else if (input == 3)
        {
            for (int i = 1; i <= 3; i++)
            {

                printf("\nThe PID and PPID of the child is %d %d\n", getpid(), getppid());
                sleep(1);
            }
            int num = 100 / 0;
        }
    }
    else if (i < 0)
    {
        printf("Error");
    }
    else
    {
        // Parent process
        int status;
        int cid = wait(&status);
        printf("\nParent resumed with the execution\n");
        if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status))
            {
                printf("\nExit from the child using exit(): %d \n", WEXITSTATUS(status));
            }
            else
            {
                printf("\nNormal Exit from the child : %d\n", WEXITSTATUS(status));
            }
        }

        if (WIFSIGNALED(status))
        {
            printf("\nAbnormal Exit from the child: %d \n", WTERMSIG(status));
        }
    }
    return 0;
}