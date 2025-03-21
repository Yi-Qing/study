# 基本使用
## docker配置
```jsonc
{
  // File: /etc/docker/daemon.json
  // 开启用户命名空间，此时如果容器需要主机的root权限，可以在转换为容器的时候添加参数：
  // --userns=host / --privileged / --cap-add=XXX
  "userns-remap": "default",    // 如果想使用特定用户开辟命名空间，把default改成对应用户名即可
  // 镜像仓库地址
  "registry-mirrors": [
    "https://docker.xuanyuan.me",
    "https://docker.1ms.run",
    "https://hub-mirror.c.163.com",
    "https://docker.m.daocloud.io"
  ]
}
```

## 镜像获取
```bash
docker pull <镜像名称>:[版本号]
```
其中，版本号可以省略，默认获取最新版本。

## 镜像删除
```bash
docker rmi <镜像名称>:[版本号]
```

## 容器运行
```bash
docker run -it --name <容器名称> <镜像名称>:[版本号] [容器启动命令]
```
> 除了上面的`docker配置`中启用用户命名空间来运行容器外，
> 还可以添加`-u`参数指定运行容器的用户，如`-u root`。
>> 与之不同的是，命名空间后再容器中还是`root`，只是在宿主机上只有用户权限。

## 容器卷挂载
```bash
docker run -it -v <宿主机目录>:<容器目录> --name <容器名称> <镜像名称>:[版本号] [容器启动命令]
```
需要注意，权限问题很麻烦，当启用了`用户命名空间`时，会根据上面配置的`userns-remap`来映射用户，
分配规则查看`/etc/subuid/`和`/etc/subgid`。容器中的`root`用户其`uid`是`/etc/subuid`中的第一个，
一般是从`165536`开始的数字，而该`uid`其实在宿主机并不存在，也就是说，当挂载一个宿主机中的目录到容器，
该目录中存在的东西默认情况下，容器中的`root`用户是无法访问修改的，毕竟对于内核来说，
一个`uid`为`165536`的用户是不可能有权限去访问一个属于`uid`为`1000`的`600`权限的文件的。
> 有一种办法就是，修改`/etc/subuid`和`/etc/subgid`，让他从对应用户的`uid`开始分配，最大分配一个`uid`。
> 这样，容器中的`root`用户就有权限访问挂载的目录了。

> 还有一种办法，新建一个`gid`为`165536`的组，把共享目录所属用户加入到这个组，
> 然后递归的把当前目录中所有的东西修改为这个组所有，再给他们添加组可读写权限。
> 其中`gid`可以在容器中执行`cat /proc/$$/gid_map`查看
```bash
sudo groupadd -g 165536 share_group
sudo usermod -aG share_group $USER
su $USER
chown -R :share_group /path/to/shared/directory
chmod -R g+rwX /path/to/shared/directory
```
>> 这里使用大写的`X`，表示如果当前文件已经有`x`权限或者是一个目录，否则不添加`x`权限。

## 容器列表
```bash
docker ps -a
```

## 容器停止
```bash
docker stop <容器名称>
```

## 容器删除
```bash
docker rm <容器名称>
```

## 容器转换为镜像
```bash
docker commit <容器名称> <镜像名称>:[版本号]
```

## 镜像导出
```bash
docker save -o <文件名>.tar <镜像名称>:[版本号]
```

## 镜像导入
```bash
docker load -i <文件名>.tar
```

# 示例
以配置一个`ubuntu`的`18.04`镜像为例，步骤如下：

## 1. 获取镜像
```bash
docker pull ubuntu:bionic-20230530
```

## 2. 运行容器
```bash
docker_mount="-v $(pwd):/workspace -w /workspace"
name="u1804"
image="ubuntu:bionic-20230530"
docker run -itd $docker_mount --name $name --hostname $name $image /bin/bash
```
> ~~如果要`--ipc=host`同时使用命名空间，需要添加`-v /dev/mqueue:/dev/mqueue`等参数，否则会报错。~~

## 3. 配置容器
```bash
docker exec -it $image bash
apt update && apt install -y vim
exit
```

## 4. 停止容器
```bash
docker stop $name
```

## 5. 导出容器
```bash
docker commit $name export_image:V1.0.0
```

## 6. 导出镜像
```bash
docker save -o export_image.tar export_image:V1.0.0
```
