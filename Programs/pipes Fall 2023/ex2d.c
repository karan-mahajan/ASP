#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
int fd[2];
int k=pipe(fd);//create a pipe
if(k == -1)
exit(1); 

int pid=fork(); 

if(pid>0)// Parent Process 
{
char *message="Hello child process!";
int n=write(fd[1], message,strlen(message));
printf("\nThe number of characters written into the pipe is %d\n",n); 
}
else
{
char ch;
char *buff1;
printf("\nFrom the Child Process:Parent has sent the following message:");
int n=read(fd[0],buff1,20); 
printf("\n%s",buff1);
printf("\nThe number of characters read is %d\n",n);

}
exit(0);
}


