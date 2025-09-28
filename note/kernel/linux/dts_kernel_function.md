## 环境
- linux 4.4.78
- qcom,sdm636

## 设备树作用与意义
应该都知道dts是因为之前arm架构下各厂商都把各个单板的板级信息放在kernel里面，
代码贼多，而且对kernel来讲全是垃圾，于是社区借鉴PowerPC等体系使用的`FDT`搞出了`DT`，
成功将大部分板级信息放入了`DTS`

整体来讲，`DTS`主要是完成了三件事：
1. 识别machine(platform identification)
2. 运行信息提取与管理(runtime configuration)
3. 生成设备信息(device population)

### machine识别
1. 从汇编代码跳转至`init/main.c`中函数`start_kernel`
2. 在函数`start_kernel`中调用`arch/arm/kernel/setup.c`中函数`setup_arch`
3. 在函数`setup_arch`中调用`arch/arm/kernel/devtree.c`中函数`setup_machine_fdt`
4. 在函数`setup_machine_fdt`中，通过`mdesc_best`指定检索到的机器信息存放位置
5. 在函数`setup_machine_fdt`中执行`drivers/of/fdt.c`中函数`of_flat_dt_match_machine`

### 设备信息生成
对于使用了设备树的kernel来说，需要将设备信息从`dtb`中解析出来，其入口可能有很多，
但是都会调用相同的接口：`of_platform_populate`。

#### of_platform_populate
```
{
	struct device_node *child;
	int rc = 0;

	root = root ? of_node_get(root) : of_find_node_by_path("/"); // 根节点，虽然也可以从非根节点开始
	if (!root)
		return -EINVAL;

	for_each_child_of_node(root, child) {   // 遍历根节点下字节点
		rc = of_platform_bus_create(child, matches, lookup, parent, true);
		if (rc) {
			of_node_put(child);
			break;
		}
	}
	of_node_set_flag(root, OF_POPULATED_BUS);

	of_node_put(root);
	return rc;
}
```