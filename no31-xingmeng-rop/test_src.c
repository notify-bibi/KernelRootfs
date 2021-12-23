#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cred.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

void gadget(void){
    asm(
        "pop %rdi; ret;"
        "mov %rax, %rdi; ret;"
        "mov %rdi, %cr4; ret;"
        "swapgs; ret;"
        "iretq; ret;"
    );
}


static ssize_t test_ioctl(struct  file * file, unsigned int cmd, size_t arg)
{
    char stack[0x200];
    size_t result;
    pr_info("test_ioctl cmd %d arg %lx\n", cmd, arg);
    switch (cmd)
    {
    case 0xff00:
        *(size_t*)stack = (size_t) gadget;
        asm("breakpoint1:");
        result = copy_to_user((void*)arg, stack, sizeof(stack));
        break;
    case 0xff01:
        result = copy_from_user(stack, (void*)arg, sizeof(stack));
        asm(
            "breakpoint2:"
            "lea %0, %%rax;"
            "mov %%rax, %%rsp;"
            "ret;"
            ::"m"(stack)
        );
    default:
        return -1;
        break;
    }
    return 0;
};


static const struct file_operations test_fops = {
   .owner = THIS_MODULE,
   .unlocked_ioctl = test_ioctl,
};

struct miscdevice test_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "test",
    .fops = &test_fops,
};

int test_init(void)
{
    int error;

    error = misc_register(&test_device);
    if(error)
    {
        pr_err("can't misc_register :(\n");
        return error;
    }

    pr_info("Test driver init\n");
    return 0;
}
void test_exit(void)
{
    misc_deregister(&test_device);
    pr_info("Test driver exit\n");
}
module_init(test_init);
module_exit(test_exit);
