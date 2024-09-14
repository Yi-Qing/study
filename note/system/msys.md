# Git
使用`msys2`安装的git遇到其他软件会很尴尬，用不了，而且性能也一般，换一种方法安装windows版本git。
[getgit](https://github.com/git-for-windows/git/wiki/Install-or-update-inside-MSYS2,-Cygwin-or-Git-for-windows-itself)安装官方版本到`MSYS2`中，可以在安装之前在脚本中添加如下内容提供`tig`软件，安装完毕后在环境变量中添加/cmd路径提供git功能。
```diff
 cp -f $gitupk/usr/bin/git* $bin
+cp -f $gitupk/usr/bin/tig.exe $bin
+cp -f $gitupk/usr/bin/msys-pcre2-posix-3.dll $bin
```

> 如果一开始安装git，然后环境使用`mingw64`，后续安装软件可能会遇到冲突导致无法安装，理论上直接删除已经存在的文件重新装也行，
> 但是不确定会不会有坑，所以我选择主用`ucrt64`，而且本来这也是官方对未来的选择。

# PATH
默认的PATH里面包含了`${HOME}/bin`目录，但是其实这个目录我压根不用，所以修改`/etc/profile.d/env.sh`删除最开始的PATH修改。

# 软链接
默认情况下`MSYS2`使用软连接功能是直接复制一份，需要在文件`msys2_shell.cmd`中开启windows的链接功能。
```diff
-rem set MSYS=winsymlinks:nativestrict
+set MSYS=winsymlinks:nativestrict
```

# HOME
默认的`MSYS2`家目录在安装目录下，而不是windows统一的目录，修改`MSYS2`配置文件`/etc/nsswitch.conf`使用windows的家目录。
```diff
-db_home: cygwin desc
+db_home: windows
```

# OPENSSH (但我不用，按需求来)
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
			"path": "C:\\Root\\MSYS2\\usr\\bin\\bash.exe",
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
	"clangd.path": "C:\\Root\\MSYS2\\ucrt64\\bin\\clangd.exe",
	"cmakeIntelliSence.cmakePath": "C:\\Root\\MSYS2\\ucrt64\\bin\\cmake.exe",
```

3. 在项目配置中添加gcc等软件路径
- .vscode/tasks.json: 设置使用msys的bash启动编译，已经携带了环境。
- .vscode/launch.json
``` jsonc
"windows": {
    "env": {
        "PATH": "${env:PATH};C:/Root/MSYS2/ucrt64/bin/;",
    },
},
```
