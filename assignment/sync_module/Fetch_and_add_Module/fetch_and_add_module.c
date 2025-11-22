#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/init.h>

#define NUM_THREAD 4

int thread_ids[NUM_THREAD];
struct task_struct *threads[NUM_THREAD];
unsigned long long counter = 0;

static int work_fn(void *data)
{
	int i;
	for(i = 0 ; i < 4 ; i++){
		__sync_fetch_and_add(&counter, 1);
		printk("pid[%d] fetch_and_add_function: counter: %lld", current->pid, counter);
	}

	return 0;
}

int __init fetch_and_add_module_init(void)
{
	printk("fetch_and_add_module_init: Entering Fetch and Add Module!\n");

	int i;
	for(i = 0 ; i < NUM_THREAD ; i++) {
		thread_ids[i]= i + 1;
		threads[i] = kthread_run(work_fn, &thread_ids[i], "thread[%d]", i + 1);
	}
	
	return 0;
}

void __exit fetch_and_add_module_cleanup(void)
{
	int i;
	for(i = 0 ; i < NUM_THREAD ; i++){
		if(threads[i]){
			kthread_stop(threads[i]);
		}
	}
	printk("fetch_and_add_module_cleanup: Exiting Fetch and Add Module!\n");
}

module_init(fetch_and_add_module_init);
module_exit(fetch_and_add_module_cleanup);
MODULE_LICENSE("GPL");
