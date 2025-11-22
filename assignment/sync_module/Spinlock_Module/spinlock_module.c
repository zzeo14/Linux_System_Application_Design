#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/init.h>

#define NUM_THREAD 4

int thread_id[NUM_THREAD];
struct task_struct *threads[NUM_THREAD];

unsigned long long insert_cnt = 0;
unsigned long long search_cnt = 0;
unsigned long long delete_cnt = 0;
unsigned long long insert_time = 0;
unsigned long long search_time = 0;
unsigned long long delete_time = 0;

static int work_fn(void *data)
{
	int range_bound[2];
	int thread_id = *(int*) data;

	set_iter_range(thread_id, range_bound);
	void *ret = add_to_list(thread_id, range_bound);
	search_list(thread_id, ret, range_bound);

	while(!kthread_should_stop()) {
		msleep(500);
	}
	printk(KERN_INFO "thread #%d stopped!\n", thread_id);

	return 0;
}

int __init spinlock_module_init(void)
{
	printk("Entering Spinlock Module!\n");

	int i;
	for(i = 0 ; i < NUM_THREAD ; i++) {
		thread_id[i]= i + 1;
		threads[i] = kthread_run(work_fn, &thread_id[i], "thread[%d]", i + 1);
	}
	
	return 0;
}

void __exit spinlock_module_cleanup(void)
{
	printk("Spinlock linked list insert imte: %lld ns, count: %lld\n", insert_time, insert_cnt);
	printk("Spinlock linked list search imte: %lld ns, count: %lld\n", search_time, search_cnt);
	printk("Spinlock linked list search imte: %lld ns, count: %lld\n", delete_time, delete_cnt);
	int i;
	for(i = 0 ; i < NUM_THREAD ; i++){
		if(threads[i]){
			kthread_stop(threads[i]);
			printk(KERN_INFO "thread #%d stopped!\n", i+1);
		}
	}
	printk("Exiting Spinlock Module!\n");
}

module_init(spinlock_module_init);
module_exit(spinlock_module_cleanup);
MODULE_LICENSE("GPL");
