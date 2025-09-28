# 设置登录前提示信息
有两个相关文件
1. /etc/issue
2. /etc/motd

# 常用命令
- [kill指令](#kill)  
    - [kill](#kill)  
    - [killall](#killall)  
    - [pkill](#pkill)  
    - [xkill](#xkill)
- [pid指令](#pidof)
    - [pidof](#xkill)

## kill
作用：向指定进程发送指定信号，信号分类可以使用`kill -l`查看，如果没有指定则发送`SIGTERM`信号  
用法：`kill [options] <pid> [...]`  

## killall
作用：给指定名字的所有进程发送指定信号，信号分类可以使用`killall -l`查看，如果没有指定则发送`SIGTERM`信号  
用法：`killall [-signal] <process name>`

## pkill
作用：根据名称和其他属性发送信号通知进程，同时也可以用于踢出用户登录  
用法：`pkill [-t] <terminal_name>`  
> tips：`terminal_name`可以通过使用`w`指令来获取

## xkill
作用：通过x资源杀死`x client`  
用法：直接执行xkill后鼠标选中即可

## pidof
作用：获取一个正在运行的程序的pid  
用法：`pidof a.out`  
> tips: `kill -9 $(pidof a.out)`，向指定应用发送信号
