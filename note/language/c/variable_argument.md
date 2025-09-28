## 不定长参数实现

#### 对于函数而言：
在c标准库中，提供了几个宏帮助我们实现可变参数的函数定义与实现：
- 申明
> 在函数定义时可以使用c标准允许的一个参数占位符`...`来实现，比如：
> ```c
> int printf( const char* format, ...);
> ```

- 定义
> void va_start(va_list ap, last);  
> 该宏用于初始化`va_list`，以供后续的`va_arg`、`va_end`使用, `last`表示最后一个非可变长的参数，比如printf中的参数`format`。
> 
> type va_arg(va_list ap, type);  
> 这里`type`表示传入参数的类型，然后通过在这个类型来解析`ap`指针获取传入的这个参数，比如传入参数为`int`型，那么就会从`ap`指针指向地址获取后四个字节的空间组成`int`型数据。需要注意的是字节对齐问题，对于`char`型数据我们在解析的时候`type`需要按照`int`型来解析
>  
> void va_end(va_list ap);  
> 这里是对应于`va_start`宏的，用于将ap指针指向NULL，需要和`va_start`同时出现，用于收尾工作。
> 
> void va_copy(va_list dest, va_list src);  
> 这是拷贝ap指针的，很少用到。
>  
附上一篇博客：[用指针处理C语言中不定数目的函数参数](https://blog.csdn.net/cybra_shaoxing/article/details/15506?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

#### 对于宏而言
在`c99`标准以后，可以在宏定义中使用`__VA_ARGS__`实现不定数量参数的定义
对于代码调试的时候，就可以使用如下形式进行
```c
#ifdef DEBUG
    #define LOG(fmt, ...) printf("%s:%s+%d" fmt "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);
#else
    #define LOG(fmt, ...)
#endif
```