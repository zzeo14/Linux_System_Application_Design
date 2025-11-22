#include "linked_list_impl.h"
#include "calclock.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/init.h>

#define NUM_THREAD 4

int thread_ids[NUM_THREAD];
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
	delete_from_list(thread_id, range_bound);

	while(!kthread_should_stop()) {
		msleep(500);
	}
	printk(KERN_INFO "thread #%d stopped!\n", thread_id);

	return 0;
}

int __init semaphore_module_init(void)
{
	printk("semaphore_module_init: Entering RW_Semaphore Module!\n");

	int i;
	for(i = 0 ; i < NUM_THREAD ; i++) {
		thread_ids[i]= i + 1;
		threads[i] = kthread_run(work_fn, &thread_ids[i], "thread[%d]", i + 1);
	}
	
	return 0;
}

void __exit semaphore_module_cleanup(void)
{
	printk("RW_Semaphore linked list insert time: %lld ns, count: %lld\n", insert_time, insert_cnt);
	printk("RW_Semaphore linked list search time: %lld ns, count: %lld\n", search_time, search_cnt);
	printk("RW_Semaphore linked list delete time: %lld ns, count: %lld\n", delete_time, delete_cnt);
	int i;
	for(i = 0 ; i < NUM_THREAD ; i++){
		if(threads[i]){
			kthread_stop(threads[i]);
		}
	}
	printk("semaphore_module_cleanup: Exiting RW_Semaphore Module!\n");
}

module_init(semaphore_module_init);
module_exit(semaphore_module_cleanup);
MODULE_LICENSE("GPL");
