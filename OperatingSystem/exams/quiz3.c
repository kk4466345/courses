#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
   int pid, fd[2], x, y;
   if(pipe(fd) < 0)
        perror("pipe");
   dup2(fd[0], 0); dup2(fd[1], 1);
   pid = fork();
   if(!pid){ //child
       scanf("%d", &y);
       fprintf(stderr, "child reads %d\n", y);
       printf("%d\n", y*y);
       fflush(stdout);
       exit(0);
   }else{
       printf("%d\n", 5);
       fflush(stdout);
       wait(NULL);  //wait for the child
       scanf("%d", &x);
       fprintf(stderr, "parent reads %d\n", x);
   }
}
/*
    (a) What is the output? (b) What is/are the possible output(s) if wait(NULL) is removed?
*/