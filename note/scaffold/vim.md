# sudo权限困扰
> 如果打开文件时没有sudo权限，同时这个文件是一个readonly的文件时，可以使用如下命令保存
```
:w ! sudo tee %
```
> 注意这里感叹号两边的空格不要忽略了。  
> tee是linux下的一个小工具，用于将stdin的内容写入指定文件，
> `%`是vim的一个特殊寄存器，保存当前编辑文件的文件路径。
> 这里就是以sudo的权限将当前文件的内容从外部写入到当前文件中，
> 完成写入后vim会提示文件内容已经更改，是否加载最新的文件，
> 选择load然后`q!`退出即可。
>> Tips: neovim 中sudo无法获取密码问题暂时没找到解决方法
> Tips: 更推荐使用sudoedit来实现，而不是sudo vi。

# ESC太远
> 有时候esc按键距离键盘中心太远，在切换模式的时候很麻烦，
> 可以使用`Ctrl + o`和`ctrl + [`来代替，
> 其中`Ctrl + o`只能输入一次指令，然后就会返回到insert模式, 
> 而`Ctrl + [`则直接代表ESC功能，从insert模式退出来。

# 块选择修改
> `Ctrl + v`选择一块数据后需要replace这部分数据时可以使用按
> 键`c`来实现，`c`的本意为change，将选中的部分删除后进入insert模式  
> 块选择后可以使用`I`来进入插入模式，在该情况下光标位置为当前位置，
> 当输入完毕后按`ESC`退出输入模式后会发现块选择的地方都会填充之前
> 输入的数据

# 行首序号
1. 先给每一行前添加一个数字，第一行为1，然后在第二行开始选择之后按`g + C-A`
2. `:%s/^/\=line(".")-1."."/`从0开始编号且后缀个点，如果想从一开始就不减一
> vim: help sub-replace-\=
