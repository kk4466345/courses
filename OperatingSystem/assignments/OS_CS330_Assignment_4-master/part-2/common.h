#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct rwlock_t{
                            long value;
                       }rwlock_t;

static int atomic_add(long *ptr, long val) __attribute__((noinline));


/*
  Return value is as follows,
   0 ==> result is zero
   1 ==> result is positive
   -1 ==> result is negative 
*/
static int atomic_add(long *ptr, long val)
{
       int ret = 0;
       asm volatile( 
                     "lock add %%rsi, (%%rdi);"
                     "pushf;"
                     "pop %%rax;" 
                     "movl %%eax, %0;"
                     : "=r" (ret)
                     : 
                     : "memory", "rax"
                    );

     
     if(ret & 0x80)
               ret = -1;
     else if(ret & 0x40)
               ret = 0;
     else
               ret = 1;
     return ret;
}

extern void init_rwlock(rwlock_t *);
extern void write_lock(rwlock_t *);
extern void write_unlock(rwlock_t *);
extern void read_lock(rwlock_t *);
extern void read_unlock(rwlock_t *);
#endif
