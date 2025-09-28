# 依赖包
```bash
sudo apt install cifs-utils -y
```

# 挂载方式
```fstab
//192.168.0.66/smb /srv/smb cifs file_mode=0664,dir_mode=0775,gid=sambashare,credentials=/root/.smbcredentials,x-systemd.automount,x-systemd.mount-timeout=30,_netdev 0 0
```

# 说明
- `//192.168.0.66/smb`：远程主机的IP地址和共享目录
- `/srv/smb`：挂载点
- `cifs`：文件系统类型
- `file_mode=0664,dir_mode=0775`：文件和目录的权限
- `gid=sambashare`：设置组ID
- `credentials=/root/.smbcredentials`：设置认证文件
- `x-systemd.automount`：自动挂载
- `x-systemd.mount-timeout=30`：设置超时时间
- `_netdev`：使用网络设备，提供给systemd，更好的管理挂载点
- `0 0`：启动顺序

## 认证文件
/root/.smbcredentials
```conf
username=your_username
password=your_password
```

## x-systemd.automount
`x-systemd.automount` 选项可以让系统在启动时自动挂载共享目录，但是在访问时才正式挂载共享目录。
配置完毕以后执行`systemctl daemon-reload`命令使配置生效，
可以通过`systemctl status srv-smb.automount`命令查看挂载状态。
> 注意：挂载点必须存在，否则会报错。
> 另外，查看状态时使用的`srv-smb.automount`是就是把挂载点中的`/`换成`-`再加上`.automount`后缀。
