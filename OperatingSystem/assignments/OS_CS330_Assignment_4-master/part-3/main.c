#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<pthread.h>

#include "common.h"


struct thread_data{
                      int tnum;
                      pthread_t tid;
                      hash_t *h;
                      struct input_manager *in;
};

void *process(void *arg)
/*Argument is the end pointer*/
{
   struct thread_data *td = (struct thread_data *)arg;
   struct operation_t *op = malloc(sizeof(struct operation_t));;
  
   int outfd;
   char filename[32];
   char opbuf[256];

   assert(op);
   sprintf(filename, "thread-%d.out", td->tnum);

   outfd = open(filename, O_RDWR|O_CREAT, 0644);
   assert(outfd >= 0);
   assert(ftruncate(outfd, 0) == 0);
   
   while(read_op(td->in, op, td->tnum) == 0){
        int bufctr = 0, ret = 0;
        bufctr += sprintf(opbuf, "%lu %u ", op->id, op->op_type);

        if(op->op_type == GET)
              ret = lookup(td->h, op);
        else if(op->op_type == PUT)
              ret = insert_update(td->h, op);
        else if(op->op_type == DEL)
              ret = purge_key(td->h, op);
        else
               assert(0);

        done_one(td->in, td->tnum); 

        bufctr += sprintf(opbuf + bufctr, "%d\n", ret);
        assert(write(outfd, opbuf, bufctr) == bufctr);

   }

  free(op);
  close(outfd);
  pthread_exit(NULL); 
}

int main(int argc, char **argv)
{
     int ctr, num_threads;
     unsigned long hash_table_size;

     struct input_manager *in;
     hash_t *h;
     struct thread_data *threads;

     if(argc != 4){
            printf("Usage: %s <fileneme> <hash_table_entries> <#threads>\n", argv[0]);
            exit(-1);         
      }  
      
          
     in = malloc(sizeof(struct input_manager));
     assert(in);
     memset(in, 0, sizeof(struct input_manager));

     assert(init_input_manager(in, argv[1]) == 0);

     hash_table_size = atol(argv[2]);
     assert(hash_table_size <= HASH_TABLE_MAX);
   
     h = malloc(sizeof(hash_t));
     init_hashtable(h, hash_table_size);
      
     num_threads = atoi(argv[3]);
     
     assert(num_threads > 0 && num_threads <= THREADS);
    
     threads = malloc(num_threads * sizeof(struct thread_data));
      
     /*Create threads*/
     for(ctr=0; ctr < num_threads; ++ctr){
           struct thread_data *td = threads + ctr;
           td->tnum = ctr + 1;
           td->h = h;
           td->in = in; 
           if(pthread_create(&td->tid, NULL, process, (void *)td) != 0){
                 perror("pthread_create");
                 exit(-1);
           }
      }

     /*Wait for threads to finish their execution*/
     for(ctr=0; ctr < num_threads; ++ctr){
           struct thread_data *td = threads + ctr;
           pthread_join(td->tid, NULL);
     }

  free(threads);
  dump_hash_table(h);
}
