#include<types.h>
#include<context.h>
#include<file.h>
#include<lib.h>
#include<serial.h>
#include<entry.h>
#include<memory.h>
#include<fs.h>
#include<kbd.h>
#include<pipe.h>


/************************************************************************************/
/***************************Do Not Modify below Functions****************************/
/************************************************************************************/
void free_file_object(struct file *filep)
{
    if(filep)
    {
       os_page_free(OS_DS_REG ,filep);
       stats->file_objects--;
    }
}

struct file *alloc_file()
{
  
  struct file *file = (struct file *) os_page_alloc(OS_DS_REG); 
  file->fops = (struct fileops *) (file + sizeof(struct file)); 
  bzero((char *)file->fops, sizeof(struct fileops));
  stats->file_objects++;
  return file; 
}

static int do_read_kbd(struct file* filep, char * buff, u32 count)
{
  kbd_read(buff);
  return 1;
}

static int do_write_console(struct file* filep, char * buff, u32 count)
{
  struct exec_context *current = get_current_ctx();
  return do_write(current, (u64)buff, (u64)count);
}

struct file *create_standard_IO(int type)
{
  struct file *filep = alloc_file();
  filep->type = type;
  if(type == STDIN)
     filep->mode = O_READ;
  else
      filep->mode = O_WRITE;
  if(type == STDIN){
        filep->fops->read = do_read_kbd;
  }else{
        filep->fops->write = do_write_console;
  }
  filep->fops->close = generic_close;
  filep->ref_count = 1;
  return filep;
}

int open_standard_IO(struct exec_context *ctx, int type)
{
   int fd = type;
   struct file *filep = ctx->files[type];
   if(!filep){
        filep = create_standard_IO(type);
   }else{
         filep->ref_count++;
         fd = 3;
         while(ctx->files[fd])
             fd++; 
   }
   ctx->files[fd] = filep;
   return fd;
}
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/



void do_file_fork(struct exec_context *child)
{
   /*TODO the child fds are a copy of the parent. Adjust the refcount*/
 
}

void do_file_exit(struct exec_context *ctx)
{
   /*TODO the process is exiting. Adjust the ref_count
     of files*/
  
}

long generic_close(struct file *filep)
{
  /** TODO Implementation of close (pipe, file) based on the type 
   * Adjust the ref_count, free file object
   * Incase of Error return valid Error code 
   * 
   */
  if(filep->type == REGULAR) {

    filep->ref_count = filep->ref_count-1;
    if(filep->ref_count == 1) {
     free_file_object(filep);
    }

  } else {
    //pipe
  }
    int ret_fd = -EINVAL; 
    return ret_fd;
}

static int do_read_regular(struct file *filep, char * buff, u32 count)
{
   /** TODO Implementation of File Read, 
    *  You should be reading the content from File using file system read function call and fill the buf
    *  Validate the permission, file existence, Max length etc
    *  Incase of Error return valid Error code 
    * */



  struct inode* node=filep->inode;
  // int offset=&(filep->offp);
  int offset=&(filep->offp);

  int r=flat_read(node, buff,count,offset);
  filep->offp = filep->offp + count;
  return r;
    int ret_fd = -EINVAL; 
    return ret_fd;
}


static int do_write_regular(struct file *filep, char * buff, u32 count)
{
    /** TODO Implementation of File write, 
    *   You should be writing the content from buff to File by using File system write function
    *   Validate the permission, file existence, Max length etc
    *   Incase of Error return valid Error code 
    * */
  struct inode* node=filep->inode;
  int offset=&(filep->offp);

  int wr=flat_write(node, buff,count,offset);

  filep->offp=filep->offp+count;

  // printk("returing=%d,count=%u\n",wr,count);

  return wr;

    int ret_fd = -EINVAL; 
    return ret_fd;
}

static long do_lseek_regular(struct file *filep, long offset, int whence)
{
  // printk("indide seek\n");

  if(whence == SEEK_CUR) {
    filep->offp=filep->offp+offset;

  } else if(whence == SEEK_SET) {
    filep->offp=filep->offp;

  } else if(whence == SEEK_END) {
    filep->offp=filep->inode->file_size + offset;
    
  } else {
    printk("Error\n");
    return -1;
  }
  return filep->offp;
    /** TODO Implementation of lseek 
    *   Set, Adjust the ofset based on the whence
    *   Incase of Error return valid Error code 
    * */


    int ret_fd = -EINVAL; 
    return ret_fd;
}

extern int do_regular_file_open(struct exec_context *ctx, char* filename, u64 flags, u64 mode)
{ 
  // printk("pid is %d, name= %s \n",ctx->pid,ctx->name);
  struct inode *current=lookup_inode(filename);

  if(current == NULL) {
    // printk("file does not exist\n");
    // printk("creating file\n");

    current=create_inode(filename, mode);
  }

  int i=0;

  while(ctx->files[i] != NULL) {
    i++;
  }
  // printk("number of files=%d\n",i);

  struct file* f=alloc_file();

  f->inode=current;

  f->mode=mode;
  f->ref_count=1;
  f->type=REGULAR;
  f->pipe=NULL;

  ctx->files[i]=f;
  f->fops->read=do_read_regular;
  f->fops->write=do_write_regular;
  f->fops->close=generic_close;
  f->fops->lseek=do_lseek_regular;




 return i;
  //file does not exist

  /**  TODO Implementation of file open, 
    *  You should be creating file(use the alloc_file function to creat file), 
    *  To create or Get inode use File system function calls, 
    *  Handle mode and flags 
    *  Validate file existence, Max File count is 32, Max Size is 4KB, etc
    *  Incase of Error return valid Error code 
    * */
    int ret_fd = -EINVAL; 
    return ret_fd;
}

int fd_dup(struct exec_context *current, int oldfd)
{
     /** TODO Implementation of dup 
      *  Read the man page of dup and implement accordingly 
      *  return the file descriptor,
      *  Incase of Error return valid Error code 
      * */
  int i=0;
  while(current->files[i] != NULL && i <= MAX_OPEN_FILES) {
    i++;
  }
  current->files[i]=current->files[oldfd];
  current->files[i]->ref_count=current->files[oldfd]->ref_count+1;
  return i;

    int ret_fd = -EINVAL; 
    return ret_fd;
}


int fd_dup2(struct exec_context *current, int oldfd, int newfd)
{
  /** TODO Implementation of the dup2 
    *  Read the man page of dup2 and implement accordingly 
    *  return the file descriptor,
    *  Incase of Error return valid Error code 
    * */
  current->files[newfd]=current->files[oldfd];
  current->files[newfd]->ref_count=current->files[oldfd]->ref_count+1;
  return newfd;

    int ret_fd = -EINVAL; 
    return ret_fd;
}
