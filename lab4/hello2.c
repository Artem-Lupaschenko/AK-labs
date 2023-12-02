#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training with list and time tracking");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(repeat_count, "Number of times to print 'Hello, world!'");

extern void print_hello(void);

static unsigned int repeat_count = 1;

module_param(repeat_count, uint, S_IRUGO);

static int __init hello2_init(void)
{
    unsigned int i;

    if (repeat_count == 0 || (repeat_count > 5 && repeat_count < 10)) {
        pr_warn("Warning: Invalid repeat_count value. Using default: 1\n");
        repeat_count = 1;
    } else if (repeat_count > 10) {
        pr_err("Error: repeat_count value is too large. Module not loaded.\n");
        return -EINVAL;
    }

    for (i = 0; i < repeat_count; ++i) {
        print_hello();
    }

    pr_info("Hello2 module initialized.\n");
    return 0;
}

static void __exit hello2_exit(void)
{
    // Do nothing here right now
    pr_info("Hello2 module exited.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
