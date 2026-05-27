# windows-terminal
之前一直想要找一个真跨平台的虚拟终端软件，在[终端宝库](https://terminaltrove.com)里面找了一大圈，
但是，符合我有`OSC 52，连字，图片预览，GPU加速，跨平台`的终端总共就三个，还都有毛病：
1. wezterm: 八百年没有更新正式版本，和linux两边配置都不一定兼容了
2. rioterm: 现阶段感觉还是半成品，配置偶尔变动，而且windows下现在字体、emoji和透明都有问题
3. contour: 在windows上字体和emoji完全没法用，查看issue，感觉不到希望

想了想，也不是非得要跨平台，摆烂了，干脆linux和windows各自用自己的吧，而且我还挺想试试ghostty的，
所以，windows就极端一点吧，直接用微软家的`windows terminal`好了。

好消息是这东西，它的配置文件是可复用的，只需要备份配置文件就行了。对于我这种使用`winget`安装的，其路径大概是：
`AppData/Local/Packages/Microsoft.WindowsTerminal_8wekyb3d8bbwe/LocalState/settings.json`
```json
{
    "$help": "https://aka.ms/terminal-documentation",
    "$schema": "https://aka.ms/terminal-profiles-schema",
    "actions": [],
    "centerOnLaunch": true,
    "compatibility.enableUnfocusedAcrylic": false,
    "copyFormatting": "none",
    "copyOnSelect": false,
    "defaultProfile": "{e0c1cc5f-d6d7-4bde-9963-02261bf40366}",
    "disableAnimations": true,
    "disabledProfileSources": 
    [
        "Windows.Terminal.Azure",
        "Windows.Terminal.VisualStudio",
        "Windows.Terminal.PowershellCore",
        "Windows.Terminal.Wsl"
    ],
    "experimental.scrollToChangeOpacity": false,
    "experimental.scrollToZoom": false,
    "keybindings": [],
    "language": "zh-Hant",
    "newTabMenu": 
    [
        {
            "type": "remainingProfiles"
        }
    ],
    "newTabPosition": "afterCurrentTab",
    "profiles": 
    {
        "defaults": 
        {
            "bellStyle": "none",
            "closeOnExit": "always",
            "colorScheme": "Tango Dark",
            "cursorShape": "filledBox",
            "font": 
            {
                "face": "JetBrainsMono Nerd Font Mono",
                "size": 13
            },
            "opacity": 70,
            "rightClickContextMenu": true,
            "scrollbarState": "hidden"
        },
        "list": 
        [
            {
                "commandline": "%SystemRoot%\\System32\\WindowsPowerShell\\v1.0\\powershell.exe",
                "guid": "{61c54bbd-c2c6-5271-96e7-009a87ff44bf}",
                "hidden": false,
                "name": "Windows PowerShell"
            },
            {
                "environment": 
                {
                    "COLORTERM": "truecolor"
                },
                "guid": "{3818a6b5-1a17-5573-bac8-4e3b3fc05130}",
                "hidden": false,
                "name": "Debian",
                "source": "Microsoft.WSL"
            },
            {
                "commandline": "D:\\msys64\\msys2_shell.cmd -defterm -here -no-start -ucrt64",
                "environment": 
                {
                    "COLORTERM": "truecolor",
                    "MSYS2_PATH_TYPE": "inherit",
                    "MSYS": "winsymlinks:nativestrict"
                },
                "guid": "{e0c1cc5f-d6d7-4bde-9963-02261bf40366}",
                "icon": "D:\\msys64\\ucrt64.ico",
                "name": "msys2",
                "startingDirectory": "~"
            }
        ]
    },
    "schemes": [],
    "searchWebDefaultQueryUrl": "https://www.ecosia.org/search?addon=opensearch&q=%s",
    "showTabsInTitlebar": true,
    "tabSwitcherMode": "disabled",
    "tabWidthMode": "equal",
    "theme": "system",
    "themes": [],
    "windowingBehavior": "useAnyExisting",
    "wordDelimiters": " /\\()\"'.,:;<>~!@#$%^&*|+=[]{}~?\u2502"
}
```
> 其实这里有一个坑，msys2里面进入wsl环境，会继承那一堆MSYS的环境变量，不过，就这样吧，摆烂。
