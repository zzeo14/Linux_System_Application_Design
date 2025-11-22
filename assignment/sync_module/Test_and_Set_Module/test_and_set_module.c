#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/sched.h>

#define NUM_THREAD 4

int thread_ids[NUM_THREAD];
struct task_struct *threads[NUM_THREAD];
unsigned long long counter = 0;

static int work_fn(void *data)
{
	int i = 1;
	while(!kthread_should_stop()) {
		__sync_lock_test_and_set(&counter, (i++)*(current->pid));
		printk("pid[%d] fetch_and_add_function: counter: %lld\n", current->pid, counter);

		msleep(100);
	}

	return 0;
}

int __init test_and_set_module_init(void)
{
	printk("test_and_set_module_init: Entering Test and Set Module!\n");

	int i;
	for(i = 0 ; i < NUM_THREAD ; i++) {
		thread_ids[i]= i + 1;
		threads[i] = kthread_run(work_fn, &thread_ids[i], "thread[%d]", i + 1);
	}
	
	return 0;
}

void __exit test_and_set_module_cleanup(void)
{
	int i;
	for(i = 0 ; i < NUM_THREAD ; i++){
		if(threads[i]){
			kthread_stop(threads[i]);
			threads[i] = NULL;
		}
	}
	printk("test_and_set_module_cleanup: Exiting Test and Set Module!\n");
}

module_init(test_and_set_module_init);
module_exit(test_and_set_module_cleanup);
MODULE_LICENSE("GPL");
