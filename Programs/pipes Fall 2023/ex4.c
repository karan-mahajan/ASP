#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
 
 //equivalent of $ls|wc -w 

int main(int argc, char *argv[]){
int fd[2];

if(pipe(fd)==-1) 
exit(1);

if(fork() > 0)//parent 
{
close(fd[0]);//optional but recommended
dup2(fd[1], 1); 
execlp("ls","ls", NULL);
}
else 
{
close(fd[1]);//optional but recommended
dup2(fd[0], 0);
execlp("wc","wc","-w",NULL);
 
//execlp("wc","wc","-w", NULL);
}
}//End main
