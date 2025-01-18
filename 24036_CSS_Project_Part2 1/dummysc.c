#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/types.h>

SYSCALL_DEFINE1(dummy, uint64_t, arg)
{
        printk("Dummy System Call called with LIBC ID : %llu\n", arg);
        return arg;
}
