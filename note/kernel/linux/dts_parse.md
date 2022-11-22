## 设备树中哪些节点会被编译为platform_device
1. 根节点下含有compatile属性的子节点，会转换为platform_device
2. 含有特定compatile属性的节点的子节点，会转换为platform_device。
    如果一个节点的compatile属性，它的值是这4者之一："simple-bus","simple-mfd","isa","arm,amba-bus",
    那么它的子结点(需含compatile属性)也可以转换为platform_device。
3. 总线I2C、SPI节点下的子节点：不转换为platform_device。
    某个总线下到子节点，应该交给对应的总线驱动程序来处理, 它们不应该被转换为platform_device。