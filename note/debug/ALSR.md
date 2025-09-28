# ALSR
`linux`下`地址空间随机化(Address Space Layout Randomization)`的简写，
是为了防止攻击者修改或利用已知(固定)地址的数据，于`kernel 2.6.12`版本中引入。
但是此时出现了一个问题，偶尔调试的时候会没有这么方便，所以需要临时关闭该功能

# 当前状态
```bash
$ cat /proc/sys/kernel/randomize_va_space
2
 
$ sysctl -a --pattern randomize
kernel.randomize_va_space = 2
```
但是对于部分嵌入式设备而已，压根木有sysctl，最通用的还是直接操控`proc`

## 状态值
0. 完全关闭
1. 半随机 `共享库、栈、mmap() 以及 VDSO 将被随机化`
2. 全随机

# 关闭ALSR
1. ```# echo 0 > /proc/sys/kernel/randomize_va_space```, 注意需要`root`，不能`sudo`
2. ```$ sysctl -w kernel.randomize_va_space=0``` 
3. ```$ setarch `uname -m` -R ./your_program```, 参数`-R`表示关闭地址空间随机化(开启ADDR_NO_RANDOMIZE)
4. gbd (version >= 7) `set disable-randomization on/off`，默认就是`on`

# PIE与ALSR
`ALSR`本身不负责数据段和代码段的随机化，在`GNU/Linux`上，其实是通过`PIE`机制来实现的数据段和代码段的随机化。
想要开启`PIE`，需要在编译的时候添加`-fpie`，链接时添加`-pie`，单独使用其中之一无效。`PIE`在`ALSR`完全关闭的时候也会关闭
> 堆可由`mmap()`与`brk()`两个系统调用分配，大空间以及可能的非长期使用空间将通过`mmap()`分配，所以调试时全关闭`ALSR`更好
