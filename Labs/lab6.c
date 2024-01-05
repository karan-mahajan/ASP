#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int i, j;

void handleAlarm(int status)
{
    kill(getpid(), SIGCONT);
}
void handleCtrlC(int status)
{
    kill(i, SIGSTOP);
    kill(j, SIGSTOP);
    alarm(2);
    pause();
}

void handleContinue(int status)
{
    // i = first child and j = second child
    printf("\nThis is from process : %d\n", getpid());
    sleep(2);
    kill(i, SIGCONT);        // Resume the first child
    kill(getpid(), SIGSTOP); // Stop the current process
}

void handleFirstContinue(int status)
{
    printf("\nThis is from process : %d\n", getpid());
    sleep(2);
    kill(j - 1, SIGCONT);    // Resume the second child
    kill(getpid(), SIGSTOP); // Stop the current process
}

void handleSecondContinue(int status)
{
    printf("\nThis is from process : %d\n", getpid());
    sleep(2);
    kill(getppid(), SIGCONT); // Resume the parent process
    kill(getpid(), SIGSTOP);  // Stop the current process
}

int main()
{
    i = fork();
    j = fork();
    if (i == 0 && j > 0)
    {
        // First child process
        signal(SIGINT, handleCtrlC);
        signal(SIGCONT, handleFirstContinue);
        setpgid(0, 0);
        int status;
        while (1)
        {
            int d = waitpid(-1, &status, WNOHANG);
            if (d > 0)
            {
                kill(getppid(), SIGSTOP);
                kill(getpid() + 1, SIGSTOP);
                kill(getppid(), SIGCONT);
                kill(getpid(), SIGSTOP);
            }
            printf("\nFrom first child : %d\n", getpid());
            sleep(2);
        }
    }
    if (i > 0 && j == 0)
    {
        // second child process
        signal(SIGINT, handleCtrlC);
        signal(SIGCONT, handleSecondContinue);
        setpgid(0, 0);
        while (1)
        {
            printf("\nFrom second child : %d\n", getpid());
            sleep(2);
        }
    }
    if (i == 0 && j == 0)
    {
        // grand child process
        while (1)
        {
            printf("\nFrom grand child : %d\n", getpid());
            sleep(2);
        }
    }
    if (i > 0 && j > 0)
    {
        // main process
        signal(SIGCONT, handleContinue);
        signal(SIGINT, handleCtrlC);
        signal(SIGALRM, handleAlarm);
        setpgid(0, 0);
        while (1)
        {
            printf("\nFrom main process : %d\n", getpid());
            sleep(2);
        }
    }
    return 0;
}