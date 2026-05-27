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
swap = 24GB
memory = 16GB
processors = 18
autoProxy = false
vmIdleTimeout = -1
guiApplications = false
networkingMode = mirrored
kernelCommandLine = "cgroup_no_v1=all systemd.unified_cgroup_hierarchy=1 sysctl.vm.swappiness=0"

[experimental]
sparseVhd = false
hostAddressLoopback = true
autoMemoryReclaim = dropCache
```
这里的`kernelCommandLine`用于使用`docker`功能。

至于为什么我不使用`guiApplications`，因为我在使用`flatpak`包的微信时，会遇到问题，有些界面无法点击。
所以我使用了`VcXsrv`作为显示。虽然也不是很完美，但是能用了。~~只能说，垃圾微信~~

至于为什么指定不能使用稀疏文件系统，则是因为一个很严重的bug，当前情况下会导致`wsl`的根vhdx文件报废。
[相关issue](https://github.com/microsoft/WSL/issues/4699)
> 血泪的教训

## VHDX挂载
有时候，不想把所有的东西都放置在自动创建的`vhdx`镜像里面，这时候就可以手动创建一个vhdx文件，
然后挂载到wsl2中，来实现隔离管理。具体实现参考`后台保留运行`章节的脚本。
> 需要WIN11的wsl2才有`--mount`参数

### 生成VHDX
windows直接搜索`vhdx`或者`创建虚拟磁盘`，开始创建即可，然后在`磁盘管理`中分离这个虚拟磁盘。
创建完毕以后需要注意，其文件的所有权是`Admin`，给`wsl`的时候有权限问题。
需要找到这个文件，然后右键-》属性-》安全-》高级-》更改所有者为当前用户。
> 因为不是给windows用的，所以到创建分区表的时候就停，不用选择分区GPT或MBR，直接退出就行。

### 数据瘦身
~~wsl默认创建的那一个vhdx文件是会自己在删除文件后清理无效数据进行瘦身的(也就是稀疏文件)，但是~~
手动创建的vhdx不行，还好可以也可以手动瘦身(或需要管理员权限)：
```powershell
diskpart      # 进入diskpart
select vdisk file="D:\VHD\new.vhdx" # 选择vhdx文件
detach vdisk    # 一般不需要这一步
attach vdisk readonly   # 只读挂载，重要
compact vdisk     # 压缩
detach vdisk    # 分离
```

这里提供一个脚本参考，用于瘦身当前目录下所有vhdx文件:
```bat
@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ========================================
echo VHDX 压缩工具
echo ========================================
echo.

set "fileCount=0"
set "successCount=0"
set "failCount=0"

REM 使用 dir 命令遍历，/a-d 表示包含隐藏/系统文件，但不包含同名文件夹
for /f "delims=" %%f in ('dir /b /a-d *.vhdx 2^>nul') do (
    set /a fileCount+=1
    echo [!fileCount!] 正在处理: %%f
    echo 文件路径: %CD%\%%f
    echo.
    
    REM 创建 diskpart 脚本
    (
        echo select vdisk file="%CD%\%%f"
        echo attach vdisk readonly
        echo compact vdisk
        echo detach vdisk
    ) > "%TEMP%\vhdx_compact.txt"
    
    REM 执行 diskpart
    diskpart /s "%TEMP%\vhdx_compact.txt"
    
    if !errorlevel! equ 0 (
        echo ✓ 压缩完成: %%f
        set /a successCount+=1
    ) else (
        echo ✗ 压缩失败: %%f
        set /a failCount+=1
    )
    echo ----------------------------------------
    echo.
)

if !fileCount! equ 0 (
    echo ⚠ 未找到任何 VHDX 文件（含隐藏文件）。
    echo.
)

REM 清理临时文件
if exist "%TEMP%\vhdx_compact.txt" del "%TEMP%\vhdx_compact.txt"

echo ========================================
echo 处理完成！
echo 总文件数: !fileCount!
echo 成功: !successCount!
echo 失败: !failCount!
echo ========================================
pause
```
>> 当前的挂载方式其实完全不需要在执行前手动填0，实在不行也可以手动执行`fstrim`。
>> 对于`btrfs`挂载的时候添加`discard=async`会自动`trim`文件系统(为了更好的压缩，可以`btrfs balance`)。
>> 而`ext4`也已经添加了`discard`，也会自动执行`trim`功能。

### 实际挂载
当启动wsl之前指定传入了相应的vhdx文件后，还需要手动格式化相应的块设备，并修改`/etc/fstab`：
```fstab
# UNCONFIGURED FSTAB FOR BASE SYSTEM

# for own with btrfs
UUID=a6fe6d74-6bfc-48e1-9ab4-32b7e0751ce1  /home  btrfs defaults,nofail,noatime,compress=zstd:5,ssd,discard=async  0 0

# # for own with ext4
# UUID=bc40b922-9ee1-4ec3-bc17-de15d29035f6  /home  ext4  defaults,nofail,noatime  0  2

//192.168.20.66/smb /mnt/smb cifs file_mode=0664,dir_mode=0775,gid=1000,iocharset=utf8,credentials=/root/.smb/nas,x-systemd.automount,x-systemd.mount-timeout=30,_netdev 0 0

/mnt/e/share /mnt/hgfs/share none bind,nofail 0 0
```

## 后台保留运行
默认情况下，wsl2会在关闭终端一段时间后自动关闭，但是我的使用习惯是丢后台，然后ssh上去，
这时候就有一个问题了，一会儿就自动断开连接了。好在找到了一个解决方法，就是有点绕：
```vbs
Set WshShell = CreateObject("WScript.Shell")

' 1. 关闭WSL（确保干净状态）
WshShell.Run "wsl --shutdown", 0, True

' 2. 挂载VHDX虚拟磁盘，这里可以多写好几条，挂载好几个
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
> 此外还需要相应的wsl系统安装`dbus-launch`，对于debian是`sudo apt install dbus-x11`

## WSL2使用串口
众所周知的是，wsl2不支持串口和USB，跨系统的IO性能也稀烂。但是好在现在有一个开源项目`USBIPD-WIN`
可以提供wsl2的usb设备支持(虽然不耽误wsl2内核自己没编译存储设备模块功能，还是用不了U盘/SD卡)。
> [参考](https://learn.microsoft.com/zh-cn/windows/wsl/connect-usb)
>> ~~不确定是不是真的不能用U盘/SD卡，按理说都可以挂载VHDX了存储相关驱动应该是有一部分了吧。~~
>> 按照文档来看，似乎可以连接U盘/SD卡这些存储设备了。

## docker with mirrored network
在wsl2使用镜像网络模式的时候，docker会无法转发端口，目前可以确定的是docker版本大于27.3后问题解决。
> [又有一种说法是wsl2版本高于2.3.11后就已经解决了](https://github.com/microsoft/WSL/releases/tag/2.3.11)
>> 我在使用`podman 5.4.2`与`wsl2 2.6.1`的时候是没有问题，虽然容器访问主机用的主机的ip地址，而不是127.0.0.1，
>> 但是docker好像确实有问题，我使用docker.io 26版本无法从windows访问docker容器

# WSL1
## signal
wsl1下使用无法实现`SA_RESTART`标志重启慢系统调用，怀疑为转译问题，win内核可能不支持

## wsl找不到设备
有时候会在wsl里面执行`adb root`来启动`adb`服务，但是这时就会出现无法找到设备问题，
正确解决方法是再`cmd`或者`powershell`中执行`adb kill-server && adb root`重启adb。

原因猜测：由于再wsl中使用的其实依然是windows下的adb，wsl中的adb只是起到桥的连接作用
但是wsl中调用`adb start-server`无法启动windows下的adb服务，导致出现这个问题
