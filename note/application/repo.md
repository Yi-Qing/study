# repo
repo是用于管理大型代码仓库的工具，其本质就是一个Git仓库，然后又下辖了更多的子仓库。
repo的安装和使用请参考[官方文档](https://source.android.com/setup/develop/repo)，
下面是一些常用的命令。
> 温馨提示：想要更好的了解本篇文章，需要对`git`和`xml`有一定的了解。

## 初始化
```bash
repo init -u <main-manifest-url> [-b branc] [-m manifest-name]
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

## upload
这个命令一般不用，因为需要通过`gerrit`审查，但是实际上又不是所以使用repo管理的项目都是安卓。

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
