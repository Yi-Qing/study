# 无法和clangd合作
部分版本的`bear`构建出来的`compile_commands.json`文件中，编译器的描述是`cc`，
但是通过多方资料查找和可用的`compile_commands.json`对比，发现编译器应该描述为实际编译器，
如`arm-linux-gnueabi-gcc`或者使用完整路径。

> 最好的解决方法是从源码编译一个高版本的，如`ubuntu 1804`可以编译一个`2.4.3`版本的。
