#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Implementation of cat 'f23.txt | grep Welcome | wc' using pipes
int main()
{
    int pipe1[2]; // For storing and input of cat and grep
    int pipe2[2]; // For storing and input of grep and wc
    if (pipe(pipe1) < 0)
    {
        exit(EXIT_FAILURE);
    }
    int pidFork = fork();
    if (pidFork > 0) // Parent
    {
        close(pipe1[0]);
        dup2(pipe1[1], 1); // Redirecting the output from bash to pipe1[1]
        execlp("cat", "cat", "f23.txt", NULL);
    }
    else if (pidFork < 0)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        // Child
        close(pipe1[1]);
        dup2(pipe1[0], 0); // Redirecting the input from bash to pipe1[0]
        if (pipe(pipe2) < 0)
        {
            exit(EXIT_FAILURE); // if pipe failed
        }
        int pidForkChild = fork();
        if (pidForkChild > 0) // Parent
        {
            close(pipe2[0]);
            dup2(pipe2[1], 1); // Redirecting the output from bash to pipe2[1]
            execlp("grep", "grep", "Welcome", NULL);
        }
        else if (pidForkChild < 0)
        {
            exit(EXIT_FAILURE); // if pipe failed
        }
        else
        {
            close(pipe2[1]);
            dup2(pipe2[0], 0);                      // Redirecting the input from bash to pipe2[0]
            freopen("lab7output.txt", "w", stdout); // For storing the ouput of wc into lab7output.txt
            execlp("wc", "wc", NULL);
        }
    }
    return 0;
}
