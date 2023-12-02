#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include "hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training with list and time tracking");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_list {
    struct list_head list;
    ktime_t timestamp_before;
    ktime_t timestamp_after;
};

static LIST_HEAD(hello_list_head);

void print_hello(void)
{
    struct hello_list *entry = kmalloc(sizeof(struct hello_list), GFP_KERNEL);

    if (!entry) {
        pr_err("Error: Failed to allocate memory for list entry.\n");
        return;
    }

    entry->timestamp_before = ktime_get();
    pr_info("Hello, world! Timestamp before: %lld\n", entry->timestamp_before);
    entry->timestamp_after = ktime_get();

    list_add(&entry->list, &hello_list_head);
}

EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    pr_info("Hello1 module initialized.\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct hello_list *entry, *temp;

    list_for_each_entry_safe(entry, temp, &hello_list_head, list) {
      	pr_info("Goodbye, world! Timestamp before: %lld, Timestamp after: %lld\n",
         entry->timestamp_before, entry->timestamp_after);    
        list_del(&entry->list);
        kfree(entry);
    }

    pr_info("Hello1 module exited.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
