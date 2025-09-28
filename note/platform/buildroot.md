- [Buildroot](#buildroot)
  - [发行周期](#发行周期)
  - [常用make参数](#常用make参数)
  - [示例编译脚本](#示例编译脚本)
  - [多线程构建](#多线程构建)
- [目录结构](#目录结构)
  - [Makefile](#makefile)
  - [Config.in](#configin)
  - [arch](#arch)
  - [board](#board)
  - [boot](#boot)
  - [configs](#configs)
  - [docs](#docs)
  - [fs](#fs)
  - [linux](#linux)
  - [package](#package)
  - [system](#system)
  - [toolchains](#toolchains)
  - [utils and support](#utils-and-support)
- [加速编译](#加速编译)
- [什么时候需要`make distclean`](#什么时候需要make-distclean)
- [离线编译](#离线编译)
- [custom tarball](#custom-tarball)
- [kernel headers](#kernel-headers)
- [自动登录](#自动登录)
- [Qt](#qt)
  - [一些环境变量](#一些环境变量)
    - [鼠标](#鼠标)
    - [触摸屏](#触摸屏)
    - [字体](#字体)
- [中文字体](#中文字体)
- [udev](#udev)

# Buildroot
`buildroot`是一个开源的嵌入式`Linux rootfs`制作工具，可以轻松地定制和编译出适用于嵌入式设备`rootfs`。
> 使用详见[官方手册](https://buildroot.org/downloads/manual/manual.html)

## 发行周期
每年总共4个版本，分别在2月，5月，8月，11月发布。每年的第一个版本是LTS（Long Term Support）版本。
> 建议搭配你使用的`Linux发行版`发行时间选择，如全志`T113X-Tina5.0`的SDK带的`buildroot`版本是`2019.02`，
> 使用`ubuntu 20.04`就不好用，需要打各种补丁，而且始终有毛病，不如直接使用`ubuntu 18.04`版本。
> 1. 大部分情况下，高版本兼容旧版本，但是也会有一些小问题，最好是完全适配。
> 2. 大部分情况下，`ubuntu LTS`基于`debian sid`，即`ubuntu LTS`与次年的`debian stable`大致兼容。

| time | debian | ubuntu LTS | Mint |
| :---: | :---: | :---: | :---: |
| 2024.07 |  |  | Wilma(22) |
| 2024.04 |  | Noble |
| 2023.06 | Bookworm(12) |
| 2022.07 |  |  | Vanessa(21) |
| 2022.04 |  | Jammy |
| 2021.08 | Bullseye(11) |
| 2020.06 |  |  | Ulyana(20) |
| 2020.04 |  | Focal |
| 2019.07 | Buster(10) |
| 2018.06 |  |  | Tara(19) |
| 2018.04 |  | Bionic |

## 常用make参数
|命令|作用|
| :--- | :--- |
| `help` | 显示帮助信息 |
| `V=1 <target>` | 显示编译过程 |
| `list-defconfigs` | 列出所有内置的defconfig文件 |
| `menuconfig` | 配置buildroot的选项，包括内核，工具链，软件包，配置等 |
| `savedefconfig` | 保存当前的配置，路径为`BR2_DEFCONFIG` |
| `update-defconfig` | 同`savedefconfig` |
| `busybox-menuconfig` | 配置buildroot的busybox选项 |
| `busybox-update-config` | 更新完整的busybox配置到`BUSYBOX_KCONFIG_FILE` |
| `uboot-menuconfig` | 配置uboot的选项，需要打开`BR2_TARGET_UBOOT_BUILD_SYSTEM_KCONFIG` |
| `uboot-savedefconfig` | 保存uboot的配置 |
| `uboot-update-defconfig` | 更新uboot的defconfig文件到`BR2_TARGET_UBOOT_CUSTOM_CONFIG_FILE` |
| `linux-menuconfig` | 配置内核功能选项 |
| `linux-savedefconfig` | 保存内核的配置 |
| `linux-update-defconfig` | 更新内核的defconfig文件到`BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE` |
| `clean` | 清理buildroot的编译结果 |
| `distclean` | 彻底清理buildroot的编译结果，但是也会顺手把`dl`目录删除，需要重新下载源码 |
| `sdk` | 打包`output/host`，结合`BR2_PACKAGE_HOST_ENVIRONMENT_SETUP`更佳 |
> 1. `BUSYBOX_KCONFIG_FILE` -> `BUSYBOX_CONFIG_FILE` -> `BR2_PACKAGE_BUSYBOX_CONFIG` -> `package/busybox/busybox.config`

## 示例编译脚本
```bash
#!/bin/bash

if [ "$_" != "$0" ];	then
	_exit="return"
else
	_exit="exit"
fi

function usage() {
    echo "Usage: $0 < **your_defconfig** | config | busybox | kernel | uboot | sdk >"
    echo ""
    printf "\tyour_defconfig: which can found in '<buildroot>/configs/' directory\n"
    printf "\tconfig: configure the buildroot\n"
    printf "\tbusybox: configure the busybox\n"
    printf "\tkernel: configure the kernel\n"
    printf "\tuboot: configure the uboot\n"
    printf "\tsdk: build the SDK\n"
    ${_exit}
}

function run_defconfig() {
    if [ -f .config ]; then
        printf "\nThe .config file already exists, sure to reconfigure [y/n]? "
        read -r answer
        echo ""
        if [ ! "$answer" = "y" ]; then
            ${_exit}
        fi
    fi

    # clean up
    make distclean

    # backup dl directory
    mkdir -p .downloads/
    ln -sf .downloads/ dl

    make "$1" || ${_exit}
}

function run_config() {
    make menuconfig || ${_exit}
    make savedefconfig || ${_exit}
}

function run_busybox() {
    make busybox-menuconfig || ${_exit}
    make busybox-update-config || ${_exit}
}

function run_kernel() {
    make linux-menuconfig || ${_exit}
    make linux-savedefconfig || ${_exit}
    if grep -q BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE .config; then
        make linux-update-defconfig || ${_exit}
    fi
}

function run_uboot() {
    make uboot-menuconfig || ${_exit}
    make uboot-savedefconfig || ${_exit}
    if grep -q BR2_TARGET_UBOOT_CUSTOM_CONFIG_FILE .config; then
        make uboot-update-defconfig || ${_exit}
    fi
}

function run_sdk() {
    sed -i 's/^# BR2_PACKAGE_HOST_ENVIRONMENT_SETUP is not set/BR2_PACKAGE_HOST_ENVIRONMENT_SETUP=y/' .config
    make sdk || ${_exit}
}

if [ $# -gt 0 ]; then
    if [ "$1" = "help" ] || [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
        usage
    elif [ "$1" = "config" ]; then
        run_config
    elif [ "$1" = "busybox" ]; then
        run_busybox
    elif [ "$1" = "kernel" ]; then
        run_kernel
    elif [ "$1" = "uboot" ]; then
        run_uboot
    elif [ "$1" = "sdk" ]; then
        run_sdk
    else
        run_defconfig "$1"
    fi
else
    if [ -f .config ]; then
        make
    else
        printf "\nNo .config file found, please set the defconfig first.\n\n"
        usage
    fi
fi
```

## 多线程构建
由于`buildroot`是编译的一个又一个的`package`，不一定有明确的依赖关系，导致`-j`功能难以实现，
直到`2020.02`版本之后，才开始实验性支持，会并行构建看起来没有依赖关系的包。
想要使用该功能需要打开`BR2_PER_PACKAGE_DIRECTORIES `，但是对应的也有一些风险，也不一定完全可用。
而且由于`package`都比较大，部分`package`内部也会有`-j`功能，所以没必要打开`buildroot`的`-j`功能。

# 目录结构
```
buildroot/
├── Makefile
├── Makefile.legacy
├── Config.in
├── Config.in.legacy
├── arch/
├── board/
├── boot/
├── configs/
├── docs/
├── fs/
├── linux/
├── package/
├── support/
├── system/
├── toolchain/
└── utils/
```

## Makefile
- 在输出目录(默认为`output`，可用`O=<dir>`指定)中初始化所有编译过程/编译输出所需要的目录
- 当使用内部工具链时，生成交叉编译工具链。
- 当使用外部工具链时，检查外部工具链的功能并将其导入`Buildroot`环境。
- 编译所有`package`，并将编译结果放到输出目录中，根据配置打包。
> `Makefile.legacy`存放旧版本的`Makefile`功能，当使用旧版本API时，会告诉你错误信息与大致修改方向。

## Config.in
`Config.in`文件位于`buildroot`目录下，主要作用描述各种配置项，本质就是一个`Kconfig`文件
> `Config.in.legacy`存放旧版本的`Config.in`功能，用于兼容，最长兼容两年时间。

## arch
处理器架构相关，比如arm、x86等。

## board
特定开发板相关，但并非必须。

## boot
bootloader相关的配置，比如u-boot、grub等。

## configs
默认的一些配置文件，效果等同于kernel的`arch/<target>/configs`目录。

## docs
一些文档，比如`buildroot`的手册。

## fs
文件系统相关的配置，也包括了生成最终镜像的实现。

## linux
内核相关的配置，比如内核参数、编译选项等。

## package
`buildroot`的核心，`package`目录下存放了所有需要编译的软件。

## system
对系统集成的支持，主要是其中的`skeleton`。

## toolchains
包含与交叉编译工具链相关的所有软件的`Makefiles`和相关文件

## utils and support
一些工具和支持文件。

# 加速编译
可以通过使能`cache`功能来加速编译，具体方法如下：
1. 使能`BR2_CCACHE`
2. 配置`BR2_CCACHE_DIR`为`ccache`缓存目录，默认是`${HOME}/.buildroot-ccache`

# 什么时候需要`make distclean`
- 如果你修改了`Config.in`文件，需要重新生成`.config`文件，可以使用`make savedefconfig`命令

# 离线编译
`buildroot`支持离线编译，即在没有网络的情况下，可以先下载好`package`源码，然后编译。
方法如下：
```bash
mkdir -p .downloads/
ln -sf .downloads/ dl
make source
```
创建`.downloads`目录，并软链接`dl`到`.downloads`目录，这样哪怕后面用户不小心使用了`make distclean`，
也能保证下载的`package`源码不会被删除。

# custom tarball
对于`kernel`和`uboot`，是支持使用特定的`tar`包的，但是在配置的时候需要注意几个地方：
1. 压缩格式为`tar`，仅打包，***需添加一级前缀路径***，他的解压方式为`tar --strip-components=1 -xf`
2. 如果要使用本地文件，请务必添加`file://`前缀，比如`file:///path/to/your/tarball.tar`
3. 如果要使用本地文件且为相对路径，也需要在路径中添加`$(TOPDIR)`前缀，比如`$(TOPDIR)/../yourpath`

# kernel headers
对于使用`custom tarball`的`kernel`，请务必使用`same as kernel being built`的`kernel headers`，
因为一般来说，`custom tarball`的内核都会有一些自己的东西，这种情况下使用标准的`kernel headers`
就可能会因为缺失一些东西而无法编译特定的软件包。
> 同时此时也需要选择`Custom kernel headers series`为内核版本对应的。

# 自动登录
一般我们使用的都是`busybox`提供的`getty`，但是`getty`并不能自动登录，需要手动输入用户名密码。
我们可以添加配置`BR2_TARGET_GENERIC_GETTY_OPTIONS="-n -l /sbin/autologin"`来实现自动登录。
1. `-n`：不显示登录提示符
2. `-l /sbin/autologin`：默认是`/bin/login`，这里指定登录程序为`autologin`

所以我们还需要一个`autologin`程序，比如我们可以创建一个`autologin`脚本,内容如下：
```bash
#!/bin/bash

exec /bin/login -f root
```
> 需要注意，这样写不能使用`linux-utils`中的`login`命令，因为它会导致`getty`无法正常工作。

然后将其复制到`system/skeleton/sbin/`目录下，并给予执行权限。
```bash
mv autologin system/skeleton/sbin/
chmod r+x system/skeleton/sbin/autologin
chmod r+s system/skeleton/sbin/autologin
```

# Qt
```log
├── qt53d                提供3D图形支持
├── qt5base              qt核心库，包括Cpp类，头文件和基础组件
├── qt5charts            提供了图表功能
├── qt5coap              提供CoAP协议支持，用于物联网设备通讯
├── qt5connectivity      提供网络连接功能，包括套接字，HTTP和Websocket等
├── qt5declarative       提供声明式编程支持，用于创建用户界面和组件
├── qt5enginio           提供web服务器和网络程序开发支持
├── qt5graphicaleffects  提供图形效果支持，如阴影，反射，模糊等
├── qt5imageformats      提供了对多种图像格式的支持 GPEG,PNG,BMP等
├── qt5knx               提供了KNX协议支持
├── qt5location          提供定位功能，如GPS,WIFI,蓝牙
├── qt5lottie            提供了Lottie动画格式支持，用于在QT应用程序中嵌入动画
├── qt5mqtt              提供MQTT协议支持
├── qt5multimedia        提供多媒体功能支持，如音频，视频，图像处理等
├── qt5quickcontrols     提供了快速控制界面的支持，用于创建简单易用的控制界面
├── qt5quickcontrols2    提供更高级的界面支持，包括自定义样式和布局
├── qt5quicktimeline     提供了时间轴的支持，用于创建动画和其它时间相关的控制
├── qt5remoteobjects     远程对象支持
├── qt5script            提供脚本能力，包括Javascript和Python
├── qt5scxml             提供了StateChatXML支持，用于创建状态机和复杂交互式应用
├── qt5sensors           提供传感器支持，如温度湿度加速度等
├── qt5serialbus         提供总线的支持，包括USB,RS323,IIC等
├── qt5serialport        提供串口通讯的支持
├── qt5svg               提供SVG图像支持
├── qt5tools             提供了实用工具
├── qt5virtualkeyboard   提供虚拟键盘的支持
├── qt5wayland           提供了Wayland协议支持，可以在wayland上运行qt
├── qt5webchannel        提供web channel的支持，用于在qt应用程序和web应用程序之间进行通讯
├── qt5webengine         提供web引擎，可以在qt程序中内嵌web浏览器
├── qt5webkit            提供了webkit引擎支持，用于创建和显示web内容
├── qt5webkit-examples   webkit示例代码
├── qt5websockets        提供了websocket的支持，实现qt程序和web应用的长连接通讯
├── qt5webview           提供了webview组件，用于在qt程序中内嵌web浏览器
├── qt5x11extras         x11图形环境支持
└── qt5xmlpatterns       提供了XML模式语言的支持，用于定义和解析xml文档
```

首先需要使能`c++`支持，然后才能配置Qt。
## 一些环境变量
1. QT_QPA_FB_HIDECURSOR: 隐藏鼠标光标
2. QT_QPA_GENERIC_PLUGINS: 选择系统插件，比如鼠标: evdevmouse:/dev/input/event5
3. QT_QPA_PLATFORM: 选择平台，比如: linuxfb:tty=/dev/fb0

总结一下，嵌入式常见情况下，就是鼠标/触摸屏，分别对应如下配置：
### 鼠标
```bash
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
export QT_QPA_GENERIC_PLUGINS=evdevmouse:/dev/input/event5
export QT_QPA_FB_HIDECURSOR=0
```

### 触摸屏
```bash
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0

export QT_QPA_FB_TSLIB=1
export QT_QPA_GENERIC_PLUGINS=tslib

export TSLIB_TSDEVICE=/dev/input/event0
export TSLIB_FBDEVICE=/dev/fb0

export QWS_MOUSE_PROTO=tslib:/dev/input/event0
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_CONSOLEDEVICE=none
export TSLIB_PLUGINDIR=/usr/lib/ts
export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/event0
```
>> 理论上应该用`QT_QPA_GENERIC_PLUGINS`就足够了，但是我们自己的Qt程序在全志的SDK上，
>> 只能用`QT_QPA_FB_TSLIB`来实现触摸屏，否则只有控件的聚焦效果，没有点击事件。

### 字体
```bash
export QT_QPA_FONTDIR=/usr/lib/fonts
export QT_QPA_FONTDIR=/usr/share/fonts/wqy-zenhei
```

# 中文字体
如果需要支持中文，则需要安装中文字体，比如`wqy-zenhei`字体。
方法A如下：
1. 下载字体文件，比如`wqy-zenhei.ttf`
2. 将字体文件放到`usr/share/fonts/`目录下

方法B如下：
1. 在`package`目录中创建一个`wqy-zenhei`的`package`
2. 在`package/Config.in`中`source "package/wqy-zenhei/Config.in"`
> 这种方式，直接复制高版本的`buildroot`即可

# udev
目前发现一个问题是，使用触摸屏时，input节点延后很久才会被创建出来：
```log
[root@EBYTE-IMX6UL ~ ]# dmesg | grep -i goodix
[    6.082397] Goodix-TS 1-005d: supply AVDD28 not found, using dummy regulator
[    6.090137] Goodix-TS 1-005d: supply VDDIO not found, using dummy regulator
[    6.216088] Goodix-TS 1-005d: ID 911, version: 1060
[    6.221312] Goodix-TS 1-005d: Direct firmware load for goodix_911_cfg.bin failed with error -2
[    6.233318] Goodix-TS 1-005d: Falling back to sysfs fallback for: goodix_911_cfg.bin
[   69.656425] input: Goodix Capacitive TouchScreen as /devices/platform/soc/2100000.bus/21a4000.i2c/i2c-1/1-005d/input/input0
[   69.677532] evbug: Connected device: input0 (Goodix Capacitive TouchScreen at input/ts)
```

# ldd
buildroot不支持ldd工具，但由于ldd本质只是一个shell脚本，移植很方便。
只需要拷贝一份其他平台的，然后修改一下这几个地方就行：
1. RTLDLIST
2. 程序调用的返回值，有些可能是127，但是有些平台是1
