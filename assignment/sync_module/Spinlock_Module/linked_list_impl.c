#include "linked_list_impl.h"
#include "calclock.h"

#define CHUNK 250000

DEFINE_SPINLOCK(list_lock);
LIST_HEAD(global_list_head);

void set_iter_range(int thread_id, int range_bound[])
{
	range_bound[0] = (thread_id - 1) * CHUNK + 1; // 1, 250001, 500001, 750001
	range_bound[1] = thread_id * CHUNK; // 250000, 500000, 750000, 1000000
}

void *add_to_list(int thread_id, int range_bound[])
{
	int start = range_bound[0], end = range_bound[1];
	printk(KERN_INFO "thread %d range: %d ~ %d\n", thread_id, start, end);

	struct node* first = NULL;
	struct timespec localclock[2];

	int i;
	for(i = start ; i != end ; i++){

		struct node* n = kmalloc(sizeof(*n), GFP_KERNEL);
		if(!n) continue;
		n->value = i;


		spin_lock(&list_lock);
		getrawmonotonic(&localclock[0]);
		list_add_tail(&n->list, &global_list_head);
		getrawmonotonic(&localclock[1]);
		calclock(localclock, &insert_time, &insert_cnt);
		spin_unlock(&list_lock);


		if(first==NULL)
			first=n;
	}

	return first; // return first entry
}

int search_list(int thread_id, void *data, int range_bound[])
{
	int start = range_bound[0], end = range_bound[1];
	printk(KERN_INFO "thread %d search range: %d ~ %d\n", thread_id, start, end);
	struct timespec localclock[2];

	struct node *cur = (struct node *) data, *tmp;


	list_for_each_entry_safe(cur, tmp, &global_list_head, list){
		if (cur->value < start || cur->value > end) continue;
		spin_lock(&list_lock);
		getrawmonotonic(&localclock[0]);
		getrawmonotonic(&localclock[1]);
		calclock(localclock, &search_time, &search_cnt);
		spin_unlock(&list_lock);
	}

	return 0;
}

int delete_from_list(int thread_id, int range_bound[])
{
	int start = range_bound[0], end = range_bound[1];
	printk(KERN_INFO "thread %d delete range: %d ~ %d\n", thread_id, start, end);
	struct node *cur, *tmp;
	struct timespec localclock[2];

	list_for_each_entry_safe(cur, tmp, &global_list_head, list){
		if(cur->value < start || cur->value > end) continue;
		spin_lock(&list_lock);
		getrawmonotonic(&localclock[0]);
		list_del(&cur->list);
		kfree(cur);
		getrawmonotonic(&localclock[1]);
		calclock(localclock, &delete_time, &delete_cnt);
		spin_unlock(&list_lock);
	}

	return 0;
}

