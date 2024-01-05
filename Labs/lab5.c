#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <stdbool.h>

bool signalCalled = false;

void karanHandler(int status)
{
    // If the ctrl c is pressed for the first time
    if (!signalCalled)
    {
        printf("\nPress again in 5 seconds to terminate the program\n");
        signalCalled = true;
        alarm(5);
    }
    else
    {
        exit(0);
    }
}

void karanUpdateSigint(int status)
{
    // Updating to the default value after 5 seconds
    signalCalled = false;
}

int main()
{
    // Registering the sigint and sigalrm signals
    signal(SIGINT, karanHandler);
    signal(SIGALRM, karanUpdateSigint);
    for (;;)
    {
        printf("Welcome to Lab5 - Signals\n");
        sleep(1);
    }
    return 0;
}