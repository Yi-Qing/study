## reg
需要考虑进行设备寻址，则需要在dts中定义`reg`属性。为了解析该属性需要同时定义：
`#address-cells, #size-cells`，属性对应的值表示需要使用几个u32的cell来表示该属性。

```
    #address-cells = <1>;
    #size-cells = <0>;
    cpu@0 {
        compatible = "arm,cortex-a9";
        reg = <0>;
    };
```
表示使用一个cell来表示基地址，同时不需要分配空间，只需要一个单独的地址

```
    #address-cells = <1>;
    #size-cells = <1>;
    gpio@101f3000 {
        compatible = "arm,pl061";
        reg = <0x101f3000 0x1000
               0x101f4000 0x0010>;
    };
```
表示使用一个cell表示基地址，一个cell表示分配的空间大小，此处gpio分配的地址空间范围有两个，
分别是`0x101f3000 ~ 0x101f3fff`和`0x101f4000 ~ 0x101f400f`

```
    #address-cells = <2>
    #size-cells = <1>;
    ethernet@0,0 {
        compatible = "smc,smc91c111";
        reg = <0 0 0x1000>;
    };
```
此处为一个外部总线地址，其片选信号为0，片选基地址的偏移量为0，长度值为0x1000。

## interrupts
在设备树中，中断资源的描述有两种：  
常见的方式：
```
    interrupt-parent = <&gpio3>;
    interrupts = <7 0x0>;
```
对于存在多中断且其中断控制器不相同的情况更实用的方式：
```
    interrupts-extended = <&gpio3 7 0>;
```
