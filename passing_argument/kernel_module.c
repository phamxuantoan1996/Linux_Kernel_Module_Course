#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int cb_valueETX = 0;

/*----------------------Module_param_cb()--------------------------------*/
int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                printk(KERN_INFO "Call back function called...\n");
                printk(KERN_INFO "New value of cb_valueETX = %d\n", cb_valueETX);
                return 0;
        }
        return -1;
}

int get_param(char *val, const struct kernel_param *kp)
{
    printk(KERN_INFO "Call back function called...\n");
    printk(KERN_INFO "Value of cb_valueETX is read...\n");
    return param_get_int(val,kp);
}

const struct kernel_param_ops my_param_ops = 
{
        .set = &notify_param, // Use our setter ...
        .get = &get_param, // .. and standard getter
};
 
module_param_cb(cb_valueETX, &my_param_ops, &cb_valueETX, S_IRUGO|S_IWUSR );
/*-------------------------------------------------------------------------*/

static int __init kernel_module_init(void)
{
        
    printk(KERN_INFO "Kernel Module Inserted Successfully...\n");
    return 0;
}
/*
** Module Exit function
*/
static void __exit kernel_module_exit(void)
{
    printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(kernel_module_init);
module_exit(kernel_module_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pham Toan");
MODULE_DESCRIPTION("Passing param");
MODULE_VERSION("1.0");