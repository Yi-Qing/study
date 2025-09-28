## wall time
我们日常使用时所使用的时间表示为`Wall clock time`，也即现实时间。
在linux中，`wall time`由系统维护，可由系统管理员任意更改，
在使用`gettimeofday`等读取的就是这个时间。

## monotonic time
记录了系统启动以来的时间。由`jiffies`变量记录，单调递增。
但其实在linux中并没有具体的变量直接记录该时间，而是通过公式：
`monotonic clock = wall time + wall_to_monotonic`计算而来。
`wall_to_monotonic`将在默认为0，但是在系统启动过程中，会在
`drivers/rtc/hctosys.c:rtc_hctosys`时修改：通过读取硬件RTC模块
来获取`wall time`的同时计算出`wall_to_monotonic`，其值一般为负数。

## example
之前在`qemu + imx6q-sabrelite`中通过`settimeofday`来配置时间，
使得时间倒流，但是无法成功，在内核中追溯发现无法通过`timespec64_compare`，
但是实际的硬件平台确实是可以成功的。后面发现就是在`rtc_hctosys`过程中
RTC得到的`wall time`一直为`unix 时间纪元：1970-01-01 00:00:00 UTC`。