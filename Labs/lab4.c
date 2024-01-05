#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
        int status;
        int cid = wait(&status);
        // If normal exit
        if (WIFEXITED(status))
        {
            printf("\nExecuting the command ls - /home/mahaja81/lab4 using execv\n");
            char *buff[] = {"/bin/ls", "/bin/ls", "-l", "/home/mahaja81/lab4", NULL};
            execv("/bin/ls", buff);
        }
    }
    if (i > 0 && j == 0)
    {
        printf("\nThe PID and PPID of the second child of the main process is : %d %d\n", getpid(), getppid());
        char *commandName = "/bin/ls";
        char *argv1 = "-1";
        char *argv2 = "/home/mahaja81";
        printf("\nExecuting the command ls - /home/mahaja81 using execl\n");
        execl(commandName, commandName, argv1, argv2, NULL);
    }
    if (i == 0 && j == 0)
    {
        char *buff;
        printf("\nThe PID and PPID of the grand-child process is : %d %d\n", getpid(), getppid());
        printf("\nCurrent Directory for grand child : %s\n", getcwd(buff, 50));
        printf("\nChanging the directory\n");
        int d = chdir("/home/mahaja81/lab4");
        if (d == -1)
        {
            printf("\nNot Able to change the directory\n");
        }
        else
        {
            printf("\nDirectory changed to : /home/mahaja81/lab4\n");
        }
        // Changing the permisson to 0777
        umask(0000);
        int fd = open("sample.txt", O_CREAT, 0777);
        if (fd == -1)
        {
            printf("\nNot Able to create the file\n");
        }
        else
        {
            printf("\nSample.txt File created\n");
        }
    }
    return 0;
}