#ifndef _COMMON_H_
#define _COMMON_H_


#define THREADS 32
#define BLOCK_SIZE 64

extern char *dataptr;
extern unsigned long *optr;
extern pthread_mutex_t lock;

extern void* hashit(void *arg);
extern unsigned long calculate_and_store_hash(char *ptr, char *endptr);
#endif
