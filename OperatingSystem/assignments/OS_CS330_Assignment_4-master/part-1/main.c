#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<pthread.h>

#include "common.h"

pthread_mutex_t lock;
char *dataptr = NULL;
unsigned long *optr = NULL;

unsigned long calculate_and_store_hash(char *ptr, char *endptr)
{
    unsigned long hashval = 0;
    int count = BLOCK_SIZE/sizeof(unsigned long);
    while(count--){
         if((endptr - ptr) < sizeof(unsigned long))
                break;
         hashval += *((unsigned long *)ptr);
         ptr += sizeof(unsigned long);
    }
   return hashval;
}


int main(int argc, char **argv)
{
     int fd, ctr, num_threads;
     unsigned long size, hash_count;
     char *buff;
     unsigned long *hashes;
     pthread_t threads[THREADS];

     struct stat sbuf;
     if(argc != 3){
            printf("Usage: %s <fileneme> <#threads>\n", argv[0]);
            exit(-1);         
      }  
     num_threads = atoi(argv[2]);
     if(num_threads <=0 || num_threads > THREADS){
            printf("Usage: #of threads must be between 1-%d\n", THREADS);
            exit(-1);         
     }
     fd = open(argv[1], O_RDONLY);
     if(fd < 0){
           printf("Can not open file\n");
           exit(-1);
     } 

     
     if(fstat(fd, &sbuf) < 0){  
         perror("stat");
         exit(-1);
     }
     
    size = sbuf.st_size;;
  
    // MMAP the file into memory and prepopulate them
    buff = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
    if(buff == MAP_FAILED){
       perror("mmap");
       close(fd);
       exit(-1);
    }
    // At this point buff points to the start of the file 
    hash_count = size / BLOCK_SIZE;
    if(size % BLOCK_SIZE)
          ++hash_count;

    hashes = malloc(sizeof(unsigned long) * hash_count);
    if(!hashes){
           perror("mem");
           exit(-1);
    }   
     
    dataptr = buff;
    optr = hashes; 

    pthread_mutex_init(&lock, NULL);
     for(ctr=0; ctr < num_threads; ++ctr){
        if(pthread_create(&threads[ctr], NULL, hashit, buff + size) != 0){
              perror("pthread_create");
              exit(-1);
        }
     }
     
     for(ctr=0; ctr < num_threads; ++ctr)
            pthread_join(threads[ctr], NULL);
 
    
     for(ctr=0; ctr < hash_count; ++ctr)
           printf("block# %d hash %lx\n", ctr, hashes[ctr]);  
     
     free((void *)hashes);
     munmap(buff, size);
     close(fd);
}
