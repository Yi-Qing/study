# doc
描述项目相关文档

# inc
```bash
inc
├── libA
├── libB
├── moduleA
└── moduleB
```

不同`模块/外部库`导出供其他模块使用的头文件，按照模块划分子目录

# main
```bash
main
├── inc
└── src
```

## inc
main函数内部需要使用到的头文件

## src
main函数相关代码

# 4. libs
```bash
libs
├── share
└── static
```

## static
引用自外部的静态库文件

## share
引用自外部的动态库文件

# 5. modules
```bash
modules
├── moduleA
│   ├── inc
│   ├── src
│   └── test
└── moduleB
    ├── inc
    ├── src
    └── test
```

## inc
当前模块内部使用的头文件存放

## src
当前模块的实现代码

## test
实现简单的demo示例，用于检查本模块代码是否按照计划流程执行

# 6. format
描述代码风格相关文档以及配置

# 7. build
存放输出文件

