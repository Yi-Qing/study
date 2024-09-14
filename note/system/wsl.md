## 局域网访问wsl2
自从更新了WSL2 2.0和win 11 23H2后，出现了一种新的网络连接方式：
[mirrored](https://learn.microsoft.com/zh-cn/windows/wsl/networking)
类似于之前虚拟机的端口直通，但这是镜像，访问win的端口等同于访问WSL的端口。
这就给了我们简单的访问wsl系统的可能。
> 但是当我使用开发板连接时发现，ping都没法ping通，查看后需要设置
> [防火墙规则](https://blog.csdn.net/qq_35132089/article/details/114385192)
> 设置完毕后基本就可以实现网线直连PC和开发板，或者PC和开发板共处同一个局域网互访了。
> (虽然好像需要win这边设置网络类型为公用网络？)

> 传闻需要win版本为`22621.2359`以上，以及一个从store下载的`Windows Subsystem for Linux`

## wsl2 使用串口
众所周知的是，wsl2不支持串口和USB，跨系统的IO性能也稀烂。但是好在现在有一个开源项目`USBIPD-WIN`
可以提供wsl2的usb设备支持(虽然不耽误wsl2内核自己没编译存储设备模块功能，还是用不了U盘/SD卡)。
> [参考](https://learn.microsoft.com/zh-cn/windows/wsl/connect-usb)

## wsl找不到设备
有时候会在wsl里面执行`adb root`来启动`adb`服务，但是这时就会出现无法找到设备问题，
正确解决方法是再`cmd`或者`powershell`中执行`adb kill-server && adb root`重启adb

原因猜测：由于再wsl中使用的其实依然是windows下的adb，wsl中的adb只是起到桥的连接作用
但是wsl中调用`adb start-server`无法启动windows下的adb服务，导致出现这个问题

## signal
wsl1下使用无法实现`SA_RESTART`标志重启慢系统调用，怀疑为转译问题，win内核可能不支持
