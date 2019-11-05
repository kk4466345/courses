#include "common.h"

int read_op(struct input_manager *in, op_t *op, int tnum)
{
   unsigned size = sizeof(op_t);
   memcpy(op, in->curr, size - sizeof(unsigned long));  //Copy till data ptr     
   if(op->op_type == GET || op->op_type == DEL){
       in->curr += size - sizeof(op->datalen) - sizeof(op->data);
   }else if(op->op_type == PUT){
       in->curr += size - sizeof(op->data);
       op->data = in->curr;
       in->curr += op->datalen;
   }else{
       assert(0);
   }
   if(in->curr > in->data + in->size)
        return -1;
    
   in->being_processed[tnum] = op;
   return 0; 
}

void done_one(struct input_manager *in, int tnum)
{
   in->being_processed[tnum] = NULL; 
}

int lookup(hash_t *h, op_t *op)
{
  unsigned ctr;
  unsigned hashval = hashfunc(op->key, h->table_size);
  hash_entry_t *entry = h->table + hashval;
  ctr = hashval;
  while((entry->key || entry->id == (unsigned) -1) && 
         entry->key != op->key && ctr != hashval - 1){
      
      ctr = (ctr + 1) % h->table_size;
      entry = h->table + ctr; 
  } 
 if(entry->key == op->key){
      op->datalen = entry->datalen;
      op->data = entry->data;
      return 0;
 }
 return -1;
}

int insert_update(hash_t *h, op_t *op)
{
   unsigned ctr;
   unsigned hashval = hashfunc(op->key, h->table_size);
   hash_entry_t *entry = h->table + hashval;
   
   assert(h && h->used < h->table_size);
   assert(op && op->key);

   ctr = hashval;

   while(entry->key && entry->key != op->key && ctr != hashval - 1){
         ctr = (ctr + 1) % h->table_size;
         entry = h->table + ctr; 
   } 

  assert(ctr != hashval - 1);

  if(entry->key == op->key){  //It is an update
      entry->id = op->id;
      entry->datalen = op->datalen;
      entry->data = op->data;
      return 0;
  }
  assert(!entry->key);   // Fresh insertion
 
  entry->id = op->id;
  entry->datalen = op->datalen;
  entry->key = op->key;
  entry->data = op->data;
  
  h->used++;
  return 0; 
}

int purge_key(hash_t *h, op_t *op)
{
   unsigned ctr;
   unsigned hashval = hashfunc(op->key, h->table_size);
   hash_entry_t *entry = h->table + hashval;
   
   ctr = hashval;
   while((entry->key || entry->id == (unsigned) -1) && 
          entry->key != op->key && ctr != hashval - 1){

         ctr = (ctr + 1) % h->table_size;
         entry = h->table + ctr; 
   } 

   if(entry->key == op->key){  //Found. purge it
      entry->id = (unsigned) -1;  //Empty but deleted
      entry->key = 0;
      entry->datalen = 0;
      entry->data = NULL;
      h->used--;
      return 0;
   }
  return -1;    // Bogus purge
}
