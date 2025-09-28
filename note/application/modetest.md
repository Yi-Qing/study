# DRM
在`Linux`下，DRM(Direct Rendering Manager)是一个处理图形硬件和显示设备交互的框架。
它提供了一套`API`来直接管理显示设备和`GPU`，支持图形渲染和显示输出。
常用于直接访问硬件，通常与`X11`或`Wayland`等图形窗口系统配合使用。

# 层级
下图中蓝色是物理硬件的抽象，黄色是软件的抽象。
![DRM的Objects](http://inews.gtimg.com/newsapp_match/0/15583477584/0)

![Objects描述](http://inews.gtimg.com/newsapp_match/0/15583477587/0)

![Objects关系](http://inews.gtimg.com/newsapp_match/0/15583477591/0)

# 查询命令
```bash
modetest -M sunxi-drm
```
> 重点关注下面几个内容：
```log
Encoders:
id      crtc    type    possible crtcs  possible clones
130     0       Virtual 0x00000003      0x00000001
132     0       TMDS    0x00000003      0x00000002
139     115     TMDS    0x00000003      0x00000004
```

```log
Connectors:
id      encoder status          name            size (mm)       modes   encoders
133     0       disconnected    HDMI-A-1        0x0             0       132
140     139     connected       eDP-1           530x300         31      139
modes:
index name refresh (Hz) hdisp hss hse htot vdisp vss vse vtot
#0 1920x1080 60.00 1920 2008 2052 2200 1080 1084 1089 1125 148500 flags: phsync, pvsync; type: preferred, driver
#1 1920x1080 59.94 1920 2008 2052 2200 1080 1084 1089 1125 148352 flags: phsync, pvsync; type: driver
```

```log
CRTCs:
id      fb      pos     size
94      0       (0,0)   (0x0)
115     0       (0,0)   (1920x1080)
#0 1920x1080 60.00 1920 2008 2052 2200 1080 1084 1089 1125 148500 flags: phsync, pvsync; type: preferred, driver
```

```log
Planes:
id      crtc    fb      CRTC x,y        x,y     gamma size      possible crtcs
89      0       0       0,0             0,0     0               0x00000001
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15 AR30 AB30 RA30 BA30 NV61 NV16 YU16 YV16 NV12 NV21 YU12 YV12 YU11 YV11 P010 P210
95      0       0       0,0             0,0     0               0x00000001
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15 AR30 AB30 RA30 BA30 NV61 NV16 YU16 YV16 NV12 NV21 YU12 YV12 YU11 YV11 P010 P210
100     0       0       0,0             0,0     0               0x00000001
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15
105     0       0       0,0             0,0     0               0x00000001
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15
110     0       0       0,0             0,0     0               0x00000002
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15 AR30 AB30 RA30 BA30 NV61 NV16 YU16 YV16 NV12 NV21 YU12 YV12 YU11 YV11 P010 P210
116     0       0       0,0             0,0     0               0x00000002
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15
121     0       0       0,0             0,0     0               0x00000002
formats: RG24 BG24 AR24 AB24 RA24 BA24 XR24 XB24 RX24 BX24 RG16 BG16 AR12 AB12 RA12 BA12 AR15 AB15 RA15 BA15
Frame buffers:
id      size    pitch
```

# 测试命令
```bash
modetest -M sunxi-drm -s 140@115:1920x1080 -P 89@115:1920x1080 -Ftiles
```
- M: 指定使用的DRM驱动，这里使用`sunxi-drm`，对应`Allwinner`芯片的`DRM`驱动。
- a: 原子操作，`Allwinner`芯片的`DRM`驱动没有支持，去掉
- s: 显示模式，这里指定为`<Connector_id>@<Crtc_id>:<mode>`。
- P: 输出模式，这里指定为`<Panel_id>@<Crtc_id>:<mode>`。
- F: 格式，这里指定为`tiles`斜彩条带格式。

# 参考
- [DRM入门](https://cloud.tencent.com/developer/news/977897)
