#include<stdio.h>
#include<stdlib.h> 
#include<unistd.h> 
#include<assert.h> 
   
#define SZ (1 << 15) 
   
int func(int val)
{
   if(val == 0){
       void *sp;
       asm volatile("mov %%rsp, %0;" 
                     :"=r"(sp)
                     ::"memory");
       printf("SP = %p\n", sp);
       return 1;
   }
   val *= func(val-1);
   return val;
}  

int main()
{
   int result;
   void *oldbrk, *address, *oldsp;
   oldbrk = sbrk(SZ);
   assert(oldbrk != (void *)(-1));
   address = oldbrk + SZ;
    
   asm volatile("mov %%rsp, %0;"     // Move the value in stack pointer (RSP) to oldsp
                    "mov %1, %%rsp;" // Move the value in address to stack pointer (RSP)
                    :"=m" (oldsp)
                    :"m" (address)
                    : "memory", "rsp"
                    );
   // The above asm block implements save_and_update_sp(oldsp, address)
   // oldsp = SP, SP = address

   result = func(5);
   printf("%d\n", result);
   asm volatile("mov %0, %%rsp;"
                     :
                     :"r" (oldsp)
                     :"memory", "rsp"
                   );
   // The above asm block implements supdate_sp(oldsp)
   // SP = oldsp
}