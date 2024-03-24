# 注册
## 老一点，能用且简单
```c
    int major = register_chrdev(0, "hello", &hello_fops);
    dev_t devt = MKDEV(major, miorr);
```
> 问题在于这种方法，只能指定主设备号，而主设备号只有255个，设备多了就不好了。

## 稍微新点，但是麻烦
```c
    cdev_init(&hello_dev, &hello_fops);
    ret = alloc_chrdev_region(&devt, 0, 1, "hello");    // cat /proc/devices
    if (ret != 0) {
        printk("alloc_chrdev_region failed: %d\n", ret);
    }
    cdev_add(&hello_dev, devt, 1);
```

# 添加节点
## 手动
如果只在内核中注册了驱动，而没有给予节点，则可以通过mknod手动创建一个。
## 自动
```c
    dev_class = class_create(THIS_MODULE, "hello_drv"); // /sys/class/hello_drv
    if (IS_ERR(dev_class)) {
        printk("failed to allocate class\n");
        return PTR_ERR(dev_class);
    }
    device_create(dev_class, NULL, devt, NULL, "hello"); // /dev/hello
```

# 删除节点
## 手动
一切皆文件，rm解千愁
## 自动
```c
    device_destroy(hello_class, devt);
    class_destroy(hello_class);
```

# 注销
## 老一点，能用且简单
```c
    unregister_chrdev(major, "hello");
```
## 稍微新点，但是麻烦
```c
    cdev_del(&hello_dev);
    unregister_chrdev_region(devt, 1);
```
