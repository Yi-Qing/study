# aarch64-linux-gnu-gcc问题
可能直接无法与该编译器编译出来的`compile_commands.json`工作
需要配置`.clangd`文件
```yaml
CompileFlags:
  -mabi=lp64
```
> 方式：直接查看clangd的log，发现报错：`Ignored diagnostic. unknown target ABI 'lp64'`
