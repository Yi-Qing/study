## union
使用联合体传输浮点数：
```
union f_data 
{
	float f;
	struct
	{
		unsigned char byte[4];
	};
}
```
union Regxxx{
	uint32_t reg;	// 代表整体的32bit寄存器
	struct {		// 通过位域实现寄存器对应功能操控, 再见了，与或操作
		uint32_t reserve:4;
		uint32_t ctrl:4;
		uint32_t enable:5;
		uint32_t dis:3;
		uint32_t stat:1;
		uint32_t loop:7;
		uint32_t ext:2;
		uint32_t mode:6;
	}bits;
};
```
