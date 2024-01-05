//redirect.c
// Demonstrates I/O redirection with dup2() 

//dup2() system call for I/O redirection
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(void) {

  int number1, number2, sum;
  int fdx = open("filex.txt", O_RDONLY);   //scanf reads from this file
  int fdy = open("filey.txt", O_RDWR);  //printf writes into this file
  
  int ret1= dup2(fdx,0); //standard input redirection    
     if(ret1 < 0) {
  	  printf("Unable to duplicate the STDIN file descriptor.");
   	 exit(EXIT_FAILURE);  }

   int ret2=dup2(fdy,1); // standard output redirection
    
  if(ret2 < 0) {
  	  printf("Unable to duplicate the STDOUT file descriptor.");
  	  exit(EXIT_FAILURE);}

  scanf("%d %d", &number1, &number2);  //reads number 1 and number2 from input.txt 
  sum = number1 + number2;
  printf("The sum of two numbers is\n");  // writes into filex.txt 
  printf("%d + %d = %d\n", number1, number2, sum);  // writes into filey.txt 
  
  
  return EXIT_SUCCESS;
} //End main

