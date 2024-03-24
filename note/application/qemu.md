## hard disk
### qemu-img
使用`qemu-img create -f qcow2 image_file 32G`创建一个`qcow2`格式的磁盘。
使用qcow2而不是raw：

|  格式 |  磁盘分配  | IO开销 |  快照 |
| :---: | :--------: | :----: | :---: |
| qcow2 | 使用时分配 |  最少  |  支持 |
| raw   | 创建时分配 |  较少  | 不支持|

使用`qemu-img resize disk_image +10G`可以修改磁盘映像大小，
但是如果内部使用了`NTFS`则可能无法引导。

## display
### spice-app
当参数使用`-display spice-app`的时候，需要安装`virt-viewer`才行。
但是通过[arch wiki](https://wiki.archlinux.org/title/QEMU#Graphic_card)中`Running a SPICE client with QEMU`节可以发现
其实是通过`xdg mime`来绑定的。

通过查看未安装`virt-viewer`时的运行报错信息以及`remote-viewer.desktop`中的`MIME`属性可以发现，此时我们应该绑定的是`x-scheme-handler/spice+unix`的对应默认程序。

此时我选择使用`spice-gtk`这个包来作为其处理程序，需要修改部分如下：

``` bash
 $ cat ~/.local/share/applications/spice-uri.desktop
[Desktop Entry]
Name=spice remote
Comment=Access remote desktops by spice uri
Exec= spice.sh %u
Type=Application
Terminal=false
MimeType=x-scheme-handler/spice;x-scheme-handler/spice+unix;x-scheme-handler/spice+tls;application/x-virt-viewer;
StartupNotify=true
Categories=GNOME;GTK;Network;RemoteAccess;
Icon=virt-viewer
```

``` bash
 $ cat spice.sh
#!/usr/bin/env sh

spicy --uri=$@
```

``` bash
 $ cat ~/.config/mimeapps.list
[Added Associations]
......
x-scheme-handler/spice+unix=spice-uri.desktop
......

````
