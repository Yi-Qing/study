## wsl找不到设备

有时候会在wsl里面执行`adb root`来启动`adb`服务，但是这时就会出现无法找到设备问题，
正确解决方法是再`cmd`或者`powershell`中执行`adb kill-server && adb root`重启adb

原因猜测：由于再wsl中使用的其实依然是windows下的adb，wsl中的adb只是起到桥的连接作用
但是wsl中调用`adb start-server`无法启动windows下的adb服务，导致出现这个问题

## signal
wsl1下使用无法实现`SA_RESTART`标志重启慢系统调用，怀疑为转译问题，win内核可能不支持
