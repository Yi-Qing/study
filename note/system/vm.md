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
然后不需要配置任何环境变量，只需要配置了`DISPLAY`即可，让他默认使用直接硬件渲染。下面的都是错误的，不好用。

对于使用VcXsrv opengl的情况，还需要配置环境变量`export LIBGL_ALWAYS_INDIRECT=1`
> 还有一个环境变量可用：`export MESA_GL_VERSION_OVERRIDE=3.3`
> 对于Qml的程序，还需要配置一个`export QT_XCB_GL_INTEGRATION=none`

# 访问与主机同网段的另外一台电脑
对于`vmware`使用`NAT`模式时候，可以直接访问主机同网段的其他电脑，不需要做任何配置。
