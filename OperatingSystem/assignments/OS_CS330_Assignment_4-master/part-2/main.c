
/*
  This is a sample test to check the correctness of rwlock. You are required to test with your testcases 
  as much as possible to make sure that things are working as expected
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<common.h>
#define MAX_THREADS 64
#define ITERS 1000000UL
#define USAGE_EXIT(s) do{ \
                             printf("Usage: %s <# of threads> \n %s\n", argv[0], s); \
                            exit(-1);\
                    }while(0);



rwlock_t lock;

unsigned long wr_ctr = 0;
unsigned long rd_ctr = 0;

struct th_arg{
                 pthread_t tid;
                 int tnum;
};

void *thfunc(void *arg)
{
   unsigned long ctr;
   struct th_arg *th = (struct th_arg *)arg;
   for(ctr=0; ctr<ITERS; ++ctr){
            write_lock(&lock);
            wr_ctr += 5;
            write_unlock(&lock);
            read_lock(&lock);
            rd_ctr += 5;
            read_unlock(&lock);
   }
   pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  int num_threads, ctr;
  struct th_arg *threads;

  if(argc !=2)
           USAGE_EXIT("not enough parameters");


  num_threads = atoi(argv[1]);
  if(num_threads <=0 || num_threads > MAX_THREADS){
          USAGE_EXIT("invalid num of threads");
  }

  init_rwlock(&lock);

  threads = malloc(num_threads * sizeof(struct th_arg));
 
  /*Create threads*/
  for(ctr=0; ctr < num_threads; ++ctr){
        struct th_arg *th = threads + ctr;
        th->tnum = ctr + 1;
        if(pthread_create(&th->tid, NULL, thfunc, (void *)th) != 0){
              perror("pthread_create");
              exit(-1);
        }

  }

  /*Wait for threads to finish their execution*/
  for(ctr=0; ctr < num_threads; ++ctr){
        struct th_arg *th = threads + ctr;
        pthread_join(th->tid, NULL);
  }

  free(threads);
  printf("rd_ctr = %ld wr_ctr = %ld (should be %ld)\n", rd_ctr, wr_ctr, 5 * ITERS * num_threads);
}

