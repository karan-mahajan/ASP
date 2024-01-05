#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
    umask(0000);
    int isOpen = open("sample.txt", O_CREAT | O_RDWR, 0777);
    char buff[40];
    for (int i = 0; i < 40; i++)
    {
        buff[i] = 'T';
    }
    long int n;
    n = write(isOpen, buff, 40);
    n = lseek(isOpen, 10, SEEK_SET);
    char *buff1 = "COMP 8567";
    char *buff2 = "ASP";
    char *buff3 = "University of Windsor";
    n = write(isOpen, buff1, strlen(buff1));
    n = lseek(isOpen, 10, SEEK_CUR);
    n = write(isOpen, buff2, strlen(buff2));
    n = lseek(isOpen, 15, SEEK_END);
    n = write(isOpen, buff3, strlen(buff3));
    close(isOpen);
    int openAgain = open("sample.txt", O_RDONLY);
    char values[100];
    long int m;
    m = lseek(openAgain, 0, SEEK_SET);
    m = read(openAgain, values, 100);
    int count = 0;
    for (int i = 0; i < strlen(values); i++)
    {
        if (values[i] == ' ')
        {
            values[i] = '#';
            count++;
        }
    }
    m = write(openAgain, values, strlen(values));
    printf("\nNumber of null characters replaced are : %d\n", count);
    close(isOpen);
    return 0;
}