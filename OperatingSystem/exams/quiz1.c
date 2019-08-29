#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

main()
{
    int value = 3, sum = 0;
    pid_t pid;

    while(value){
       pid = fork();
       if(pid < 0){     /*This never happens*/
            perror("fork");
            exit(-1);
        }
       if(pid == 0)
             sum += value;
       value--;
   }

   printf("sum = %d\n", sum);
}