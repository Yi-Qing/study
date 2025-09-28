# 常见的vm平台
目前常见的vm平台有：
1. VMware
2. VirtualBox
3. KVM

# NAT模式-SSH
`NAT`模式下，主机不方便直接访问虚拟机，一般可以通过配置`NAT`模式的端口转发来实现主机访问虚拟机。
对于`SSH`，比较简单，只需要配置将虚拟机的`22`端口映射到主机的任意端口，然后配置`.ssh/config`文件即可。
```conf
Host myvm
    HostName 127.0.0.1              # 虚拟机的IP地址
    User me                         # 虚拟机的用户名
    Port 8899                       # 映射过来的端口
    IdentityFile ~/.ssh/id_ed25519  # 密钥文件路径
    IdentitiesOnly yes              # 只使用密钥文件
```

# NAT模式-TFTP
为了方便开发调试，有时候需要使用tftp协议传输文件到虚拟机，这里介绍一下`NAT`模式下如何映射端口并传输文件。
1. 查看`/etc/services`文件，查看默认的端口号，一般都是`69`，一般情况客户端也是使用的这个端口。
2. 对于`tftpd-hpa`，还可以通过配置`TFTP_OPTIONS=-R <port:port>`参数来指定数据传输使用的端口号。

所以综上所述，需要映射如下端口：
- 虚拟机的`69`端口映射到主机的`69`端口
- 虚拟机的`<port:port>`范围内的端口映射到主机的对应端口

# NAT模式-NFS
nfs需要使用的端口比较多，有以下几个：
1. rpcbind: 111
2. NFS: 2049
3. mountd: 随机
4. nlockmgr: 随机，为了文件安全，还是用的好
5. statd: 随机，可以不用
6. callback: 随机，可以不用
7. quotad: 随机，可以不用
8. listen: 随机，可以不用

## mountd
对于`mountd`有两种修改方式，一种是修改`/etc/services`文件，这种直接照抄即可。
另外一种是修改`/etc/default/nfs-kernel-server`文件，给`RPCMOUNTDOPTS`参数添加`-p <port>`参数。

## nlockmgr
对于`nlockmgr`，同样有两种修改方式，一种是修改`/etc/modprobe.d/nlockmgr.conf`文件，
```conf
# modprobe.d/nlockmgr.conf
options lockd nlm_tcpport=8897
options lockd nlm_udpport=8897
```
另外一种是创建`/etc/sysctl.d/nfs-static-ports.conf`文件或修改`/etc/sysctl.conf`文件。
```conf
# sysctl.d/nfs-static-ports.conf
fs.nfs.nlm_tcpport = 8897
fs.nfs.nlm_udpport = 8897
```

# win主机防火墙
如果想要其他主机访问到虚拟机，需要在主机防火墙放行对应的端口。
如对于`SSH`，新建一个入站规则，规则类型为`端口`，协议为`TCP`，端口为`8899`，动作为`允许`，名称随便起。

# X11转发
一般情况下，我们都是通过虚拟机软件自带的图形界面来访问虚拟机，但对于多数情况工作在终端下的我来说，
切换窗口很麻烦，而幸运的是，有很多种方式实现远程桌面访问，比如`vnc`，`rdp`以及`qemu`的`spice`。
这些协议虽然性能好，但是也会遇到同样的窗口切换麻烦问题，所以我选择使用`X11`转发的方式来作为`GUI`访问。

我们只需要满足以下条件：
1. 虚拟机可以ping通主机
2. 虚拟机配置`DISPLAY`环境变量
3. 主机存在`X11`服务
4. 虚拟机与主机支持`ssh`的`X11`转发功能(可选)

对于第一点，使用`NAT`模式时候，可以直接查看`route`表，`default`路由对应的`ip`就是主机的`ip`地址。
对于第二点，可以在`~/.bashrc`文件中添加`export DISPLAY=<host_ip>:0.0`配置环境变量。
对于第三点，如果一般都是使用windows作为主机，可以使用`VcXsrv`软件来实现`X11`服务。
对于第四点，需要打开`ssh`的`X11`转发功能，分别对应到服务器端和客户端。
```conf
# /etc/ssh/sshd_config
X11Forwarding yes

# ~/.ssh/config
Host myvm
    ForwardX11 yes
    ForwardX11Trusted yes
```
> Tips: `vmware`使用`NAT`模式时候，可以直接访问主机同网段的其他电脑，而不是使用`default`路由地址。

## 环境变量
对于自身支持3D加速的系统，主机端务必不要配置支持`opengl`功能，不一定好用，特别是比如`VcXsrv`的`wgl`功能。
不需要配置任何环境变量，只需要配置了`DISPLAY`即可，让他默认使用直接硬件渲染。

下面是针对不支持`3D`加速的系统的配置，这里以`VcXsrv opengl`的情况为例:
1. `export LIBGL_ALWAYS_INDIRECT=1`: 间接渲染，强制不使用本地硬件渲染
2. `export MESA_GL_VERSION_OVERRIDE=3.3`: 强制指定`opengl`版本为`3.3`
3. `export QT_XCB_GL_INTEGRATION=none`: 强制使用软件渲染代替`opengl`渲染，可以试试`xcb`或`mesa`

## 安全控制
默认情况下`VcXsrv`只允许`localhost`访问，对于需要把虚拟机一类的用户，是无法显示内容在主机上的，
一般的教程都是说在启动`XLaunch`的时候关闭`access control`，但是这样一来又不安全了，
好在`VcXsrv`提供了另外的功能，允许使用`xauth`控制访问权限，当然，其对应的控制权文件无法在`win`上生成，
我们需要在允许显示的`linux`中生成好以后共享给`windows`。

1. 生成`xauth`文件：
```bash
magiccookie=$(echo '{your-pass-phrase}' | tr -d '\n\r' | md5sum | awk '{print $1}')
xauth add <xserver_ip>:<server_id> . $magiccookie
xauth list
cp ~/.Xauthority /mnt/c/Users/<your_pc_name>/.Xauthority
```

2. 配置`Vcxsrv`：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<XLaunch WindowMode="MultiWindow" ClientMode="NoClient" LocalClient="False" Display="-1" RemoteProgram="xterm" RemotePassword="" PrivateKey="" RemoteHost="" RemoteUser="" XDMCPHost="" XDMCPBroadcast="False" XDMCPIndirect="False" Clipboard="True" ClipboardPrimary="True" ExtraParams="-auth C:\users\PC\.Xauthority" Wgl="False" DisableAC="Flase" XDMCPTerminate="False"/>
```

3. 构建快捷方式，其中的`config.xlaunch`文件就是上面的配置文件，放在`C:\Users\PC`目录下。
```
"C:\Program Files\VcXsrv\xlaunch.exe" -run C:\Users\PC\config.xlaunch
```

# 访问与主机同网段的另外一台电脑
对于`vmware`使用`NAT`模式时候，可以直接访问主机同网段的其他电脑，不需要做任何配置。

# vmware
## vmware空间压缩
除了直接使用`vmware`程序自带的压缩功能外，还可以在虚拟机系统的执行一些操作来压缩，
之所以需要这样，是因为有时候实体硬盘不够用，而使用程序自带的压缩功能，至少需要同等大小的硬盘空间。
下面是操作方法与理论：

### 理论
由于`vmware`对硬盘的存储方式使用了`稀疏文件`的形式，所以只要我们可以把多余的要删除的空间全部填0，
然后再恢复对应的存储形式即可。

同时，为什么对于虚拟机软件来说，看到的空间依然是存在内容的，就在于，我们从系统中执行删除操作，
只是删除了文件实际内容的索引信息，但是文件本身还是存在于系统中的，这也是我们误删文件后还能找回的根本。

### 操作
对于第一步，直接使用`dd`命令即可，之后再从文件系统层面中删除这个文件即可。
```bash
dd if=/dev/zero of=wipefile bs=1G count=10; /usr/bin/rm wipefile
```

对于第二步，`open-vm-tools`提供了一个命令：`vmware-tools-cmd`，执行下面的命令即可：
```bash
sudo vmware-tools-cmd disk shrinkonly
```

### 注意
由于想要把文件从原始的情况转换成稀疏文件，这时候需要一次拷贝过程，
所以实体硬盘的剩余空间也需要保证满足对应的最终稀疏文件的最小大小。
这时候，在创建虚拟机的时候把硬盘拆分成多个文件就是一个很好的选择。

## 卡顿
### 大小核
分配vmware管理员权限，供vmware自行调配大小核分配，一定不要分配超过主机大核心数量，避免卡顿。

### cpu分配
分配多线程时，应该注意处理器数量为1，每个处理器的内核数量才是想要配置的。如果写反了，也会卡顿。

## 后台运行
"vmware_path\vmrun.exe" -T ws start "vm_machine_path\machine_name.vmx" nogui

## 共享文件目录，但是找不到
使用`open-vm-tools`而不是安装`vmware-tools`，这时候就可能出现找不到共享目录的问题，
[只需要在`etc/fstab`后面添加一行即可自动挂载](https://www.jianshu.com/p/1fc383f560cd)

```
.host:/  /mnt/hgfs  fuse.vmhgfs-fuse  allow_other  0  0
```

> 或者也可以每次手动挂载：
> ```bash
> vmhgfs-fuse .host:/ /mnt/hgfs -o subtype=vmhgfs-fuse,allow_other
> ```
