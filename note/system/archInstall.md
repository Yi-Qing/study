# Arch install
本文是个人向的`Arch`安装过程记录

# 文件系统选择
本文选择使用`btrfs`作为主用的文件系统，主要是看上了它的快照、跨设备和透明压缩功能。

## 当前分区情况
```bash
 $ lsblk -o NAME,TYPE,FSTYPE,FSVER,LABEL,SIZE /dev/nvme0n1

NAME        TYPE FSTYPE FSVER LABEL       SIZE
nvme0n1     disk                        953.9G
├─nvme0n1p1 part vfat   FAT32 ARCHBOOT    380M
└─nvme0n1p2 part btrfs        ArchLinux 953.5G
```

## nvme0n1p1
此分区使用于存放`kernel`和`boot`相关的文件，供`systemd-boot`使用引导系统启动，也即挂载于`/boot`目录。

## nvme0n1p2
此分区为整个系统核心，存放除了`boot`相关的文件以外的一切文件。使用`btrfs`文件系统。
其中又划分了如下子卷：
```bash
ID 256 gen 1287 top level 5 path @
ID 257 gen 1291 top level 5 path home
```
同时单独创建一个目录snapshot用于存放备份。

## 挂载命令
``` bash
# mount -o ssd,compress=zstd,noatime,subvol=@ /dev/nvme0n1p2 /mnt
# mkdir /mnt/{boot,home}
# mount /dev/nvme0n1p1 /mnt/boot/
# mount -o ssd,compress=zstd,noatime,subvol=home /dev/nvme0n1p2 /mnt/home/
```

## 挂载情况
```bash
# /dev/nvme0n1p2 LABEL=ArchLinux
UUID=edc9f45d-e9de-45b6-91f3-8e4ffb43bb20   /   btrfs   rw,noatime,compress=zstd:3,ssd,discard=async,space_cache=v2,subvolid=256,subvol=/@	0 0

# /dev/nvme0n1p1 LABEL=ARCHBOOT
UUID=3C7D-B12B          /boot  vfat    rw,relatime,fmask=0022,dmask=0022,codepage=437,iocharset=ascii,shortname=mixed,utf8,errors=remount-ro	0 2

# /dev/nvme0n1p2 LABEL=ArchLinux
UUID=edc9f45d-e9de-45b6-91f3-8e4ffb43bb20   /home btrfs   rw,noatime,compress=zstd:3,ssd,discard=async,space_cache=v2,subvolid=257,subvol=/home	0 0
```
## snapshot
创建`/.snapshots`目录，用于挂载`/dev/nvme0n1p2`以便创建备份`snapshot/@ro`，使用pacman自带hook功能实现备份。
```bash
 $ cat /etc/pacman.d/hooks/58-backup-btrfs-snap.hook
# Back up the root partition before changing something by pacman
[Trigger]
Operation = Install
Operation = Upgrade
Operation = Remove
Type = Package
Target = *

[Action]
Description = "Back up the root partition"
When = PreTransaction
Exec = /usr/local/sbin/runsnap pacman
Depends = btrfs-progs
AbortOnFail
```

```bash
 $ cat /etc/pacman.d/hooks/59-backup-kernel.hook
# Backup old kernel files
[Trigger]
Operation = Install
Operation = Upgrade
Operation = Remove
Type = Package
Target = linux
[Action]
Description = "Backup old kernel files"
When = PreTransaction
Exec = /usr/local/sbin/runsnap kernel
Depends = coreutils
```

> 这里使用58和59作为hook编号是因为`mkinitcpio`的hook编号是60，需要保证备份在这之前。
>> `runsnap`脚本参考我的其他仓库

## 额外注意事项
### 时间校准
除了官方文档中的步骤外，还需添加一个步骤启用NTP服务，不然部分网站可能会因时间问题拒绝服务。
```bash
 # timedatectl set-ntp true
```

# 软件
## WM
Hyprland (Wayland)
> 理论上wayland支持分数缩放了，但是gui toolkit还没有。这就导致了一些奇怪问题，比如[看图](## 看图)
> 所以暂时来说对于非整数倍缩放的来说还是设置低缩放倍数，然后单独针对性设置吧。[Archwiki HIDPI](https://wiki.archlinux.org/title/HiDPI#GUI_toolkits)
### Qt
有两种配置方法，参考：
1. 全局强制配置:
```
env = QT_AUTO_SCREEN_SCALE_FACTOR, 0
env = QT_ENABLE_HIGHDPI_SCALING, 0
env = QT_SCALE_FACTOR, 1.25
```

2. 针对wayland
```
env = QT_WAYLAND_FORCE_DPI, 120
```
> 参考：
> 1. [依云](https://blog.lilydjwg.me/2022/2/2/wayfire-migration-4-not-so-high-dpi.216078.html)
> 2. [Qt.io](https://www.qt.io/blog/2018/12/14/whats-new-wayland-platform-plugin-qt-5-12)
>> 但是这种方法会导致在fcitx5的输入栏在Qt软件上显小，所以还是用第一种好。

#### theme
使用kvantum

### GTK
也有两种配置方法，但其实本质是一种：
1. 设置环境变量: `env = GDK_DPI_SCALE, 1.25`
2. 设置`dconf`: `gsettings set org.gnome.desktop.interface text-scaling-factor 1.25`

### XWayland
在`hyprland.conf`文件中配置`exec-once = echo 'Xft.dpi:120' | xrdb -merge`

### fcitx5
1. fcitx5-configtool->Addons -> Classic User Interface -> ✅ Use Per Screen DPI
2. fcitx5-configtool->Addons -> Classic User Interface -> Force Font DPI on Wayland 120

### other
详细参考上面`Archwiki`的文章。
我个人是把`vscode`运行在`wayland`下，然后使用它自己的配置实现缩放，其他`electron`软件继续运行在`X11`。
> wezterm不属于任何一个，好在可以手动缩放字体就行。

## 网络
直接用`systemd + iwd`来解决

## IM
fcitx5-rime
> theme: fcitx5-nord

## 终端模拟器
qterminal/wezterm
> 1. qterminal：暂不支持图片显示。
> 2. konsole: kde系列软件依赖那是真的多。
> 3. alacritty: 不支持图片显示(sixel分支可以)和连体字。

## 浏览器
firefox
> 旧版本的`Hyprland`需要设置环境变量`MOZ_ENABLE_WAYLAND=1`

## vscode
使用`VsCodium`版本，创建`${HOME}/.config/codium-flags.conf`，内容如下：
```
--enable-features=UseOzonePlatform
--ozone-platform=wayland
--enable-wayland-ime
```
> 在`Hyprland`下有几个已知问题：
> 1. 只有`electron21`及以上才有`--enable-wayland-ime`（`vscode 1.79`版本开始使用electron22），然后才能使用fcitx5，旧版本可修改code的启动脚本用高版本electron启动来绕过，但又会导致无法使用终端。
> 2. fcixt5使用`预编辑模式`的时候没有主题，如果使用默认暗色主题，看不清输入的是什么，但是`Solarized Dark`主题勉强能用。
> 3. 时不时的crash。

## 声音
pipewire + pavucontrol(GUI) + WirePlumber + qpwgraph(GUI)
> plugins: pipewire-alsa + pipewire-audio + pipewire-jack + pipewire-pulse

## 蓝牙
bluez-utils
> 其实就是命令行下的基础工具包。但是和iwd差不多的难度，够用了。

## 背景光控制
acpilight
> 需要把自己添加到`video`组里面。
> 也可以直接操控`/sys/class/backlight/xxx/brightness`

## polkit
mate-polkit/polkit-gnome
> 如果使用了kde的程序，也可以用kde的套件，不得不说KDE的依赖太多了。

## 截图
grim + slurp
```bash
 $ grim -g "$(slurp)"
```
> `grim`截图有一个问题，不能暂停下方正在播放的视频，所以推荐截取视频的时候不要搭配`slurp`选择，一定要选择可以截图以后在裁去多余的部分。

## 背景
hyprpaper

## 文件管理器
pcmanfm-qt && nnn
> pcmanfm-qt + oxygen-icons + gvfs-mtp + tumbler + engrampa
>> 其实ark支持的压缩功能最多，但我不理解arkrc为啥要存放历史记录，这也不像是配置文件啊，而且kde系依赖太多了。

## 启动器
fuzzel
> `fuzzel`不能输入中文，但是默认配置搜索的时候又按照`LANG`的设置，需要注意增加`Exec`查找范围。
> 依赖oxygen，默认的hicolor图标太少了，不够用。

## 音乐
mpd(local) + lx-music(network)
> 使用mpd-mpris提供统一播放控制

## 视频
mpv
> 使用mpv-mpris提供统一的播放控制

## 计算器
qalculate-gtk
> Hyrpland下，软件不能自动切换显示大小，其他计算器每次切换模式都显示不完全，还好qalculate-gtk不需要变更模式。

## 图像
pqiv + drawing
> 在看看的，wayland下的看图工具似乎都有点问题，明明大小和屏幕一样，就是没法全屏显示，都要放大才行，但是又糊了。
>> 问题似乎出在分数倍缩放上，wayland设置整数倍缩放就没问题。

## pdf
1. zathura + zathura-pdf-poppler
2. firefox

## top
htop

## 通知
mako

## 锁屏
swaylock

## bar
waybar

## lunar
colunar/lunarcalendar-bin
> waybar自定义时钟依赖colunar实现干支历和阴历。lunarcalendar-bin是还不错的GUI农历软件。

## QQ && wechat
问题还很大，用xwayland/qemu虚拟机吧。
> 微信勉强可以用web版本将就用。而QQ和微信的桌面版本，并不能输入中文。QQ还会闪退。
>> xwayland下也有毛病，时不时的某一个拼音直接上屏，打字错乱。

## 游戏
steam本身就不支持wayland，wine暂时也还不支持wayland，所以还是kvm+显卡直通吧，或者也不是不可能用xwayland将就一下，实在不行还能双系统。
> `wine`已经有分支初步支持了，明年(2024)估计能进入主线。
> 听说steam也在推进wayland下的情况了，再等等吧。

# 系统设置
## 自动登录
[greetd](https://wiki.archlinux.org/title/Greetd#Run_local_programs)
> 之前想着用`getty`的方式自动登录，但是我希望只自动登录一次，那种方式不能实现。
>> 现在这种方式需要注意它只读取.profile，需要创建该文件，内容就source一下bash配置就行。

