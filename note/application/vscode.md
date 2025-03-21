## vscode debug
对于使用gdb + wsl来说，通过控制台输入指令时需要使用"-exec cmd xxx"形式，
否则会报错为："-var-create: unable to create variable object"

## remote-ssh/wsl
在这种情况下，没办法方便的使用`windows`的终端，只能使用`ssh`目标机器的终端，
所以这时候有一种稍微折中的方案，从目标机器反向`ssh`到`win`系统中。

此时需要如下步骤：
1. 在`win`系统中安装`openssh-server`软件，并启动服务。
2. 配置自启动项，使`openssh-server`服务在系统启动时自动启动。

然后下面有两个可选项：
1. 修改默认终端为`powershell`。
2. 限制只能使用`ssh-key`密钥登录，毕竟win系统一般是作为host机器使用，不应该暴露在公网上。

### 安装
启动具有**管理员**权限的终端，执行以下命令：
```powershell
Get-WindowsCapability -Online | ? Name -like 'OpenSSH*' # 查询当前状态
Add-WindowsCapability -Online -Name OpenSSH.Server~~~~0.0.1.0 # 安装openssh-server, 注意版本号
```

### 自启动
启动具有**管理员**权限的终端，执行以下命令：
```powershell
Set-Service -Name ssh-agent -StartupType 'Automatic' # 设置ssh-agent服务自启动
Set-Service -Name sshd -StartupType 'Automatic' # 设置sshd服务自启动
```

### 默认终端
启动具有**管理员**权限的终端，执行以下命令：
```powershell
New-ItemProperty -Path "HKLM:\SOFTWARE\OpenSSH" -Name DefaultShell -Value "C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe" -PropertyType String -Force
```

### 限制登录
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
