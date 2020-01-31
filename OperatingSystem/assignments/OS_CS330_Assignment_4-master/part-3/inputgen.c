/*
This program creates two files: a binary input file and a human readable txt file

Binary input contains hash operations

PUT entry:
     u64 id  
     u64 key 
     u32 op_type
     u32 datalen  
     char data[datalen]

GET/DEL entry:
     u64 id
     u64 key
     u32 op_type
     
*/

#include "common.h"

#define MANDATORY_PUTS 100

#define PUT_FRESH 20
#define PUT_OVERWRITE 10
#define GET_SUCCESS 50
#define GET_FAIL 10
#define DEL_SUCCESS 7
#define DEL_FAIL 3


#define PUT_FRESH_LOW 0
#define PUT_FRESH_HIGH (PUT_FRESH * 100)

#define PUT_OVERWRITE_HIGH (PUT_FRESH_HIGH + PUT_OVERWRITE * 100)
#define GET_SUCCESS_HIGH (PUT_OVERWRITE_HIGH + GET_SUCCESS * 100)
#define GET_FAIL_HIGH (GET_SUCCESS_HIGH + GET_FAIL * 100)
#define DEL_SUCCESS_HIGH (GET_FAIL_HIGH + DEL_SUCCESS * 100)

#define MAX_LEN_TXT 64
#define MIN_LEN_TXT 16

static int devrandfd = -1;

static void read_random_u64(unsigned long *buf)
{
   if(devrandfd == -1){
       devrandfd = open("/dev/urandom", O_RDONLY);
       assert(devrandfd >= 0);
   }

   assert(read(devrandfd, (char *)buf, 8) == 8);
   return;  
}
static int generate_rand_text(char *buf)
{ 
    int ctr = 0;
    int len = 8 + rand() % 25;

    *(unsigned *)buf = len;
    buf += sizeof(unsigned);

    for(ctr=0; ctr<len; ++ctr){
           int ch = rand() % 62;
           if(ch < 10)
                  *buf = '0' + ch;
           else if(ch < 36)
                  *buf = 'A' + ch - 10;
           else 
                  *buf = 'a' + ch - 36;
           buf++;
    } 

   return len + sizeof(unsigned);  
}

void create_put(long id, int binfd, int txtfd, unsigned long key)
{
    char buf[4096]; 
    char txtbuf[4096];

    unsigned long len, txtctr = 0;

    char *ptr = buf;

    *((unsigned long*)ptr) = id;
    ptr += sizeof(unsigned long);

    txtctr += sprintf(txtbuf, "%lu ", id); 

    *((unsigned long *)ptr) = key;
    ptr += sizeof(unsigned long);
    
    txtctr += sprintf(txtbuf + txtctr, "%lu ", key); 
    
    *((unsigned*)ptr) = PUT;
    ptr += sizeof(unsigned);

    
    txtctr += sprintf(txtbuf + txtctr, "%u ", PUT); 

    
    len = ptr - buf;
    len += generate_rand_text(ptr);
    
    txtctr += sprintf(txtbuf + txtctr, "%u ", *(unsigned*)ptr); 
    memcpy(txtbuf + txtctr, ptr + sizeof(unsigned),  *(unsigned *)ptr); 
    txtctr+=  *(unsigned*)ptr;
    txtbuf[txtctr++] = '\n';
    
    assert(write(txtfd, txtbuf, txtctr) == txtctr);
    assert(write(binfd, buf, len) == len);
}

void create_get_del(long id, int binfd, int txtfd, unsigned long key, int op)
{
    char buf[4096]; 
    char txtbuf[4096];

    unsigned long len, txtctr = 0;

    char *ptr = buf;

    *((unsigned long*)ptr) = id;
    ptr += sizeof(unsigned long);

    txtctr += sprintf(txtbuf, "%lu ", id); 
    
    *((unsigned long *)ptr) = key;
    ptr += sizeof(unsigned long);
    
    txtctr += sprintf(txtbuf + txtctr, "%lu ", key); 

    *((unsigned*)ptr) = op;
    ptr += sizeof(unsigned);

    
    txtctr += sprintf(txtbuf + txtctr, "%u ", op); 

    
    len = ptr - buf;
    txtbuf[txtctr++] = '\n';

    assert(write(txtfd, txtbuf, txtctr) == txtctr);
    assert(write(binfd, buf, len) == len);
}
int main(int argc, char **argv)
{
    long id = 1;
    int binfd, txtfd;

    char buf[4096];
    char *ptr;

    unsigned long totalops;
    unsigned max_unique;

    unsigned long *unique_keys;
    unsigned unique_ctr;
    
    if(argc != 4){
            printf("Usage: %s <filename> <totalops> <max. unique>\n", argv[0]);
            printf("max. unique is used to determine hash table size while running the hash computation programs and should be one-third of total ops\n");
            exit(-1);
    }

    totalops = atol(argv[2]);
    max_unique = atoi(argv[3]);
    
    sprintf(buf, "%s-%d.bin", argv[1], max_unique);
    binfd = open(buf, O_RDWR|O_CREAT, 0644);
    assert(binfd >= 0);
    
    sprintf(buf, "%s-%d.txt", argv[1], max_unique);
    txtfd = open(buf, O_RDWR|O_CREAT, 0644);
    assert(binfd >= 0);

    unique_keys = malloc(max_unique * sizeof(unsigned long));
    
    /* Mandatory puts */ 
    
    for(unique_ctr = 0; unique_ctr < MANDATORY_PUTS; ++unique_ctr){
          unsigned long key;
          read_random_u64(&key);
          unique_keys[unique_ctr] = key;
          assert(unique_ctr < max_unique);
          create_put(id, binfd, txtfd, key);
          ++id;
          assert(unique_ctr < max_unique);
    }     
 
    totalops -= MANDATORY_PUTS;
    
    while(totalops){
           unsigned long key;
           unsigned prob = rand() % 10000; 
           if(prob < PUT_FRESH_HIGH){
                read_random_u64(&key);
                unique_keys[unique_ctr++] = key;
                assert(unique_ctr < max_unique);
                create_put(id, binfd, txtfd, key);
                ++id;
           }else if(prob < PUT_OVERWRITE_HIGH){
                unsigned keypos = rand() % unique_ctr;
                key = unique_keys[keypos];
                create_put(id, binfd, txtfd, key);
                ++id;
           }else if(prob < GET_SUCCESS_HIGH){
                unsigned keypos = rand() % unique_ctr;
                key = unique_keys[keypos];
                create_get_del(id, binfd, txtfd, key, GET);
                ++id;
           }else if(prob < GET_FAIL_HIGH){
                read_random_u64(&key);
                create_get_del(id, binfd, txtfd, key, GET);
                ++id;
           }else if(prob < DEL_SUCCESS_HIGH){
                unsigned keypos = rand() % unique_ctr;
                key = unique_keys[keypos];
                create_get_del(id, binfd, txtfd, key, DEL);
                ++id;
           }else{
                read_random_u64(&key);
                create_get_del(id, binfd, txtfd, key, DEL);
                ++id;
           } 
          totalops--;
    } 
     
    fsync(binfd);
    fsync(txtfd);
    close(binfd);
    close(txtfd);
}
