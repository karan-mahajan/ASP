#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t karanThreadId[10];
int num = 10;
pthread_mutex_t lock; // Mutex step1 - GLobal Declaration

void *decrementThreadCounter(void *state)
{
    pthread_mutex_lock(&lock); // Started the mutex lock to decrement in the thread
    pthread_t threadId = pthread_self();
    if (pthread_detach(threadId) == 0)
    {
        num = num - 1;
        printf("The thread Id is - %d\n", threadId);
    }
    pthread_mutex_unlock(&lock); // Removing the mutex lock
}

int main(void)
{
    int i = 0;
    int err;

    if (pthread_mutex_init(&lock, NULL) != 0) // started the Initialization the mutex lock
    {
        printf("\n Unable to initialize the mutex lock\n");
        exit(1);
    }

    pthread_attr_t att;
    pthread_attr_init(&att);
    pthread_attr_setdetachstate(&att, PTHREAD_CREATE_DETACHED);
    for (int i = 0; i < 10; i++)
    {
        int detachstate = 1;
        if ((i + 1) % 2 == 0)
        {
            int threadResponse = pthread_create(&(karanThreadId[i]), &att, decrementThreadCounter, &detachstate);
            if (threadResponse != 0)
            {
                printf("\nUnable to create the thread\n");
                exit(2);
            }
        }
        else
        {
            int threadResponse = pthread_create(&(karanThreadId[i]), NULL, decrementThreadCounter, NULL);
            if (threadResponse != 0)
            {
                printf("\nUnable to create the thread\n");
                exit(2);
            }
        }
    }

    pthread_join(karanThreadId[0], NULL);
    pthread_join(karanThreadId[1], NULL);
    pthread_join(karanThreadId[2], NULL);
    pthread_join(karanThreadId[3], NULL);
    pthread_join(karanThreadId[4], NULL);
    pthread_join(karanThreadId[5], NULL);
    pthread_join(karanThreadId[6], NULL);
    pthread_join(karanThreadId[7], NULL);
    pthread_join(karanThreadId[8], NULL);
    pthread_join(karanThreadId[9], NULL);

    pthread_attr_destroy(&att);

    pthread_mutex_destroy(&lock); // Mutex lock Destroyed at the end

    printf("The final value of num variable is %d\n", num);

    return 0;
}
