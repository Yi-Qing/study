## 管脚连接
众所周知，spi基本为4线结构：
> 一根时钟线clk，用于时钟同步，保证数据的准确性，避免接收数据错乱  
> 一根片选线cs/ss，在一主多从的情况下，用于给主机选择与之通信的从机  
> 两个串行数据线，分别从主机发送数据到从机，从从机发送数据到主机

对于串行数据线，有以下两种写法：

1. 站在信号线角度，在连线时不用交叉连接，主机从机相同的命名线直接相连
> miso：master input，slave output，主机接收数据，从机输出数据  
> mosi：master output，slave input，主机输出数据，从机接收数据  
2. 站在器件角度，在连接时需要考虑交叉连接，主机从机不同命名线相连
> sdo/so：serial data output，该串行线用于输出  
> sdo/si：serial data input，该串行线用于输入

## 变种
截至`2022-06-10`，市面上存在如下变种：
1. dspi: 同样为四线，数据线不在限定方向，可同时两根数据线都是接收/输出
2. qspi: 增加两根数据线，数据线方向同`dspi`
3. xspi: 暂时没有细看，但是已经出现了[参考](https://www.arasan.com/product/xspi-master-ip/)