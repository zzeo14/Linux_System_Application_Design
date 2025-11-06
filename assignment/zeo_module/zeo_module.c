#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

int __init zeo_module_init(void)
{
	printk("simple module\n");
	return 0;
}

void __exit zeo_module_cleanup(void)
{
	
}

module_init(zeo_module_init);
module_exit(zeo_module_cleanup);
MODULE_LICENSE("GPL");
