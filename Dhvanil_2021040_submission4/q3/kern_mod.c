#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

static int pid = 0;
module_param(pid, int, 0); // PID of the process to be read from the kernel module parameter list at load time 

static int __init read_task_struct_init(void) {
    struct task_struct *TASK;
    struct pid *PID;

    PID = find_get_pid(pid); // Find the PID structure for the given PID number
    if (!PID) {
        pr_err("Invalid PID\n");
        return -EINVAL; // Invalid argument
    }

    TASK = get_pid_task(PID, PIDTYPE_PID);
    if (!TASK) {
        pr_err("Failed to get task_struct\n");
        return -EINVAL;
    }

    pr_info("PID: %d\n", task_pid_nr(TASK));
    pr_info("UID: %d\n", task_uid(TASK));
    pr_info("PGID: %d\n", task_pgrp_nr(TASK));
    pr_info("Command path: %s\n", task->comm);

    return 0;
}

static void __exit read_task_struct_exit(void) {
    pr_info("Exiting !\n");
}

module_init(read_task_struct_init);
module_exit(read_task_struct_exit);
