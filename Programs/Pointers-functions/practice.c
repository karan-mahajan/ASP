#include <stdio.h>

int main(void)
{
    int ar[4] = {15, 90, 115, 120};
    int *ptr;
    ptr = &ar[1];
    int *ptr1;
    ptr1 = &ar[3];
    printf("%d %d", ptr1 - 2, &ar[1]);
    return (0);
}