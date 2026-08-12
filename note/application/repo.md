# repo
repo是用于管理大型代码仓库的工具，其本质就是一个Git仓库，然后又下辖了更多的子仓库。
repo的安装和使用请参考[官方文档](https://source.android.com/setup/develop/repo)，
下面是一些常用的命令。
> 温馨提示：想要更好的了解本篇文章，需要对`git`和`xml`有一定的了解。

## 初始化
```bash
repo init -u <main-manifest-url> [-b branch] [-m manifest-name]
```
- main-manifest-url：指向manifest文件的URL，本质也是一个`git`仓库。
- branch：指定manifest文件中的分支。
- manifest-name：指定manifest文件的文件名，默认为default.xml。

## 同步拉取所有子仓库变更
```bash
repo sync
```
- 可以通过`-j`参数指定同步的线程数，也可以在`manifest.xml`文件中设置`sync-j`属性指定线程数。
- 通过添加`-l`参数可以从本地缓存中拉取，而不用从远程仓库拉取，一般用于压缩SDK然后提供给其他开发者。
- 通过添加`-c`参数控制仅获取服务器中的当前清单分支。

## 所有子仓库统一执行某一条命令
```bash
repo forall -c "git log -1"
```

## 列出所有子仓库
```bash
repo list
```

## 查看子仓库的变更
```bash
repo status
```

## start
等同于执行`git checkout -D <branch name>`，用于从当前分支创建新的分支。

## upload
这个命令一般不用，因为必须通过`gerrit`审查，但又不是使用`repo`管理的项目都需要审核。
> 使用`git push`来代替。

# manifes.xml
manifest文件是repo的核心，它定义了仓库的结构，包括子仓库的位置、分支、是否需要拉取等。
他的格式是一个xml文件，那么首先来看一下`xml`文件的基本写法
```xml
<?xml version="1.0" encoding="UTF-8"?>
<top_tag>
    <sub_tag value1 value2 />
</top_tag>
```
- `<?xml version="1.0" encoding="UTF-8"?>`：xml文件的版本和编码。
- `<top_tag>`：标签的名称。
- `</top_tag>`：标签的结束。
- `<sub_tab>`：标签的属性。
- `value`：标签的属性值。

对于不包含子内容的标签，可以按照自闭合的方式简写成：
```xml
<tagname value />
```
> 上面示例中的`top_tag`标签就是一个闭合标签，内部存在子标签`sub_tag`，用`</top_tag>`闭合。

如果有现有项目，可以使用如下命令导出一个配置来查看：
```bash
repo manifest -o generated_manifest.xml
```

## 示例
```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
    <remote name="name" fetch="fetch_url" revision="revision" review="url_of_gerrit_server" />

    <default sync-j="4" />

    <project name="platform/frameworks/base" path="frameworks/base" revision="main" />
    <project name="platform/packages/apps/Settings" path="packages/apps/Settings" revision="main" >
        <linkfile src="packages/apps/Settings/res/values/strings.xml" dest="frameworks/base/core/res/res/values/strings.xml" />
        <copyfile src="packages/apps/Settings/res/values/strings.xml" dest="frameworks/base/core/res/res/values/strings.xml" />
    </project>

    <include name="additional-manifest.xml" />
</manifest>
```

### remote
`remote`标签定义了远程仓库的名称和URL，后续project标签中使用`remote`标签的名称来引用远程仓库。
> 可以存在多个`remote`标签，每个`remote`标签都可以指定不同的名称、URL、Gerrit服务器的URL。
- name：远程仓库的名称，后续project标签中使用`remote`标签的名称来引用远程仓库。
- fetch：远程仓库的URL，可以是相对路径或绝对路径，如果是相对路径，则是相对于manifest仓库所在的目录。
- review：Gerrit服务器的URL，主要是上面所说的`upload`命令使用，审查代码的。

### default
`default`标签定义了默认的配置，后续project标签中如果没有指定，则使用`default`标签指定的配置。
- sync-j：默认的同步线程数。
- depth：默认的克隆深度。

### project
`project`标签定义了子仓库的名称、路径、分支、是否需要拉取等。
> 一般都存在多个`project`标签，每个`project`标签都可以指定不同的名称、路径、分支、是否需要拉取等。
- groups：子仓库所属的组，后续可以方便对一个组的所有仓库进行管理。
- name：子仓库的名称，配合`remote`标签最终组合成完整的URL: `fetch_url/name`。
- path：子仓库检出后的路径，是一个相对于`.repo`目录的相对路径。
- remote：远程仓库的名称，也就是在`remote`标签中定义的名称。
- revision：子仓库的分支/标签/commit。
- linkfile：链接文件，将一个文件链接到另一个文件。
- copyfile：复制文件，将一个文件复制到另一个文件。

### include
包含另外一个manifest文件，可以方便的复用多个manifest文件，路径是相对当前manifest文件的相对路径。
> 可以存在多个`include`标签，每个`include`标签都可以指定一个manifest文件。

# 修改现有repo
如果已经存在一个repo仓库，那么我们可以修改它的`manifes.xml`文件来管理它的子仓库。

## 修改revison
如果我们想修改某个子仓库的分支，那么我们可以直接在`manifes.xml`文件中修改`revision`属性的值。

## 更新git信息
如果我们不想指定`revision`，只使用`default`的配置，那我们在本地需要修改`git`配置中的`refs`。
```bash
git update-ref refs/remotes/<remote_name>/<branch_name> <new_commit_id>
```
> 这里的`remote_name`就是`remote`标签中定义的名称，`branch_name`就是`project`标签中定义的分支名。

有时候在`.git/packed-refs`中打包存储了`refs`信息，可以通过如下命令更新：
```bash
git pack-refs --all
```

# 构建自己的repo
既然我们已经知道repo本质就是通过一个`manifes.xml`文件来管理多个Git仓库，
那么我们就可以自己构建自己的repo，只需要去创建一个本地仓库，然后在里面创建一个`manifes.xml`文件，
然后就可以通过`repo init`命令来初始化这个仓库，然后就可以通过`repo sync`命令来同步仓库了。
> 注意：repo的远程仓库只能是裸仓库，不能是普通仓库。
```tree
.
├── local
│   ├── hello
│   │   └── hello
│   └── world
│       └── world
└── remote
    ├── manifests.git
    │   ├── config
    │   ├── description
    │   ├── HEAD
    │   ├── hooks
    │   ├── info
    │   ├── objects
    │   ├── packed-refs
    │   └── refs
    └── t113
        ├── hello.git
        └── world.git
```

此时相应的xml配置文件如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
    <remote name="origin" fetch="./t113" />
    <default revision="master" remote="origin" sync-j="4" />

    <project path="hello" name="hello" />
    <project path="world" name="world" />
</manifest>
```

可以使用如下命令获取：
```bash
repo init -u ../remote/manifests.git/ -b master -m test.xml --no-clone-bundle --depth=1
```

# 从现有的移植成新的
## 同步逻辑
通过`repo forall -c 'echo "$REPO_PROJECT|$REPO_PATH|$REPO_RREV"'`获取每一个仓库的相关信息：

1. REPO_PROJECT：该仓库 git 本身路径
2. REPO_PATH：同步后将相应源码存放在哪里
3. REPO_RREV：当前仓库所在的 commit 信息

记录下所有相关信息后就可以去`REPO_PATH`目录执行`git push`推送到服务器上的`REPO_PROJECT`路径仓库了。

这里之所以需要`REPO_RREV`，是因为`rk`的 SDK 存在部分仓库根据不同的`tag`迁出到不同的目录，或者明明会将同一个仓库的同一个`REPO_RREV`推送到不同路径，这时候直接推送就会比较麻烦，我选择了如下逻辑：

1. 如果不存在同一个`REPO_PROJECT`对应多个`REPO_PATH`的情况，正常推送
2. 如果同一个`REPO_PROJECT`的同一个`REPO_RREV`同步到多个`REPO_PATH`，只推送一个
3. 否则推送的时候修改最终路径，在路径最后添加`REPO_RREV`:

```
  linux/poky | yocto/meta-browser | refs/tags/meta-browser-linux-6.1-stan-rkr5
  linux/poky | yocto/meta-clang | refs/tags/meta-clang-linux-6.1-stan-rkr5
  linux/poky | yocto/meta-openembedded | refs/tags/meta-openembedded-linux-6.1-stan-rkr5
  linux/poky | yocto/poky | refs/tags/poky-linux-6.1-stan-rkr5
```

```
  linux/poky-meta-browser | yocto/meta-browser | refs/tags/meta-browser-linux-6.1-stan-rkr5
  linux/poky-meta-clang | yocto/meta-clang | refs/tags/meta-clang-linux-6.1-stan-rkr5
  linux/poky-meta-openembedded | yocto/meta-openembedded | refs/tags/meta-openembedded-linux-6.1-stan-rkr5
  linux/poky-poky | yocto/poky | refs/tags/poky-linux-6.1-stan-rkr5
```

## 同步脚本
这里使用`python`写了一个相应脚本：

```python
#!/usr/bin/env python3
import os
import subprocess
import sys
from pathlib import Path
from collections import defaultdict

# ======================== 配置区 ========================
# 存放所有 bare 仓库的根目录
BARE_BASE = "rk3506"
REMOTE_IP = "git@192.168.20.25"
REMOTE_PREFIX = f"ssh://{REMOTE_IP}/~"
REPO_CMD = "repo"

def get_project_info():
    """调用 repo forall 获取所有项目的 (project, path, rrev)"""
    cmd = [REPO_CMD, "forall", "-c", 'echo "$REPO_PROJECT|$REPO_PATH|$REPO_RREV"']
    try:
        output = subprocess.check_output(cmd, text=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running repo forall: {e}")
        sys.exit(1)

    projects = []
    for line in output.strip().splitlines():
        if not line.strip():
            continue
        parts = line.strip().split('|')
        if len(parts) != 3:
            continue
        project, path, rrev = parts
        projects.append((project, path, rrev))
    for project, path, rrev in projects:
        print(f"  {project} | {path} | {rrev}")
    print("\n")
    return projects

def deal_project_info(raw):
    """检查是否有重复的REPO_PROJECT，对于重复的，修改项目名为 $(project)/$(basename rrev)"""
    project_to_paths = defaultdict(list)
    for project, path, rrev in raw:
        project_to_paths[project].append((path, rrev))

    projects = []
    for project, paths in project_to_paths.items():
        if len(paths) == 1:
            projects.append((project, *paths[0]))
        else:
            """如果rrev也一致，则删除后续的项目"""
            rrev_set = set(rrev for _, rrev in paths)
            if len(rrev_set) == 1:
                print(f"  [Warning] {project} has multiple paths with the same rrev, using the first one")
                projects.append((project, *paths[0]))
                continue
            for i, (path, rrev) in enumerate(paths):
                """rrev仅保留`-linux`前面的内容，比如rk3506-cn-linux-6.1-stan-rkr4.4 -> rk3506-cn"""
                tmp=os.path.basename(rrev)
                tmp=tmp.split("-linux")[0]
                target_name = f"{project}-{tmp}"
                print(f"  [Warning] {project} has multiple paths with different rrev, using {target_name}")
                projects.append((target_name, path, rrev))

    """打印"""
    print("\n")
    for project, path, rrev in projects:
        print(f"  {project} | {path} | {rrev}")
    print("\n")

    return projects

def is_shallow_repo():
    """判断当前仓库是否为浅克隆"""
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--is-shallow-repository"],
            capture_output=True, text=True, check=False
        )
        return result.stdout.strip() == "true"
    except Exception:
        return False

def restore_ref(ref_value):
    """恢复到之前记录的引用"""
    subprocess.run(["git", "checkout", ref_value], check=True)

def push_project(path, bare_repo_path, project, rrev):
    """
    进入项目目录，根据是否浅克隆选择推送策略。
    浅克隆：创建孤儿分支，压缩为一次提交并强制推送。
    完整克隆：直接推送当前 HEAD 到 master。
    """
    os.chdir(path)
    bare_path = Path(bare_repo_path)
    if not bare_path.exists():
        print(f"Creating bare repo: {bare_repo_path}")
        subprocess.run(["ssh", REMOTE_IP, "init-bare", bare_repo_path], check=True)
        # subprocess.run(["git", "init", "--bare", bare_repo_path], check=True)

    shallow = is_shallow_repo()
    try:
        if shallow:
            print(f"  [Shallow] Creating orphan branch and force push")
            try:
                old_ref=subprocess.run(["git", "rev-parse", "HEAD"], capture_output=True, text=True, check=True).stdout.strip()
                tmp=os.path.basename(rrev)
                subprocess.run(["git", "checkout", "--orphan", "temp-branch"], check=True)
                subprocess.run(["git", "add", "-A"], check=True)
                print(f"  Squashing commit from {project} at {rrev}")
                subprocess.run(["git", "commit", "-m", tmp], check=True)
                subprocess.run(
                    ["git", "push", f"{REMOTE_PREFIX}/{bare_repo_path}", "HEAD:refs/heads/master", "--force"],
                    check=True
                )
            finally:
                # 恢复原始引用
                restore_ref(old_ref)
                # 删除临时分支（如果存在）
                subprocess.run(
                    ["git", "branch", "-D", "temp-branch"],
                    stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL
                )
        else:
            print(f"  [Full] Pushing complete history to master")
            subprocess.run(
                ["git", "push", f"{REMOTE_PREFIX}/{bare_repo_path}", "HEAD:refs/heads/master"],
                check=True
            )
        print(f"Successfully pushed {path}")
    except subprocess.CalledProcessError as e:
        print(f"Failed to push {path}: {e}")
        raise

def main():
    raw = get_project_info()
    items = deal_project_info(raw)
    curr_path = os.getcwd()

    # 开始推送
    for project, path, rrev in items:
        bare_repo_path = os.path.join(BARE_BASE, project)
        print(f"\nPushing {project} to {bare_repo_path}")
        tmp_path=os.path.join(curr_path, path)
        print(f"  [Path] {tmp_path}\n")
        push_project(tmp_path, bare_repo_path, project, rrev)

if __name__ == "__main__":
    main()

```

# 服务器配置
因为需要考虑多人共享使用，所以上面的脚本需要依赖一个服务器，正常来说还需要搭建`git`服务，
但是考虑到只是为了共享，没必要弄那么复杂，直接`裸仓库`就行。所以只需要创建一个`git`用户，
给该用户简单配置一下`.gitconfig`和`.ssh/authorized_keys`就可以了。

```config
[init]
    defaultBranch = master
[color]
    ui = auto
[receive]
    denyNonFastForwards = true		# 禁止 force push
    denyDeletes = true				# 禁止删除分支
```

考虑到安全性，还需要配置该用户不可登录：

```bash
sudo useradd -m git
sudo chsh /usr/bin/git-shell git
sudo mkdir /home/git/git-shell-commands
```

但是这样一来就无法通过本地机器远程创建仓库了，所以还需要在`/home/git/git-shell-commands`创建一个脚本`init-bare`来支持调用：

```bash
#!/bin/bash

# Usage: ssh git@server init-bare <repo-name>

if [ -z "$1" ]; then
    echo "Usage: $0 <repo-name>"
    exit 1
fi

REPO_NAME="$1.git"
REPO_PATH="/home/git/${REPO_NAME}"

if [ -d "$REPO_PATH" ]; then
    echo "Warring: Repository $1 already exists."
else
    git init --bare "$REPO_PATH"
fi

echo ""
echo "Repository $1 created successfully."
echo "Clone URL: ssh://git@$(hostname -I | awk '{print $1}')${REPO_PATH}"
```

之后只需要在本地机器上执行：`ssh git@server-ip init-bare manifest`即可在`git`用户家目录创建`manifest.git`裸仓库。

完成服务器配置后，就可以到对应的 SDK 顶层目录通过上面的同步脚本同步 SDK 到服务器上了。

> 注意：服务器内存需要大一点，之前测试的时候分配 4G 内存发现部分仓库拉不下来，`oom`了。

## manifest.xml
以 RK3506 的 SDK 为示例，可以在 SDK 的顶层目录执行`repo manifest -o manifest.xml`获取当前配置下的配方文件。之后需要修改如下部分：

1. fetch 属性：根据同步脚本的配置，这里需要修改为`./rk3506`
2. default 属性：添加`revision="master"`值，固定拉取`master`分支
3. 删除特定仓库下的`revision`属性。
4. 根据同步时部分修改了名字的仓库，更改配方文件并`push`到服务器的`manifest.git`。

## 使用
1. 创建一个空目录
2. 执行`repo init ssh://git@192.168.20.25/~/manifest.git -m rk3506.xml`
3. 执行`repo sync`同步。




