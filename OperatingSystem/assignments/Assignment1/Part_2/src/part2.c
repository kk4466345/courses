#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
int pi;
int pipe1[2];
int pipe2[2];

int main(int argc,char* argv[])
{
    if(strcmp(argv[1],"@") == 0) {
        int fd[2];
        pipe(fd);

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            printf("error in pipe\n");
            exit(1);
        }

        if (pid == 0) {
            dup2(fd[0], 0);
            close(fd[1]);
            execlp("wc", "wc","-l", NULL);
            perror("Error from parent::");
            exit(1);
        } else {

            dup2(fd[1], 1);
            close(fd[0]);
            // printf("fjre\n");
            execlp("grep", "grep","-rF", argv[2], argv[3], NULL);
            perror("Error from child::");
            exit(1);
        }
    } else if(strcmp(argv[1],"$") == 0) {
        // printf("enter \t %d\n\n\\n",argc);
        if (pipe(pipe1) == -1) {
            perror("bad pipe1");
            exit(1);
          }

          // fork (ps aux)
          if ((pi = fork()) == -1) {
            perror("bad fork1");
            exit(1);
          } else if (pi == 0) {
            // stdin --> ps --> pipe1
            exec1(argv[2],argv[3]);
          }
          // parent

          // create pipe2
          if (pipe(pipe2) == -1) {
            perror("bad pipe2");
            exit(1);
          }

          // fork (grep root)
          if ((pi = fork()) == -1) {
            perror("bad fork2");
            exit(1);
          } else if (pi == 0) {
            // pipe1 --> grep --> pipe2
            exec2(argv[4]);
          }
          // parent

          // close unused fds
          close(pipe1[0]);
          close(pipe1[1]);

          // fork (grep sbin)
          if ((pi = fork()) == -1) {
            perror("bad fork3");
            exit(1);
          } else if (pi == 0) {
            // pipe2 --> grep --> stdout
            if(argc == 6) {
                exec3(argv[5]);
            } else if(argc == 7) {
                exec4(argv[5],argv[6]);
            }
          }

    }

    return 0;
}

void exec1(char *str,char *path) {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  execlp("grep", "grep","-rF",str, path, NULL);
  // exec didn't work, exit
  perror("Error in exec 1:");
  _exit(1);
}

void exec2(char *str) {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("tee", "tee", str, NULL);
  // exec didn't work, exit
  perror("Error in exec2::");
  _exit(1);
}

void exec3(char *kk) {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp(kk, kk, NULL);
  // exec didn't work, exit
  perror("bad exec grep sbin\n\n\n\nn\n\n\n");
  _exit(1);
}

void exec4(char *str,char *flag) {
    // printf("%s\n",flag );
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp(str, str,flag, NULL);
  // exec didn't work, exit
  perror("bad exec grep sbin\n\n\n\nn\n\n\n");
  _exit(1);
}
