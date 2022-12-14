#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

#define BUFFER_SIZE 128
#define PROC_NAME "jiffies"

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count,
                         loff_t *pos);

// used to register an (owner) and a function to be called whenever
// /proc/PROC_NAME is read
static const struct proc_ops proc_ops = {
    // .owner = THIS_MODULE,
    .proc_read = proc_read,
};

/* This function is called when the module is loaded. */
static int proc_init(void) {
    // creates the /proc/jiffies entry
    proc_create(PROC_NAME, 0, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) {
    // removes the /proc/jiffies entry
    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/*This function is called each time the /proc/jiffies is read. */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count,
                         loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "Current value of jiffies: %lu\n", jiffies);
    // copies kernel space buffer to user space usr buf
    raw_copy_to_user(usr_buf, buffer, rv);
    return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(
    "Report the current value of jiffies when the /proc/jiffies file is read.");
MODULE_AUTHOR("Adam Soliev");
