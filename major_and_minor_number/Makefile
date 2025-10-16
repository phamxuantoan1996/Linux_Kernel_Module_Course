# Path to the BBB kernel build directory (update this)
KDIR ?= /home/phamtoan/Downloads/Build_Image_BBB/kernelbuildscripts/KERNEL

# Path to your cross-compiler (update prefix as needed)
CROSS_COMPILE ?= /home/phamtoan/Downloads/Build_Image_BBB/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
ARCH ?= arm

PWD := $(shell pwd)

# Name of module
obj-m := kernel_module.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean