# 目录

# 常用托管平台
1. [github](https://github.com/): 全球最大, 但是国内访问较慢
2. [码云gitee](https://gitee.com/): 国内托管平台, 由于可以快速将github的仓库导入到该平台, 往往作为一个工具平台.
3. [其他详见](https://zhuanlan.zhihu.com/p/33904396)

# 安装配置
debain系linux系统安装: sudo apt install git

用户配置:
> 全局配置, 该配置文件位于家目录下.gitconfig文件:
> ```
> git config --global user.name "yourname"
> git config --global user.email you@emial.com
> ```
>
> 局部配置, 该配置位于工作区下的.git目录中的config文件:
> ```
> git config --local user.name "yourname"
> git config --local user.email you@emial.com
> ```

# 传输协议
1. 本地协议:
> 远程仓库位于一个大家都能共享的地方, 比如硬盘上的另外一个目录. 常见于团队每一个成员都对一个共享的文件系统(例如NFS)拥有访问权, 或者比较少见的多人共用同一台电脑的情况.
> 
> 克隆的时候远程URL地址就是远程仓库的路径
> ```
> git clone /opt/git/project.git
> 或者
> git clone file:///opt/git/project.git
> ```

2. ssh协议:
> 目前常见的git传输协议之一, 服务器容易架设, 安全高效, 但是不能匿名访问
> ```
> git clone ssh://user@server:project.git
> 或者不指明为ssh协议, 直接使用如下URL也会默认选择ssh协议
> git clone user@server:project.git
> ```

3. http/s协议
> 最为简单常见的传输协议, 目前主要使用https协议
> ```
> git clone http://example.com/gitproject.git
> git clone https://example.com/gitproject.git
> ```

4. git协议
> 包含在git中的一个特定协议, 监听特殊端口9418, 目前最快的协议(但是国外服务器不容易使用代理加速, 反而麻烦), 同时架设难, 任何人都可以进行推送
> ```
> git clone git://codeaurora.org/platform/manifest.git
> ```

# 仓库创建
1. 主动创建: *`git init`*
> 该方法会在当前目录创建一个`.git`目录, 用于版本信息的记录
2. 拷贝远程仓库: *`git clone <url>`*
> 该方法本质是将远程仓库完全的拷贝到本地, 而不是一个特定的版本, 会比较大. 但是可以使用 *`git clone -b [branch] <url>`* 下载指定的分支

# 目录了解
- 工作区: 一般来说在进行仓库创建的时候, 我们会mkdir一个新的目录用于管理, 或者`git clone`的时候也会默认将拷贝下来的数据放在一个单独的目录中. 
- 暂存区: 英文名stage或者index, 位于`.git`目录下的index文件中, 也将暂存区叫做索引
- 版本库: git管理版本的仓库, 也就是`.git`目录

# 基本选项
- init: 初始化
- clone: 克隆拷贝
- status: 状态查看
> 查看上一次提交后是否有改变, 使用`-s`参数获得精简输出
- add: 添加暂存
> 将文件添加到暂存区, 使用`-p`参数选择添加文件部分内容

![git各区域和命令的关系](../picture/git_relationship_of_region_and_commond.png)