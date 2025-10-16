# How to pass the information to the kernel modules during the loading time.

# There are several types of permissions:
+ S_IWUSR
+ S_IRUSR
+ S_IXUSR
+ S_IRGRP
+ S_IWGRP
+ S_IXGRP

In this S_I is a common header.
R = read ,W =write ,X= Execute.
USR =user ,GRP =Group

# module_param()
- This macro is used to initialize the arguments.

- module_param takes three parameters: the name of the variable, its type, and a permissions mask to be used for an accompanying sysfs entry.

- module_param() macro, defined in linux/moduleparam.h :
    # module_param(name, type, perm);

- module_param() macro creates the sub-directory under /sys/module. 

- Example : module_param(valueETX, int, S_IWUSR|S_IRUSR);

- Numerous types are supported for module parameters:
+ bool
+ invbool
+ charp
+ int
+ long
+ short
+ uint
+ ulong
+ ushort

# module_param_array()
- This macro is used to send the array as an argument to the Linux device driver. Array parameters, where the values are supplied as a comma-separated list, are also supported by the module loader. To declare an array parameter, use:
    # module_param_array(name,type,num,perm);

where:
+ name is the name of your array
+ type is the type of the array element
+ num is an interger variale (optional) otherwise NULL
+ perm is the usual permissions value

# module_param_cb()
This macro is used to register the callback. Whenever the argument (parameter) got changed, this callback function will be called.

By using this module_param_cb() macro, we can get a notification when argument change. 

If you want to get a notification whenever the value got to change, we need to register our handler function to its file operation structure first.

struct kernel_param_ops 
{
 int (*set)(const char *val, const struct kernel_param *kp);
 int (*get)(char *buffer, const struct kernel_param *kp);
 void (*free)(void *arg);
};

