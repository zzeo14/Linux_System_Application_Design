#ifndef __LINKED_LIST_IMPL_C
#define __LINKED_LIST_IMPL_C

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/timekeeping.h>

extern unsigned long long insert_cnt, search_cnt, delete_cnt;
extern unsigned long long insert_time, search_time, delete_time;

DEFINE_SPINLOCK(list_lock);

struct node {
	int value;
	struct list_head list;
};

LIST_HEAD(global_list_head);

void set_iter_range(int thread_id, int range_bound[]);
void *add_to_list(int thread_id, int range_bound[]);
int search_list(int thread_id, void *data, int range_bound[]);
int delete_from_list(int thread_id, int range_bound[]);

#endif
