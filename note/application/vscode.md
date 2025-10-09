## vscode debug
对于使用gdb + wsl来说，通过控制台输入指令时需要使用"-exec cmd xxx"形式，
否则会报错为："-var-create: unable to create variable object"

## remote-ssh/wsl
有时候在我们使用习惯中会出现这样一个情况：我使用远程功能连接了开发虚拟机，
使用`serial termianl`插件打开了一个串口终端，想要使用windows命令行执行一些操做。
同时希望可以分屏显示这两个终端。比如：使用uuu工具烧写`imx`芯片的开发板，同时查看串口日志。

这时候就可以通过如下快捷键是`ctrl + alt + ~`在vscode中打开一个本地的`powershell`终端。
> 对应的命令ID为：`workbench.action.terminal.newLocal`

### default shell
至少在`windows`平台，强制只能使用`powershell`，而且还不能改，这就让人很难受。
通过查询发现一种绕过的方式：打开`powershell`后自动执行一个脚本启动目标`shell`。
通过进入`powershell`后执行`echo $PROFILE`找到当前会自动加载哪一个脚本。
然后创建该脚本，内容写入：
```powershell
# Function to start MSYS2 manually
function Start-Msys2 {
    param(
        [string]$System = "UCRT64"
    )
    
    $msys2Path = "D:\msys64\usr\bin\bash.exe"
    
    if (Test-Path $msys2Path) {
        $env:MSYSTEM = $System
        $env:MSYS = "winsymlinks:nativestrict"
        $env:MSYS2_PATH_TYPE = "inherit"
        $env:CHERE_INVOKING = "enabled_from_arguments"
        
        Write-Host "Starting MSYS2 $System..." -ForegroundColor Green
        & $msys2Path --login -i
    } else {
        Write-Warning "MSYS2 not found at path: $msys2Path"
    }
}

# Set alias
Set-Alias msys2 Start-Msys2

# Auto-detect VS Code environment
if ($env:TERM_PROGRAM -eq "vscode") {
    Write-Host "VS Code environment detected, auto-starting MSYS2 UCRT64" -ForegroundColor Cyan
    Start-Msys2 -System "UCRT64"
}
```
这样一来，通过`vscode`打开`powershell`就会检测到`TERM_PROGRAM`环境变量，从而自动启动到`msys2`环境。
同时也可以从常规的`powershell`中手动执行`msys2`命令进入`msys2`环境。

### ssh方案
~~还可以从目标机器反向`ssh`到`win`系统中。此时需要如下步骤：~~
1. 在`win`系统中安装`openssh-server`软件，并启动服务。
2. 配置自启动项，使`openssh-server`服务在系统启动时自动启动。

然后下面有两个可选项：
1. 修改默认终端为`powershell`。
2. 限制只能使用`ssh-key`密钥登录，毕竟win系统一般是作为host机器使用，不应该暴露在公网上。

#### 安装
启动具有**管理员**权限的终端，执行以下命令：
```powershell
Get-WindowsCapability -Online | ? Name -like 'OpenSSH*' # 查询当前状态
Add-WindowsCapability -Online -Name OpenSSH.Server~~~~0.0.1.0 # 安装openssh-server, 注意版本号
```

#### 自启动
启动具有**管理员**权限的终端，执行以下命令：
```powershell
Set-Service -Name ssh-agent -StartupType 'Automatic' # 设置ssh-agent服务自启动
Set-Service -Name sshd -StartupType 'Automatic' # 设置sshd服务自启动
```

#### 默认终端
启动具有**管理员**权限的终端，执行以下命令：
```powershell
New-ItemProperty -Path "HKLM:\SOFTWARE\OpenSSH" -Name DefaultShell -Value "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -PropertyType String -Force
```

#### 限制登录
拷贝当前系统的`id_rsa.pub`公钥到当前系统的`C:\Users\用户名\.ssh\authorized_keys`文件中
```powershell
scp C:\Users\用户名\.ssh\id_rsa.pub 用户名@目标机器地址:C:\Users\用户名\.ssh\authorized_keys
```
然后打开该文件后，将`id_rsa.pub`内容追加到文件末尾。
然后给该文件授权：
```powershell
icacls.exe "C:\Users\PC\.ssh\authorized_keys" /inheritance:r /grant "Administrators:F" /grant "SYSTEM:F" # 给管理员和系统授权
```

然后修改配置文件，允许`sshd`服务使用`authorized_keys`，同时禁止密码登录：
打开文件`%programdata%\ssh\sshd_config`:
```sh
PubkeyAuthentication yes    # 允许公钥登录
AuthorizedKeysFile	.ssh/authorized_keys    # 非绝对路径则表示相对用户目录

PasswordAuthentication no  # 禁止密码登录
PermitEmptyPasswords no  # 禁止空密码登录

# Match Group administrators    # 关闭组用户限制
#        AuthorizedKeysFile __PROGRAMDATA__/ssh/administrators_authorized_keys
```

最后重启`sshd`服务：
```powershell
Restart-Service sshd
```

## vscodium
这是一个去除了微软官方监测接口的第三方程序，但是不能使用微软的`remote`功能，好在这两年发展不错，
有一个名为`open-remote-ssh`的插件，但是这个有一个对于我们这些有长城防火墙的用户来说比较麻烦的点，
微软的`remote`可以配置下载服务器时的代理功能，而这个版本的不支持：
```jsonc
    "remote.SSH.httpProxy": {
        "httpProxy": "http://192.168.20.2:10809",
    },
    "remote.SSH.httpsProxy": {
        "httpsProxy": "http://192.168.20.2:10809",
    },
```
好在通过查看日志后发现，它是使用的`wget`下载，这时候可以手动先ssh上去服务器，配置一个`~/.wgetrc`：
```conf
use_proxy=on
ftp_proxy=127.0.0.1:10809
http_proxy=127.0.0.1:10809
https_proxy=127.0.0.1:10809
```
这样可以强制让`wget`下载服务器的时候通过代理来完成下载。

## 老旧系统
按照[官方文档](https://code.visualstudio.com/docs/remote/faq#_can-i-run-vs-code-server-on-older-linux-distributions)
如果想要使用`remote-server`功能，需要满足以下要求：
```
内核 >= 4.18，glibc >=2.28，libstdc++ >= 3.4.25，binutils >= 2.29
```
对于一些老旧的系统，有一个[docker脚本](https://github.com/naitaku/old-linux-vscode/blob/main/Dockerfile)，
我基于此实现了一个`shell`脚本：
```bash
#!/bin/sh

# ref-url: https://code.visualstudio.com/docs/remote/faq
# section: can-i-run-vs-code-server-on-older-linux-distributions
# ref-url: https://github.com/naitaku/old-linux-vscode/blob/main/Dockerfile

os_version=$(grep VERSION_CODENAME /etc/os-release | cut -d= -f2)

if [ "$(id -u)" != 0 ]; then
    echo "This script must be run as root"
    exit 1
fi

pwd=$(pwd)
echo "Current directory: $pwd"
vsc_helper_dir="/opt/vsc_helper"
patchelf_dir="$vsc_helper_dir/bin"
lib_dir="$vsc_helper_dir/lib"

export CT_ALLOW_BUILD_AS_ROOT=y
export CT_ALLOW_BUILD_AS_ROOT_SURE=y

update() {
    echo ""
    echo "====================================================="
    echo "           update and install depend-tools           "
    echo "====================================================="

    apt-get update && apt-get install -y \
        gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
        python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 \
        xz-utils unzip patch rsync meson ninja-build
}

crosstool_ng() {
    echo ""
    echo "====================================================="
    echo "           download and build crosstool-ng           "
    echo "====================================================="

    # shellcheck disable=SC3043
    local url_prefix="http://crosstool-ng.org/download/crosstool-ng"
    # shellcheck disable=SC3043
    local file="crosstool-ng-1.26.0.tar.bz2"
    # shellcheck disable=SC3043
    local out_dir="$pwd/crosstool-ng-1.26.0/out"

    if [ ! -f $file ]; then
        wget $url_prefix/$file -O $file && tar -xaf $file
    fi
    if [ ! -x "$out_dir"/bin/ct-ng ]; then
        cd "$out_dir"/.. && ./configure --prefix="$out_dir" && \
            make && make install && cd "$pwd" || exit
    fi
    export PATH="$PATH":"${out_dir}"/bin
}

build_sysroot() {
    echo ""
    echo "====================================================="
    echo "            build vscode runtimes sysroot            "
    echo "====================================================="

    # shellcheck disable=SC3043
    local url_prefix1="https://raw.githubusercontent.com/"
    # shellcheck disable=SC3043
    local url_prefix2="microsoft/vscode-linux-build-agent/refs/heads/main/"
    # shellcheck disable=SC3043
    if [ "$os_version" = "xenial" ] || [ "$os_version" = "stretch" ]; then
        local file="x86_64-gcc-8.5.0-glibc-2.28.config"
    else
        local file="x86_64-gcc-10.5.0-glibc-2.28.config"
    fi

    # shellcheck disable=SC3043
    local toolchain_dir="$pwd/toolchain"
    mkdir -p "$toolchain_dir"/downloads
    wget ${url_prefix1}${url_prefix2}${file} -O "$toolchain_dir"/.config

    # shellcheck disable=SC3043
    local out_dir="$toolchain_dir"/x86_64-linux-gnu/x86_64-linux-gnu/sysroot
    if [ ! -d "$out_dir" ]; then
        cd "$toolchain_dir" && ct-ng build && cd "$pwd" || exit
    fi

    url_prefix1="https://github.com/NixOS/patchelf/releases/download/0.18.0/"
    file="patchelf-0.18.0-x86_64.tar.gz"
    if [ ! -f "$toolchain_dir"/downloads/${file} ];  then
        wget ${url_prefix1}${file} -O "$toolchain_dir"/downloads/${file}
    fi
    tar -xaf "$toolchain_dir"/downloads/${file} -C "$out_dir"
    rm -rf $vsc_helper_dir && mv "$out_dir" "$vsc_helper_dir"
}

config_env() {
    echo ""
    echo "====================================================="
    echo "          config vscode runtime environment          "
    echo "====================================================="
    {
        echo "VSCODE_SERVER_PATCHELF_PATH=$patchelf_dir/patchelf"
        echo "VSCODE_SERVER_CUSTOM_GLIBC_LINKER=$lib_dir/ld-2.28.so"
        echo "VSCODE_SERVER_CUSTOM_GLIBC_PATH=$lib_dir"
    } >> /etc/environment
}

update
crosstool_ng
build_sysroot
config_env
```
这个脚本可以解决`remote-server`的基本要求中除内核外的问题。
