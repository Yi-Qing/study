# dev_dbg
默认完全不打印，查看debug信息方法有两种：
1. 添加宏定义`DEBUG`
2. 使用系统`dynamic debug`方式：
    * 始能`CONFIG_DYNAMIC_DEBUG`和`CONFIG_DEBUG_FS`。
    * 写入`echo -n "file xxx.c +p" > /sys/kernel/debug/dynamic_debug/control`
    * 调整`/proc/sys/kernel/printk`等级到`8`
> note: 一开始查看control文件发现有对应的源文件，结果就是没有打印，后面发现有注意是`=p`才有
> 打印，而`=_`没有打印。
>> PS: 在需要打印的文件中，执行步骤`1`即可默认打印，不用在系统起来以后再去配置。

# /proc/interrupts
读取到的内容从左到右，分别为：1、逻辑中断号，2、中断在各CPU发生的次数，3、中断所属设备类名称，4、硬件中断号，5、中断处理函数。

> 但是stm32有一个坑，哪怕不同gpio组，也只能有一个编号的中断，比如GPIOB_5设置为中断后，GPIOx_5都不能设置中断了。
> [参考回答](https://community.st.com/t5/stm32-mpus-embedded-software/can-not-use-two-irq-for-different-driver/td-p/347134)
