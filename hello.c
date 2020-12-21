#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>

MODULE_AUTHOR("Andrew Yushchenko");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint lines = 1;
module_param(lines, uint, S_IRUGO);
MODULE_PARM_DESC(lines, "Number of printed 'Hello world' lines");

struct my_list_head {
	struct my_list_head *next;
	ktime_t time;
};

static struct my_list_head *head;

static void eraseList(void) {
	struct my_list_head *temp;
	while (head != NULL) {
		// Save current space address
		temp = head;
		printk(KERN_INFO "Time - %lld", temp->time);
		head = temp->next;

		// Free/Erase allocated space
		kfree(temp);
	}

	printk(KERN_ALERT "Erase List\n");
}

static int __init hello_init(void)
{

	struct my_list_head *temp1, *temp2;
	uint i = 0;

	if (!lines || (lines >= 5 && lines <= 10))
		printk(KERN_WARNING "%s", !lines ? "Lines equal to 0" : "Lines between 5 and 10");
	else
		BUG_ON(lines > 10);

	// Set first node
	head = kmalloc(sizeof(struct my_list_head*), GFP_KERNEL);
	temp1 = head;

	for (i = 0; i < lines; i++) {
		temp1->next = kmalloc(sizeof(struct my_list_head), GFP_KERNEL);
		
		// Create artificial Error
		if (i == 6) {
			kfree(temp1->next);
			temp1->next = NULL;
		}
	

		if (temp1->next == NULL) break;

		temp1->time = ktime_get();
		printk(KERN_INFO "Hello world\n");

		temp2 = temp1;
		temp1 = temp1->next;
	}

	if (i != lines) {
		eraseList();
		return 0;
	}

	if (lines > 0) {
		kfree(temp2->next);
		temp2->next = NULL;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	eraseList();
	printk(KERN_ALERT "Delete Module\n");
}

module_init(hello_init);
module_exit(hello_exit);
