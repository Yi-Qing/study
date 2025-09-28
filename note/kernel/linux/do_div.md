# 64位除法
编译过程中出现问题
```bash
ERROR: "__aeabi_uldivmod" [xxx.ko] undefined!
```
此处问题在于，目标平台为32bit，而64位除法需要使用专门的除法操作执行，
linux内核中一般使用`do_div()`，当然了，linux自然有有linux的快乐，在头文件`linux/math64.h`当中已经有了解决方案。
