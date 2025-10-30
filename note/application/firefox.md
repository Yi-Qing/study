# 网站的特殊处理
1. bilibili: 在登录的时候需要关闭“增强型跟踪保护”
2. jianshu: 需要修改UA才能显示图片，目前确定的是不能有firefox和linux，推荐使用windows + chrome
3. reddit: 登录的时候不能使能隐私獾

```
当你在 Firefox 上遇到问题时，首先要做的是在禁用附加组件的隐私模式下加载它。如果它能用，那就是某个附加组件导致的问题。
如果不行，就用同样的方法，这次关掉跟踪保护。如果还是不行，断开VPN，再试一下前两个步骤。
排除这三种可能性，然后才能得出第四个结论：这个网站和Firefox不兼容。 
```

## UA
配置自定义模式：
```json
{
  "www.jianshu.com, note.youdao.com": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36 Trailer/93.3.3570.29",
  "xiaoeknow.com": "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36 NetType/WIFI MicroMessenger/7.0.20.1781(0x6700143B) WindowsWechat(0x6305002e)",
  "wx.qq.com": "Mozilla/5.0 (X11; Linux x86_64; rv:140.0) Gecko/20100101 Firefox/140.0"
}
```
