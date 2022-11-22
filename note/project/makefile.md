# 什么是Makefile
供make调用的，提供当前工程编译链接的规则，描述当前工程中所以文件的编译顺序、编译规则。

# 如何生成Makefile
1. 自行编写
2. AutoTools
3. Cmake
4. Scons
5. 其他自动生成工具

# 编写规则
## 基本格式
Makefile其实是由一系列规则组成的。每一条规则形式如下：
```make
<target> : {prerequisites}
[tab]  {commands}
```
1. target： 必选，改规则的命名
2. prerequisites：实现规则的依赖条件，一般为其他的target，会先执行相应的target
3. commands：实现规则所需要的指令，与上述`prerequisites`可以只拥有一个，也可以同时拥有

### target
`target`可以是文件名也可以只是某一个操作的名字，也即`伪目标(phony target)`，
可以使用`.PHONY : target`申明为伪目标。

> Tips: make命令可以指定执行某一个target。当没有指定时，会执行Makefile文件中的第一个目标

### prerequisites
前置条件可以为多个，此时使用空格隔开

### commands
可以由一行或者多行shell命令组成，但是不同行的命令会在单独的shell中执行，解决方法有三种：
1. 写在一行，中间用`;`隔开。
2. 使用`\`转义换行符，但是依然需要使用分号隔开
3. 使用`.ONESHELL`修饰，但是只有GNU make支持，是非POSIX的，而且是全局的

# 语法
## 注释
使用`#`表示注释, 用法形如C语言中的`//`

## 回显
默认情况下，make会打印每一条命令，然后再执行，这就是回显(echoing),
可以在命令之前加上`@`关闭回显，如：

`$ cat Makefile`
> ```make
> all:
>     echo "hello"
>     @echo "world"
> ```

`$ make`
> ```make
> echo "hello"
> hello
> world
> ```

## 通配符
Makefile的通配符与bash一致。

## 模式匹配
make支持对文件名运行类似正则运算匹配，主要用到的是`%`。
如当前工程存在a.c和b.c两个文件，可以按照如下方式实现：
> ```make
> %.o : %.c
> ```
其效果等效于：
> ```make
> a.o : a.c
> b.o : b.c
> ```

## 变量与赋值
可以使用`=`自定义变量。
由于同时存在make的变量与shell的变量，在shell命令中同时使用两个`$`符号，后者被转义。
在shell中使用Make变量需要将变量放在`$()`中。

`$ cat Makefile`
> ```make
> var = hello world
> all: 
> 	@echo $(var); \
> 	aaaa=100; \
> 	echo $${aaaa}
> ```

`$ make`
> ```
> hello world
> 100
> ```

> Tips: 限定变量生效范围方法：`target : var = value`，其中`target`可以使用模式匹配方式指定多个

### 赋值符号
make中的变量可以在定义时扩展(静态扩展)，也可以在运行中扩展(动态扩展)，为了便于使用，
提供了四种赋值运算符: `=`, `:=`, `?=`, `+=`。
1. `=`: 在执行时扩展，会递归的扩展
2. `:=`: 在定义的时候扩展
3. `?=`: 只有当没有被赋值时才会赋值
4. `+=`: 将值追加到末尾

> Tips: Make存在大量内置的变量，详见gnu官网：
> [Make内置变量](https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html)

# 注意事项
## 缩进使用空格还是Tab?
Makefile中其实分为两种语言格式
1. recipe: 为需要执行的shell命令，这种情况下必须使用Tab作为缩进
2. non-recipe: 为Makefile自身的语法，这种情况下其前面一般不能有Tab

> Tips: 当GNU make版本>=3.82时，可以使用`.RECIPEPREFIX`重新指定特殊字符替代Tab的功能
