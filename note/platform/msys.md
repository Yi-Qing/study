# Git
使用`MSYS2`安装的git遇到其他软件会很尴尬，用不了，而且性能极差，需要换一种方法安装`windows`版本git。

## 解法一
使用`MinGit + lazygit`，这种方式应该是对`MSYS2`环境最没有侵占的了，完全就是用的`win`环境的`git`。
这种方式不存在任何额外的`MSYS2`环境，但不支持交互式功能，要增加一个包装程序，我选用`lazygit`。
> 1. `MinGit`不能使用`busybox`版本，和`lazygit`结合的时候暂时来说有些功能有问题。
> 2. 记得添加相应目录到`PATH`环境变量。
>> 使用这种方式，我不能完全确定不会因为`MSYS2`和`WIN`路径的原因导致问题。

## 解法二
[脚本安装](https://gitforwindows.org/install-or-update-inside-msys2-cygwin-or-git-for-windows-itself.html)
安装官方版本到`MSYS2`中，可以在安装之前在脚本中添加如下内容提供`tig`软件，安装完毕后在环境变量中添加/cmd路径提供git功能。
```diff
 cp -f $gitupk/usr/bin/git* $bin
+cp -f $gitupk/usr/bin/tig.exe $bin
+cp -f $gitupk/usr/bin/msys-pcre2-posix-3.dll $bin
```
> 如果一开始安装git，然后环境使用`mingw64`，后续安装软件可能会遇到冲突导致无法安装，理论上直接删除已经存在的文件重新装也行，
> 但是不确定会不会有坑，所以我选择主用`ucrt64`，而且本来这也是官方对未来的选择。

## 解法三
[pacman安装](https://gitforwindows.org/install-inside-msys2-proper.html)
> 需要注意，一定要添加仓库在所有其他仓库上面。

# PATH
默认的PATH里面包含了`${HOME}/bin`目录，但是其实这个目录我压根不用，所以修改`/etc/profile.d/env.sh`删除最开始的PATH修改。

# 软链接
默认情况下`MSYS2`使用软连接功能是直接复制一份，需要在文件`msys2_shell.cmd`中开启windows的链接功能。
```diff
-rem set MSYS=winsymlinks:nativestrict
+set MSYS=winsymlinks:nativestrict
```

# pacman签名报错
如果在第一次使用`pacman`更新时报签名错误问题，执行下面命令解决。
```bash
rm -r /etc/pacman.d/gnupg/
pacman-key --init
pacman-key --populate msys2
```

# HOME
默认的`MSYS2`家目录在安装目录下，而不是windows统一的目录，修改`MSYS2`配置文件`/etc/nsswitch.conf`使用windows的家目录。
```diff
-db_home: cygwin desc
+db_home: windows
```

# OPENSSH (按需求来)
默认情况下`MSYS2`不完全支持windows命令，需要在文件`msys2_shell.cmd`中开启windows环境变量继承功能。
```diff
-rem set MSYS2_PATH_TYPE=inherit
+set MSYS2_PATH_TYPE=inherit
```
> 也可以自己添加环境变量，也不是不行。

# VSCODE
1. 在settings.json中添加终端
```jsonc
"terminal.integrated.env.windows": {
		"MSYSTEM": "UCRT64",
		"MSYS": "winsymlinks:nativestrict",	// symlink
		"CHERE_INVOKING": "enabled_from_arguments",	// open shell in current directory
	},
	"terminal.integrated.profiles.windows": {
		"bash (MSYS2)": {
			"path": "D:\\msys64\\MSYS2\\usr\\bin\\bash.exe",
			"args": [
				"--login",
				"-i",
			],
		}
	},
	"terminal.integrated.defaultProfile.windows": "bash (MSYS2)",
```

2. 在settings.json中添加部分应用路径
```jsonc
	"clangd.path": "D:\\msys64\\MSYS2\\ucrt64\\bin\\clangd.exe",
	"cmakeIntelliSence.cmakePath": "D:\\msys64\\MSYS2\\ucrt64\\bin\\cmake.exe",
    "python.defaultInterpreterPath": "D:\\msys64\\ucrt64\\bin\\python.exe",
```

3. 在项目配置中添加gcc等软件路径
- .vscode/tasks.json: 设置使用msys的bash启动编译，已经携带了环境。
- .vscode/launch.json
``` jsonc
"windows": {
    "env": {
        "PATH": "${env:PATH};D:/msys64/ucrt64/bin/;",
    },
},
```

# wezterm
在wezterm中，需要通过配置一些环境变量来代替`msys2_shell.cmd`的功能。
```lua
-- local bash_path = "C:/Program Files/Git/usr/bin/bash.exe"
local bash_path = "D:/msys64/usr/bin/bash.exe"

local function config_env(config)
    if string.find(bash_path, "msys") or string.find(bash_path, "MSYS") then
        config.set_environment_variables = {
            MSYSTEM = "UCRT64",
            -- MSYSTEM = "MINGW64",
            MSYS2_PATH_TYPE = "inherit",
            MSYS = "winsymlinks:nativestrict",
            CHERE_INVOKING = "enabled_from_arguments",
        }
    elseif string.find(bash_path, "Git") then
        config.set_environment_variables = {
            MSYSTEM = "MINGW64",
            MSYS = "winsymlinks:nativestrict",
            CHERE_INVOKING = "enabled_from_arguments",
        }
    end
end
```
