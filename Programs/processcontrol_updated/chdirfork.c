#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int p, p1;
    p = fork();

    if (p == -1)
    {
        printf("There is an error while calling fork()");
    }
    if (p == 0)
    {
        printf("\nWe are in the child process\n");
        char *s = malloc(50 * sizeof(char));
        // current working directory
        printf("\nThe current working directory of the child process is %s\n", getcwd(s, 50));
        // using the system call chdir()
        chdir("/home/karanmahajan/University");

        printf("\nThe new working directory of the child process is %s\n", getcwd(s, 50));
        // new working directory

        exit(0);
    }
    else
    {
        int status;
        int k = wait(&status);
        printf("\nParent process resumes\n");
        char *s;
        exit(0);
    }
    return 0;
}
