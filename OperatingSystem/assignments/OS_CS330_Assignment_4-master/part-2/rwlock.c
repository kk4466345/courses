#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<common.h>

/* XXX NOTE XXX  
       Do not declare any static/global variables. Answers deviating from this 
       requirement will not be graded.
*/
void init_rwlock(rwlock_t *lock)
{
   /*Your code for lock initialization*/
}

void write_lock(rwlock_t *lock)
{
   /*Your code to acquire write lock*/
}

void write_unlock(rwlock_t *lock)
{
   /*Your code to release the write lock*/
}


void read_lock(rwlock_t *lock)
{
   /*Your code to acquire read lock*/
}

void read_unlock(rwlock_t *lock)
{
   /*Your code to release the read lock*/
}
