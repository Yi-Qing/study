## FakeVim
对于长时间使用vim模式进行编辑的人来说，IDE中支持vim编辑模式是很棒的，
而`Qt Creator`中则提供的默认的`FakeVim`模式。

`FakeVim`模式提供了调用系统`vimrc`的功能，但有时候不需要这么多功能，
此时`FakeVim`也提供了另外一个选择：`user commond mapping`，
用户可以通过IDE配置部分功能。

但是这个时候出现了一个问题，怎么调用该映射？通过
[search](https://stackoverflow.com/questions/15437559/how-to-set-user-command-in-qt-FakeVim)
发现还需要在IDE的键盘映射中绑定对于的映射，可搜索`UserAnction`查找。

## MSVC && UTF-8
对于`CMAKE`项目：
```cmake
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
```
> Tips: 需要放置在`add_executable()`之前才能生效

> Tips: 使用`/utf-8`会同步修改执行时的字符集，在未启用`UTF-8`的系统命令行上会在执行时乱码。
    推荐使用`/source-charset:utf-8`代替

对于`QMAKE`项目：
```qmake
msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
```
