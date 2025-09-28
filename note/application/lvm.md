# lvm
主要分为以下几个部分：
1. 物理卷(Physical Volume)
2. 卷组(Volume Group)
3. 逻辑卷(Logical Volume)

他们的关系如下图所示：
```
+-------------------------------------+
|            卷组 (VG)                 |
|      (例如: vg_data)                 |
|                                     |
|  +------------+   +--------------+  |
|  | 物理卷 (PV) |   |  物理卷 (PV)  |  |
|  | (/dev/sda1)|   | (/dev/sdb1)   | |
|  +------------+   +--------------+  |
|                                     |
|  +-----------------------------+    |
|  |        逻辑卷 (LV)        |       |
|  |    (例如：lv_data)        |       |
|  +-----------------------------+    |
|                                     |
|    (逻辑卷上可以创建文件系统)           |
+-------------------------------------+
```

# 物理卷
- 定义: 物理卷是实际的存储设备或其分区，例如`硬盘`、`SSD`或`RAID`设备。它们是`LVM`管理的基础。
- 作用: 物理卷被`LVM`识别并用作存储的基础单元。可以将多个物理卷组合到一起，以形成卷组。
- 示例: 比如`/dev/sda1`、`/dev/sdb1`等都可以是物理卷。
> 需要注意的是，物理卷的大小并不等于物理设备的大小，而是分区大小，这也是这里为什么是`sda1`，而不是`sda`

通过下面几个命令管理物理卷：
- `pvcreate`: 创建物理卷
- `pvremove`: 删除物理卷
- `pvresize`: 调整物理卷大小
- `pvscan`: 扫描系统中的物理卷
- `pvdisplay`: 显示物理卷信息

# 卷组
- 定义: 卷组是一个或多个物理卷的集合。它将多个物理存储设备整合成一个逻辑存储池。
- 作用: 卷组是`LVM`管理的中间层，用户可以在卷组内创建和管理逻辑卷。卷组允许用户将物理卷的空间聚合在一起，从而实现更大的灵活性和扩展性。
- 示例: 可以创建一个名为`vg_data`的卷组，包含`/dev/sda1`和`/dev/sdb1`这两个物理卷。

通过下面几个命令管理卷组：
- `vgcreate`: 创建卷组
- `vgremove`: 删除卷组
- `vgscan`: 扫描系统中的卷组
- `vgdisplay`: 显示卷组信息
- `vgreduce`: 减少卷组中的物理卷
- `vgextend`: 增加卷组中的物理卷
- `vgchange`: 修改卷组属性

# 逻辑卷
- 定义：逻辑卷是从卷组中分配出来的存储空间。逻辑卷可以被视为传统文件系统中的分区。
- 作用：逻辑卷是 LVM 提供给用户使用的存储单元。用户可以在逻辑卷上创建文件系统，进行数据存储和访问。逻辑卷的大小可以动态调整，支持在线扩展和缩小。
- 示例：在卷组 vg_data 中可以创建一个名为 lv_data 的逻辑卷。

通过下面几个命令管理逻辑卷：
- `lvcreate`: 创建逻辑卷
- `lvremove`: 删除逻辑卷
- `lvresize`: 调整逻辑卷大小
- `lvscan`: 扫描系统中的逻辑卷
- `lvdisplay`: 显示逻辑卷信息
- `lvextend`: 扩充逻辑卷
- `lvreduce`: 缩减逻辑卷

# ext4
除了逻辑卷以外，我们还需要一个对应的文件系统来存储数据。常见的有`ext4`、`xfs`、`btrfs`等。
但目前使用`ext4`是最多的，此处也只考虑该种方式。

通过如下命令重新配置文件系统大小：
```bash
sudo resize2fs /dev/your_vg/your_lv
```

# swap
默认情况下，ubuntu安装过程中，如果选择lvm模式，会自动创建swap分区在lvm逻辑卷上。
如果不需要swap分区，可以执行以下命令关闭swap：
```bash
sudo swapoff /dev/your_vg/your_swap
```
但是除此之外，还需要关注几个地方：
1. /etc/fstab文件中，swap分区的挂载信息需要注释掉。
2. /etc/initramfs-tools/conf.d/下的配置文件中，swap分区的配置也需要注释掉。
3. 更新initramfs文件，使修改生效: `sudo update-initramfs -c -k all`

# 扩展
扩展总共分为两种情况，一种是新增一个物理硬盘，一种是单纯增加物理卷的容量。

## 增加物理卷容量
这种情况比较麻烦，需要重新分区，把需要增加的容量分配给物理卷所对应的分区。
```bash
# 1. 使用gparted为sda1扩容
sudo gparted /dev/sda1

# 2. 扩展物理卷
sudo pvresize /dev/sda1

# 3. 扩展逻辑卷
sudo lvextend -l +100%FREE /dev/your_vg/your_lv

# 4. 重新调整ext4文件系统大小
sudo resize2fs /dev/your_vg/your_lv

# 5. 更新initramfs文件
sudo update-initramfs -c -k all

# 6. 更新grub文件
sudo grub-mkconfig -o /boot/grub/grub.cfg
```
> 在没有一定实力以前，不要用`fdisk`调整分区，而是用`gparted`来调整，否则系统可能会挂掉。
>> grub-probe: error: disk `lvmid/CT6MUC-2Vig-PC6n-ZvhZ-W5IB-GxYm-X2M8Z1/P4NsHH-tRye-pJDc-MOvz-Xgd7-lzno-fubeeu' not found.

## 新增物理硬盘
这种情况比较简单，只需要在新的硬盘上创建一个物理卷，然后添加到卷组中即可。
```bash
# 1. 创建物理卷
sudo pvcreate /dev/sdb

# 2. 添加物理卷到卷组
sudo vgextend your_vg /dev/sdb

# 3. 扩展逻辑卷
sudo lvextend -l +100%FREE /dev/your_vg/your_lv

# 4. 重新调整ext4文件系统大小
sudo resize2fs /dev/your_vg/your_lv

# 5. 更新initramfs文件
sudo update-initramfs -c -k all

# 6. 更新grub文件
sudo grub-mkconfig -o /boot/grub/grub.cfg
```
