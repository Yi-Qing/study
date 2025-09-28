# WSL2
## 局域网访问WSL2
按照官方的文档说明，对于`win11 22h2`及更高版本的计算机上，支持了一种新的网络模式：
[mirrored](https://learn.microsoft.com/zh-cn/windows/wsl/networking)
类似于之前虚拟机的端口直通，但这是镜像，访问win的端口等同于访问WSL的端口。
这就给了我们简单的访问wsl系统的可能。
> 但是当我使用开发板连接时发现，ping都没法ping通，查看后需要设置
> [防火墙规则](https://blog.csdn.net/qq_35132089/article/details/114385192)
> 设置完毕后基本就可以实现网线直连PC和开发板，或者PC和开发板共处同一个局域网互访了。
> (虽然好像需要win这边设置网络类型为公用网络？)

## 个人使用配置
```conf
# File: /etc/wsl.conf 
[boot]
systemd = true

[network]
hostname = trixie

[interop]
appendWindowsPath = false
```

```conf
# File: <Windows_home_dir>/.wslconfig
[wsl2]
swap = 0GB
memory = 16GB
processors = 18
autoProxy = false
vmIdleTimeout = -1
guiApplications = false
networkingMode = mirrored
kernelCommandLine = cgroup_no_v1=all systemd.unified_cgroup_hierarchy=1

[experimental]
sparseVhd = true
hostAddressLoopback = true
autoMemoryReclaim = dropCache
```
这里的`kernelCommandLine`用于使用`docker`功能。

至于为什么我不使用`guiApplications`，因为我在使用`flatpak`包的微信时，会遇到问题，有些界面无法点击。
所以我使用了`VcXsrv`作为显示。虽然也不是很完美，但是能用了。~~只能说，垃圾微信~~

## 后台保留运行
默认情况下，wsl2会在关闭终端一段时间后自动关闭，但是我的使用习惯是丢后台，然后ssh上去，
这时候就有一个问题了，一会儿就自动断开连接了。好在找到了一个解决方法，就是有点绕：
```vbs
Set WshShell = CreateObject("WScript.Shell")

' 1. 关闭WSL（确保干净状态）
WshShell.Run "wsl --shutdown", 0, True

' 2. 挂载VHDX虚拟磁盘
WshShell.Run "wsl --mount --vhd ""D:\vhdx\own.vhdx"" --bare", 0, True

' 3. 启动Debian的D-Bus服务（后台静默运行）
WshShell.Run "wsl -d Debian --exec dbus-launch true", 0, False
```
我们需要一个vbs脚本，由这个脚本来实现自动后台运行。当然，其实重点只是这一句：
```powershell
wsl -d Debian --exec dbus-launch true
```
然后把这个脚本丢到`shell:startup`目录里面，就可以实现开机自动在后台运行WSL2了。
当然，还需要关注`wslconfig`中的`vmIdleTimeout`配置项。

## VHDX挂载
有时候，不想把所有的东西都放置在自动创建的`vhdx`镜像里面，这时候就可以手动创建一个vhdx文件，
然后挂载到wsl2中，来实现隔离管理。具体实现参考`后台保留运行`章节的脚本。

## WSL2使用串口
众所周知的是，wsl2不支持串口和USB，跨系统的IO性能也稀烂。但是好在现在有一个开源项目`USBIPD-WIN`
可以提供wsl2的usb设备支持(虽然不耽误wsl2内核自己没编译存储设备模块功能，还是用不了U盘/SD卡)。
> [参考](https://learn.microsoft.com/zh-cn/windows/wsl/connect-usb)

# WSL1
## signal
wsl1下使用无法实现`SA_RESTART`标志重启慢系统调用，怀疑为转译问题，win内核可能不支持

## wsl找不到设备
有时候会在wsl里面执行`adb root`来启动`adb`服务，但是这时就会出现无法找到设备问题，
正确解决方法是再`cmd`或者`powershell`中执行`adb kill-server && adb root`重启adb。

原因猜测：由于再wsl中使用的其实依然是windows下的adb，wsl中的adb只是起到桥的连接作用
但是wsl中调用`adb start-server`无法启动windows下的adb服务，导致出现这个问题
