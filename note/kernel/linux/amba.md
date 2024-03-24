## 从vexpress-a9控制台驱动看amba
对于普通的`platform`驱动而言，在匹配的时候会先看`of_match_table -> id_table -> name`来寻找匹配是设备资源，
但是在`drivers/tty/serial/amba-pl011.c:pl011_driver`结构体中有`id_table`和`name`，
其中`id_table`中内容与普通的`platform`设备不同，于是先查询`name`属性，但是在dts中没有匹配设备。
然后开始查看这个陌生的`id_table`：
```
static struct amba_id pl011_ids[] = {
	{
		.id	= 0x00041011,
		.mask	= 0x000fffff,
		.data	= &vendor_arm,
	},
	{
		.id	= 0x00380802,
		.mask	= 0x00ffffff,
		.data	= &vendor_st,
	},
	{ 0, 0 },
}; // kernel version 4.4
```
其中id字段很明显是匹配相关的，但是这些id在dtb中找不到。通过搜索可以发现，这是写在硬件外设当中的。
同时发现了linux中添加amba外设设备的接口：`amba_device_add`，在该接口中，可以找到这样一段代码：
```
if (cid == AMBA_CID || cid == CORESIGHT_CID)
	dev->periphid = pid;
```
其中`AMBA_CID = 0xb105f00d`，`CORESIGHT_CID = 0xb105900d`，如果cid值符合则便是这是一个amba总线设备，
这时将该设备pid保存下来，用于后续probe。

但是，当我使用qemu调试时发现，没有一个符合的pid，但是他就是能probe，于是开始通过probe函数调用栈来查看调用关系：
`drivers/amba/bus.c:amba_probe -> drivers/tty/serial/amba-pl011.c:pl011_probe`，仔细查看amba_probe函数：
```
	const struct amba_id *id = amba_lookup(pcdrv->id_table, pcdev);
```
这句话中的`amba_lookup`运用到了`id_table`，进去看一下：
```
	while (table->mask) {
		ret = (dev->periphid & table->mask) == table->id;
		if (ret)
			break;
		table++;
	}

```
可以发现：其实`mask`字段并不是直接用在`id`字段的，而是用于与设备的periphid相与的。
这时在通过qemu模拟调试可以发现总共出现了四次`0x00141011`，同时`pl011_probe`也调用了四次，刚好匹配。
同时也可以发现一个巧妙的设计：之前`id_table`中最后是使用`{0,0}`来结尾，这里则是使用`mask == 0`来跳出循环。
