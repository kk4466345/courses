#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<assert.h>

#define THREADS 32
#define HASH_TABLE_MAX 100000001UL

enum{
        PUT,
        GET,
        DEL,
        MAX_OPS    
};

typedef struct hash_entry_t{
                              unsigned id;
                              unsigned datalen;
                              unsigned long key;
                              char *data;
                              pthread_mutex_t lock;
}hash_entry_t;

typedef struct hashtable_t{
                             unsigned table_size;
                             unsigned used;
                             hash_entry_t *table;
}hash_t;

typedef struct operation_t{
                              unsigned long id;
                              unsigned long key;
                              unsigned op_type;
                              unsigned datalen;
                              char *data;
}op_t;

struct input_manager{
                         int fd;
                         char *data;
                         char *curr;
                         unsigned long size;
                         op_t *being_processed[THREADS];  //Currently processed keys
                         pthread_mutex_t lock;
                         pthread_cond_t  cond;
                    };
                               
static unsigned hashfunc(unsigned long key, unsigned size)
{
   unsigned long hashval = key >> 32;
   hashval += key & 0xffffffff;
   return(hashval % size);             
}

static void init_hashtable(hash_t *h, unsigned size)
{
    assert(h);
    h->used = 0;
    h->table_size = size;
    h->table = (hash_entry_t *)malloc(size * sizeof(hash_entry_t));
    memset((void *)h->table, 0, size * sizeof(hash_entry_t));
    return;
}

static int init_input_manager(struct input_manager *in, char *filename)
{
     struct stat sbuf;

     assert(in);
     in->fd = open(filename, O_RDWR);

     if(in->fd < 0){
           printf("Can not open file\n");
           return -EINVAL;
     } 

     if(fstat(in->fd, &sbuf) < 0){  
         perror("stat");
         return -EINVAL;
     }
     
     in->size = sbuf.st_size;;
  
    // MMAP the file into memory and prepopulate them
    in->data = mmap(NULL, in->size, PROT_READ, MAP_SHARED, in->fd, 0);
    if(in->data == MAP_FAILED){
       perror("mmap");
       close(in->fd);
       return -ENOMEM;
    }

    in->curr = in->data;
    pthread_mutex_init(&in->lock, NULL);
    pthread_cond_init(&in->cond, NULL);

    return 0;
}

static void dump_hash_table(hash_t *h)
{
  
   unsigned ctr, lused;
   hash_entry_t *entry = h->table;   
   printf("total entries: %u used entries: %u\n", h->table_size, h->used);  
   lused = h->used;
   
   for(ctr=0; ctr < h->table_size; ++ctr){
       if(entry->key){
              printf("id: %u key=%lu datalen=%u ptr=%p\n", entry->id, entry->key, entry->datalen, entry->data);
              lused--;
       }
       entry++;
   }
  assert(lused == 0);
  return;
}
extern int read_op(struct input_manager *in, op_t *op, int tnum);   //Fills up the op and returns
extern void done_one(struct input_manager *in, int tnum);   //Processed one request

extern int lookup(hash_t *h, op_t *op);   //Fills up the op with lookedup value
extern int insert_update(hash_t *h, op_t *op);       //Insert or update hashtable entry

extern int purge_key(hash_t *h, op_t *op);            //Delete an entry hashtable
#endif
