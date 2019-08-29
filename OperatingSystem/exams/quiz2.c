#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sighandler1(int);
void sighandler2(int);
void (*handler)(int);   // Function pointer

void sighandler1(int signo)
{
   handler = &sighandler2;
   printf("I am SIGINT in handler 1\n");
}

void sighandler2(int signo)
{
   handler = &sighandler1;
   printf("I am SIGINT in handler 2\n");
}

int main()
{
   handler = &sighandler1;   // Assign function pointer
   if(signal(SIGINT, handler) < 0)
        perror("signal");

    while(1)
      ;
}

/*
    What will be the output of the above program if SIGINT is sent three (3) times 
    from another program using kill() system call?
*/