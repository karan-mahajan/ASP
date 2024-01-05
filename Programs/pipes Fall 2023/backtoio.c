#include <stdio.h>   //backtoio.h 
#include <stdlib.h>   
#include <unistd.h>
#include <fcntl.h>
 //Redirection and reversal 
int main(void) {
 int number1, number2, sum;
 int fdx = open("filex.txt", O_RDONLY); 
  int fdy = open("filey.txt", O_RDWR); 
  
  int cp1=dup(1);
  printf("cp1:%d\n",cp1); 
  int cp0=dup(0); 
  printf("cp0:%d\n",cp0); 

  int ret1= dup2(fdx,0); //0 is the fd of standard input 
   if(ret1 < 0) {
    printf("Unable to duplicate the STDIN file descriptor.");
    exit(EXIT_FAILURE);
  }
  
 int ret2= dup2(fdy, 1); 
if(ret2 < 0) {
    printf("Unable to duplicate the STDOUT file descriptor.");
    exit(EXIT_FAILURE);
  }
  scanf("%d %d", &number1, &number2);
  sum = number1 + number2;
  printf("\nThe sum of two numbers is\n"); 

  printf("%d + %d = %d\n", number1, number2, sum);
  fflush(stdout);
  
// Reversing the redirection

int retval1= dup2(cp0,0);
int retval2=dup2(cp1,1);

printf("\nBack to standard input-output, enter the value of num\n");
fflush(stdout); 
fflush(stdin); 
 int num;
 scanf("%d",&num); 

  return EXIT_SUCCESS;
} //end main 

