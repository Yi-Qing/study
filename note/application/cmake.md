# 相关网址
1. [官网](https://cmake.org/)
2. [WIKI](https://gitlab.kitware.com/cmake/community/-/wikis/home)
3. [Documentation](https://cmake.org/documentation)
4. [官方例程](https://github.com/Kitware/CMake/tree/master/Help/guide/tutorial)

# language support
1. C/CXX (default)
2. ASM (if use this, enable at the last)
3. Fortran
4. CUDA (V3.8)
5. Swift (V3.15)
6. OBJC/OBJCXX (V3.16)
7. ISPC (V3.18)
8. HIP (V3.21)

# 例程解析
## Step1
```CMake
# 将 *.h.in 文件拷贝到输出目录中并修改名字
configure_file(buildCfg.h.in buildCfg.h)

target_include_directories(hello PUBLIC
    ${PROJECT_BINARY_DIR}
    )
```

```C
// 小写的hello表示当前CMakeLists.txt中project指定的项目名字
#define HELLO_VERSION_MAJOR @hello_VERSION_MAJOR@
#define HELLO_VERSION_MINOR @hello_VERSION_MINOR@
```