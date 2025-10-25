#include <linux/syscalls.h>

SYSCALL_DEFINE0(mycall)
{
	printk("System call Example!\n");

	return 0;
}	
