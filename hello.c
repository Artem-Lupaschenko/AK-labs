
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h> // Додано для kmalloc та kfree

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training with list");
MODULE_LICENSE("Dual BSD/GPL");

// Структура для розміщення у списку
struct hello_list {
    struct list_head list;
    ktime_t timestamp;
};

// Статична змінна голови списку
static LIST_HEAD(hello_list_head);

static unsigned int repeat_count = 1; // Значення за умовчанням

module_param(repeat_count, uint, S_IRUGO); // Оголошення параметра

static int __init hello_init(void)
{
    unsigned int i;

    if (repeat_count == 0 || (repeat_count > 5 && repeat_count < 10)) {
        pr_warn("Warning: Invalid repeat_count value. Using default: 1\n");
        repeat_count = 1;
    } else if (repeat_count > 10) {
        pr_err("Error: repeat_count value is too large. Module not loaded.\n");
        return -EINVAL;
    }

    // Цикл для додавання елементів до списку
    for (i = 0; i < repeat_count; ++i) {
        struct hello_list *entry = kmalloc(sizeof(struct hello_list), GFP_KERNEL);

        if (!entry) {
            pr_err("Error: Failed to allocate memory for list entry.\n");
            return -ENOMEM;
        }

        entry->timestamp = ktime_get(); // Отримання часу
        list_add(&entry->list, &hello_list_head); // Додавання до списку

        pr_emerg("Hello, world! Entry %u added to the list. Timestamp: %lld\n", i + 1, entry->timestamp);
    }

    return 0;
}

static void __exit hello_exit(void)
{
    struct hello_list *entry, *temp;

    // Цикл для виведення часу та видалення елементів зі списку
    list_for_each_entry_safe(entry, temp, &hello_list_head, list) {
        pr_emerg("Goodbye, world! Entry removed from the list. Timestamp: %lld\n", entry->timestamp);
        list_del(&entry->list);
        kfree(entry); // Звільнення пам'яті
    }

    // Do nothing here right now
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_PARM_DESC(repeat_count, "Number of times to print 'Hello, world!'");
