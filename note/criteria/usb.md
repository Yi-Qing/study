## usb 描述符
对于hid设备的枚举通信而言，最主要的部分为四个描述符：
1. 设备描述符: 用于找到设备
2. 配置描述符: 用于获取设备配置信息
3. 接口描述符: 用于存储端点描述符
4. 端点描述符: 端点用于通信，可理解为linux下的文件描述符

对于这几个描述符，可以用下面这个栗子来简单的理解：  
你是一个设计师，有一天有所学校的校长找你，请你规划一栋教学楼(枚举到设备)，然后你有了几套设计方案(配置描述符)，
在其中的一套设计方案中，你将一部分房间作为了教室，一部分作为音乐室、美术室等等(各种接口描述符)，
其中某一间特定的教室(特定端点)自然是不可能同时是教室和办公室的。但是并不是说每间教室在这种配置下只能做单一工作，
比如考试的时候，教室可以变成考场，这就是接口的备用配置，也就是`interface`接口中`bAlternateSetting`的作用

所以自然而然地可以得出这几个结论：
1. 一个设备可以有多个配置，但同一个时刻只能有一个配置有效；
2. 每个配置下可以有一个或多个接口；
3. 每个接口下可以有一个或多个端点；
4. 同一个端点号不能出现在同一个配置下两个或多个不同的接口中；但是可以用在不同的配置中。

### 设备描述符
对于`libusb`的定义而言，其设备描述符信息可按照如下结构体解析，需要注意的是，该结构体对齐方式为单字节对齐
```
struct libusb_device_descriptor {
	uint8_t  bLength;               // 本结构体自身大小
	uint8_t  bDescriptorType;       // 本描述符类型，此处为 LIBUSB_DT_DEVICE
	uint16_t bcdUSB;                // 协议版本，遵循USB-IF
	uint8_t  bDeviceClass;          // 遵循USB-IF的 class code
	uint8_t  bDeviceSubClass;       // 遵循USB-IF 的subclass code
	uint8_t  bDeviceProtocol;       // 遵循USB-IF protocol code
	uint8_t  bMaxPacketSize0;       // 特殊端点 0 的最大传输包字节
	uint16_t idVendor;              // 遵循USB-IF的vendor id
	uint16_t idProduct;             // 遵循USB-IF的product id
	uint16_t bcdDevice;             // 遵循USB-IF的释放版本号，直接按照上面的bcdusb抄就行
	uint8_t  iManufacturer;         // 字符串描述符(描述制造商)索引
	uint8_t  iProduct;              // 字符串描述符(描述产品信息)索引 
	uint8_t  iSerialNumber;         // 字符串描述符(描述设备序列号)索引
	uint8_t  bNumConfigurations;    // 配置描述符数量
};
```

### 配置描述符
当获取到设备后就要开始获取设备配置，来了解具体的设备信息，配置描述符的大小为9byte，
但是在`libusb`中可以发现其大小是大于这个数值的：
```
struct libusb_config_descriptor {
	uint8_t  bLength;               // 配置描述符大小，为9byte
	uint8_t  bDescriptorType;       // 该描述符类型：LIBUSB_DT_CONFIG
	uint16_t wTotalLength;          // 总共应该获取的数据量大小，包括自身，所有的接口描述符，端点描述符，类特殊描述符
	uint8_t  bNumInterfaces;        // 该配置支持的接口描述符数量
	uint8_t  bConfigurationValue;   // 当前描述符对应的索引值
	uint8_t  iConfiguration;        // 描述此配置的字符串索引
	uint8_t  bmAttributes;          // 设备属性，是否支持远程唤醒，是否自供电
	uint8_t  MaxPower;              // 该配置下最高功耗，在高速模式下单位为2mA，在3.x对应的super speed下单位为8mA

	const struct libusb_interface *interface;   // 接口描述符数组，大小由bNumInterfaces决定
	const unsigned char *extra;                 // 额外的数据存放处
	int extra_length;                           // 额外数据大小
};
```
这里将结构体设置为这样是由于我们在获取配置描述符的时候会同时按照配置描述符中的`wTotalLength`获取其他的描述符数据。
再看下位机分配的配置描述符空间，也可以发现，配置描述符的大小其实是包含了其他一些描述符的。

### 接口描述符
存储一些接口相关的信息，在`libusb`中，其结构体也如配置描述符一般，存储了端点描述符数组，
这里同配置描述符一样，结构体大小也是大于实际描述符的。
```
struct libusb_interface_descriptor {
	uint8_t  bLength;                   // 该描述符大小
	uint8_t  bDescriptorType;           // 描述符类型，此处为 LIBUSB_DT_INTERFACE
	uint8_t  bInterfaceNumber;          // 当前接口描述符索引值
	uint8_t  bAlternateSetting;         // 接口备用功能，实现一个接口，多种功能的能力 
	uint8_t  bNumEndpoints;             // 接口端点数量
	uint8_t  bInterfaceClass;           // 遵循USB-IF的 class code
	uint8_t  bInterfaceSubClass;        // 遵循USB-IF的 subclass code
	uint8_t  bInterfaceProtocol;        // 遵循USB-IF的 protocol code
	uint8_t  iInterface;                // 描述此接口的字符串索引

	const struct libusb_endpoint_descriptor *endpoint;  // 端点描述符数组
	const unsigned char *extra;         // 额外数据存放处
	int extra_length;                   // 额外数据大小
};
```

### 端点描述符
在libusb中，端点描述符可用以下结构体描述：
```
struct libusb_endpoint_descriptor {
	uint8_t  bLength;					// 描述符大小
	uint8_t  bDescriptorType;			// 描述符类型
	uint8_t  bEndpointAddress;			// bit0:3表示端点号，bit7表示端点数据方向
	uint8_t  bmAttributes;				// bit0:1表示传输类型，bit2:3仅在同步端点时生效，表示同步类型，
										// bit4:5也仅在同步端点时生效，表示同步用法。
	uint16_t wMaxPacketSize;			// 表示该端点一次最大传输数据量
	uint8_t  bInterval;					// 轮询传输的时间间隔
	uint8_t  bRefresh;					// 仅适用音频设备，提供同步反馈的速率
	uint8_t  bSynchAddress;				// 仅适用音频设备，提供同步端点地址

	const unsigned char *extra;			// 额外数据存放处
	int extra_length;					// 额外数据大小
};
```
对于端点描述符，其中的IN/OUT都是对于host端来说的
