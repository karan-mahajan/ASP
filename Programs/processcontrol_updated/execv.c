#include <unistd.h>
#include <stdio.h>

int main(void)
{

  printf("\nThe program will be replaced by 'ls -1 /Users/karanmahajan/University/2nd-Sem/ASP/Programs/processcontrol_updated'  using execv()\n");

  char *programName = "/bin/ls";
  char *args[] = {"/bin/ls", "-lah", "/Users/karanmahajan/University/2nd-Sem/ASP/Programs/processcontrol_updated", NULL};
  // Note: ls has two arguments in this example

  execv(programName, args);

  printf("\nThe program successfully completed\n");
  return 0;
}
