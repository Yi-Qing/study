# 确定io号
```bash
$ mount -t debugfs none /sys/kernel/debug
$ cat /sys/kernel/debug/gpio
```
或者
```bash
$ cat /sys/class/gpio/gpiochip*/lable
$ cat /sys/class/gpio/gpiochip*/base
```
