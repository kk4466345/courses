#include<pthread.h>
#include "common.h"

/*TODO:  
     Insert approproate synchronization statements
     to make it work with multiple threads
*/
void *hashit(void *arg)
/*Argument is the end pointer*/
{
   char *cptr;
   unsigned long *chash;
   char *endptr = (char *)arg;   // END pointer

   while(1){
        if(dataptr >= endptr){
              break;
        }
        cptr = dataptr;
        dataptr += BLOCK_SIZE;
        
        chash = optr;
        optr++;
   
        /*   Perform the real calculation. The following line should not be inside any locks*/
        *chash = calculate_and_store_hash(cptr, endptr); 
  }
  pthread_exit(NULL); 
}
