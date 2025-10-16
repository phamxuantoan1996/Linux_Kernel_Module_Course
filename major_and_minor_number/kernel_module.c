// #include <linux/module.h>
// #include <linux/init.h>
// #include <linux/fs.h>
// #include <linux/cdev.h>
// #include <linux/uaccess.h>

// #define DEVICE_NAME "mydev"
// #define DEVICE_COUNT 2   // Create 2 minor devices: /dev/mydev0, /dev/mydev1

// static dev_t dev_number;         // holds major + minor
// static struct cdev my_cdev;

// // buffer for each device
// static char device_buffer[DEVICE_COUNT][100] = {0};

// // ---------------------------------------------------------------------

// static int my_open(struct inode *inode, struct file *file)
// {
//     int minor = MINOR(inode->i_rdev);
//     pr_info("mydev: device opened (minor=%d)\n", minor);
//     file->private_data = &device_buffer[minor];
//     return 0;
// }

// static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
// {
//     char *data = (char *)file->private_data;
//     size_t len = strlen(data);

//     if (*offset >= len) // end of file
//         return 0; // EOF
//     if (count > len - *offset) // so byte yeu cau doc lon hon so byte con lai cua file
//         count = len - *offset;

//     if (copy_to_user(buf, data + *offset, count)) // buf la bo dem cua space user
//         return -EFAULT;

//     *offset += count;
//     return count;
// }

// static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
// {
//     char *data = (char *)file->private_data;

//     if (count > sizeof(device_buffer[0]) - 1) // neu so byte yeu cau ghi lon hon size cua bo dem kernel space
//         count = sizeof(device_buffer[0]) - 1;

//     if (copy_from_user(data, buf, count))
//         return -EFAULT;

//     data[count] = '\0';
//     pr_info("mydev: written '%s'\n", data);
//     return count;
// }

// static int my_release(struct inode *inode, struct file *file)
// {
//     int minor = MINOR(inode->i_rdev);
//     pr_info("mydev: device closed (minor=%d)\n", minor);
//     return 0;
// }

// // ---------------------------------------------------------------------

// static struct file_operations fops = {
//     .owner = THIS_MODULE,
//     .open = my_open,
//     .read = my_read,
//     .write = my_write,
//     .release = my_release,
// };

// // ---------------------------------------------------------------------

// static int __init kernel_module_init(void)
// {
//     int ret;

//     // dynamically allocate major and minor numbers
//     ret = alloc_chrdev_region(&dev_number, 0, DEVICE_COUNT, DEVICE_NAME);
//     if (ret < 0) {
//         pr_err("Failed to allocate char device region\n");
//         return ret;
//     }

//     pr_info("mydev: Major = %d, Minor = %d-%d\n",
//             MAJOR(dev_number), MINOR(dev_number),
//             MINOR(dev_number) + DEVICE_COUNT - 1);

//     // initialize and add cdev
//     cdev_init(&my_cdev, &fops);
//     my_cdev.owner = THIS_MODULE;
//     ret = cdev_add(&my_cdev, dev_number, DEVICE_COUNT);
//     if (ret < 0) {
//         unregister_chrdev_region(dev_number, DEVICE_COUNT);
//         pr_err("Failed to add cdev\n");
//         return ret;
//     }

//     pr_info("mydev: module loaded successfully\n");
//     return 0;
// }

// static void __exit kernel_module_exit(void)
// {
//     cdev_del(&my_cdev);
//     unregister_chrdev_region(dev_number, DEVICE_COUNT);
//     pr_info("mydev: module unloaded\n");
// }

// module_init(kernel_module_init);
// module_exit(kernel_module_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Pham Toan");
// MODULE_DESCRIPTION("Simple Character Device Example with Major/Minor");

/*
Run commands to create device file.
sudo mknod /dev/mydev0 c 240 0
sudo mknod /dev/mydev1 c 240 1
sudo chmod 666 /dev/mydev0 /dev/mydev1
*/


/*
Create device files automally
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/uaccess.h> // for copy_to_user() and copy_from_user()

#include <linux/device.h>  // for class_create(), device_create()

#define DEVICE_NAME "mydev"
#define DEVICE_COUNT 2   // Create 2 minor devices: /dev/mydev0, /dev/mydev1

static dev_t dev_number;         // holds major + minor
static struct cdev my_cdev;

static struct class *my_class;    // for /dev entries
static struct device *my_device[DEVICE_COUNT];

// buffer for each device
static char device_buffer[DEVICE_COUNT][100] = {0};

// ---------------------------------------------------------------------

static int my_open(struct inode *inode, struct file *file)
{
    int minor = MINOR(inode->i_rdev);
    pr_info("mydev: device opened (minor=%d)\n", minor);
    file->private_data = &device_buffer[minor];
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    char *data = (char *)file->private_data;
    size_t len = strlen(data);

    if (*offset >= len) // end of file
        return 0; // EOF
    if (count > len - *offset) // so byte yeu cau doc lon hon so byte con lai cua file
        count = len - *offset;

    if (copy_to_user(buf, data + *offset, count)) // buf la bo dem cua space user
        return -EFAULT;

    *offset += count;
    return count;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    char *data = (char *)file->private_data;

    if (count > sizeof(device_buffer[0]) - 1) // neu so byte yeu cau ghi lon hon size cua bo dem kernel space
        count = sizeof(device_buffer[0]) - 1;

    if (copy_from_user(data, buf, count))
        return -EFAULT;

    data[count] = '\0';
    pr_info("mydev: written '%s'\n", data);
    return count;
}

static int my_release(struct inode *inode, struct file *file)
{
    int minor = MINOR(inode->i_rdev);
    pr_info("mydev: device closed (minor=%d)\n", minor);
    return 0;
}

// ---------------------------------------------------------------------

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,
};

// ---------------------------------------------------------------------

static int __init kernel_module_init(void)
{
    int ret,i;

    // dynamically allocate major and minor numbers
    ret = alloc_chrdev_region(&dev_number, 0, DEVICE_COUNT, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate char device region\n");
        return ret;
    }

    pr_info("mydev: Major = %d, Minor = %d-%d\n",
            MAJOR(dev_number), MINOR(dev_number),
            MINOR(dev_number) + DEVICE_COUNT - 1);

    // initialize and add cdev
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_cdev, dev_number, DEVICE_COUNT);
    if (ret < 0) {
        unregister_chrdev_region(dev_number, DEVICE_COUNT);
        pr_err("Failed to add cdev\n");
        return ret;
    }

    // Create /sys/class/mydev directory
    my_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(my_class)) {
        pr_err("Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, DEVICE_COUNT);
        return PTR_ERR(my_class);
    }

    // Create device nodes: /dev/mydev0, /dev/mydev1
    for (i = 0; i < DEVICE_COUNT; i++) {
        my_device[i] = device_create(my_class, NULL, MKDEV(MAJOR(dev_number), i),
                                     NULL, "mydev%d", i);
        if (IS_ERR(my_device[i])) {
            pr_err("Failed to create device %d\n", i);
            while (i--)
                device_destroy(my_class, MKDEV(MAJOR(dev_number), i));
            class_destroy(my_class);
            cdev_del(&my_cdev);
            unregister_chrdev_region(dev_number, DEVICE_COUNT);
            return PTR_ERR(my_device[i]);
        }
    }

    pr_info("mydev: module loaded successfully\n");
    return 0;
}

static void __exit kernel_module_exit(void)
{
    int i;
    //delete device file
    for (i = 0; i < DEVICE_COUNT; i++)
        device_destroy(my_class, MKDEV(MAJOR(dev_number), i));

    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number, DEVICE_COUNT);

    pr_info("mydev: module unloaded\n");
}

module_init(kernel_module_init);
module_exit(kernel_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pham Toan");
MODULE_DESCRIPTION("Simple Character Device Example with Major/Minor");


