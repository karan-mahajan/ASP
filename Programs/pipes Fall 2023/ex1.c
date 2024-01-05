// C program to illustrate pipe() system call 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
char* msg1 = "Welcome to COMP 8567";

//As a simple example, the same process writes and reads from the pipe 
int main()
{
    char inbuf[10];
    int p[2], i;
    if (pipe(p) < 0)  //Invoke the pipe() system call 
        exit(1);
        
    //write msg1 into the pipe using the write FD p[1]
    int bw=write(p[1],msg1,20);
    printf("\nThe number of bytes written into the pipe is: %d\n",bw);
    
    //read the contents of the pipe into inbuf using the read FD p[0] 
    int br= read(p[0], inbuf, 10);
    printf("The contents of the pipe are\n%s", inbuf);
    printf("\nThe number of bytes read from the pipe is: %d\n",br);
    br= read(p[0], inbuf, 10);
    printf("The contents of the pipe are\n%s", inbuf);
    printf("\nThe number of bytes read from the pipe is: %d\n",br);

    
   return 0; 
}
