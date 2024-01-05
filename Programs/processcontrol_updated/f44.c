#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
// f44.c

int main()
{

    int a;
    a = fork();
    a = fork();
    a = fork();

    printf("\nProcess id = %d, Parent id= %d\n", getpid(), getppid());
    return 0;
}
