obj-m := test.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build  
M=$(PWD) modules
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module* modules* *.mod

.PHONY: modules modules_install clean
