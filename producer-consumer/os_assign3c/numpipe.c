#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/semaphore.h>
#include <linux/slab.h>

#define def 10

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nitin Goyal");

//Definition of a semaphore and mutex
static DEFINE_SEMAPHORE(Full);
static DEFINE_SEMAPHORE(Empty);
static DEFINE_MUTEX(mut);
static int bufsize;
static int Device_open = 0; /* Is device open? Used to prevent multiple access to device */
	
module_param(bufsize,int,0);

static int write_end= -1;
static int read_end= -1;
static int ret;
static char **buffer;

//open function

int dev_open(struct inode *inode,struct file *filep)
{
	Device_open++;
	return 0;
}

//read function

ssize_t dev_read(struct file *filep, char *bufStoreData, size_t len, loff_t *offset)
{	
   int value;
	if (down_interruptible(&Full)!= 0) 
	{
	printk("Process Wait on semaphore Full\n");
        return 0;
	}
	if (mutex_lock_interruptible(&mut)!= 0) 
    {
        printk("Process Wait on mutex read\n");
		up(&Full);
        return 0;
    }      
	value=strlen(buffer[read_end]);		
   	ret = copy_to_user(bufStoreData,buffer[read_end],strlen(buffer[read_end])+1);//copy_to_user(destination,source,sizetotransfer)	      
	if(ret!=0) 
	{
	    printk(KERN_INFO " Failed to read characters to the user\n");
	    mutex_unlock(&mut);		
    	    up(&Empty);		
		return -1;  
	}   
	else
	{
		if(read_end == write_end)
			read_end=write_end=-1;
		else
		{
			if(read_end==bufsize-1)
			read_end=0;
			else
			read_end=read_end+1;
		}
	}	       	
  	mutex_unlock(&mut);
    up(&Empty);			      	      
	return value;  
 }
 
//write function 

ssize_t dev_write(struct file *filep, const char *bufStoreData, size_t bufCount, loff_t *offset){
   static char   message[256] = {0};             
   ret = copy_from_user(message, bufStoreData,bufCount); //copy_from_user(destination,source,count)
   if(ret!=0)
   {
	printk(KERN_INFO " Failed to write characters to device\n");    
	return -1; 	          	
   }
   if (down_interruptible(&Empty)!= 0) 
   {
       printk("Process Wait on semaphore Empty\n");
       return 0;
   }
   if (mutex_lock_interruptible(&mut)!= 0) 
   {
     printk("Process Wait on mutex write\n");
     up(&Full);
       return 0;
   }
   if(read_end==-1 && write_end==-1)
	read_end=write_end=0;
   else	
	  if(write_end==bufsize-1)
		write_end=0;
	  else		
		write_end=write_end+1;

   strcpy(buffer[write_end],message);	
   mutex_unlock(&mut);						
   up(&Full);							
   return bufCount;
}
//close function
int dev_close(struct inode *inode,struct file *filep)
{
	Device_open--;
	return 0;
}
//file operations structure
static const struct file_operations my_fops = {
    .owner = THIS_MODULE, //prevent unloading of this module when operations are in use
   	.read = dev_read,	//points to the method to call when reading from the device
   	.write = dev_write, //points to the method to call when writing to the device
	.open = dev_open, //points to the method to call when opening the device
	.release = dev_close //points to the method to call when closing the device
};

//character device structure
static struct miscdevice my_misc_device = {
.minor = MISC_DYNAMIC_MINOR,
.name = "numpipe",
.fops = &my_fops
};


//driver entry function
static int __init driver_entry(void)
{
	int i=0;
	ret = misc_register(&my_misc_device);
    if (ret){
        printk(KERN_ERR "Unable to register misc device\n");
        return 1;
    }
    printk("Device Registered\n");
    sema_init(&Full,0);
    sema_init(&Empty,bufsize);
    mutex_init(&mut);
	printk("size of buffer entered is %d\n",bufsize);	
	buffer= kmalloc(sizeof(char*)*def,GFP_KERNEL);
	if(buffer==NULL)
	 {
 		printk("Memory Allocation Error\n");
	 }
	for(i=0;i<bufsize;i++){
	buffer[i]=kmalloc(sizeof(char)*bufsize,GFP_KERNEL);	
	if(buffer[i]==NULL)
	 {
 		printk("Memory Allocation Error\n");
	 }
	}	
return ret;
}

//driver exit function
static void __exit driver_exit(void)
{      
	int i;	
	for(i=0;i<bufsize;i++){
	kfree(buffer[i]);	
	buffer[i]=NULL;
	}
	kfree(buffer);
	misc_deregister(&my_misc_device);
	printk(KERN_INFO "device unregistered\n");
}

module_init(driver_entry);
module_exit(driver_exit);

