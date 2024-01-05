#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *func1(void *karan)
{
    sleep(2);
    char *karanStr = karan;
    printf("Message from the thread 1 - %s\n", karanStr);
    printf("Thread 1 Id - %d\n", pthread_self());
}

void *func2(void *karan)
{
    char *karanStr = karan;
    printf("Message from the thread 2 - %s\n", karanStr);
    printf("Thread 2 Id - %d\n", pthread_self());
}

void *func3(void *karan)
{
    sleep(1);
    char *karanStr = karan;
    printf("Message from the thread 3 - %s\n", karanStr);
    printf("Thread 3 Id - %d\n", pthread_self());
}

int main()
{
    char *msg = "Welcome to COMP8567"; // Msg to print
    pthread_t t1, t2, t3;              // Creating three threads variable

    pthread_create(&t1, NULL, &func1, msg); // Creating the thread 1
    pthread_create(&t2, NULL, &func2, msg); // Creating the thread 2
    pthread_create(&t3, NULL, &func3, msg); // Creating the thread 3

    pthread_join(t2, NULL); // Pausing t2
    pthread_join(t3, NULL); // Pausing t3
    pthread_join(t1, NULL); // Pausing t1

    printf("Main thread Id - %d\n", pthread_self());

    return 0;
}