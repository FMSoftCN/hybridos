# 合璧操作系统设备端hiAceJs设计


本文阐述合璧操作系统设备端hiAceJs设计。

- [基本框架及术语](#基本框架及术语)
- [架构及关键模块](#架构及关键模块)


## 基本框架及术语

下图给出了 HybridOS 设备侧的系统架构图：

```
 ---------------------------------------------------------------------------------
| DockerBar, StatusBar,   |                   |           | Input Method Window,  |
| Screen Lock, Launcher,  | App Main Windows  |  N/A      | System Alert Window,  |        - main windows
| Notification...         |                   |           | ...                   |
 ---------------------------------------------------------------------------------
|      System Manager     |     App Agent     | Wallpaper | mginit w/ compositor  |____    - processes
 ---------------------------------------------------------------------------------     |
|       hiWebKit          |    hiAceJs        |                                   |    |
 ---------------------------------------------                                    |    |
|  MiniGUI, hiCairo, hiMesa, SQLite, FreeType, HarfBuzz, LibPNG, LibJPEG, ...     |    | hiBus
|                                                                                 |    |
 ---------------------------------------------------------------------------------     |
|                           HybridOS servers and user daemons                     |____|
 ---------------------------------------------------------------------------------
|                          Python runtime environment (optional)                  |
|                             LibZ, CURL, LibGcrypt, ...                          |
|                               C/C++ runtime environment                         |
 ---------------------------------------------------------------------------------
|                               Linux Kernel/Drivers                              |
 ---------------------------------------------------------------------------------
```

在 HybridOS 中，提供了两种应用开发框架。一种是基于hiWebKit，另一种是基于hiAceJs。

hiWebKit 基础代码来自于 WebKit，提供了完整理web应用支持。

hiAceJs 的一些思想和部分代码来自于 OpenHarmony 的 ace_lite_jsfwk，提供了一套跨平台的类web应用
开发框架，通过Toolkit将开发者编写的HML、CSS和JS 文件编译打包成JS Bundle，解析运行JS Bundle，
生成native UI View组件树并进行渲染显示。通过支持三方开发者使用声明式的API进行应用开发，以数
据驱动视图变化，避免大量的视图操作，大大降低应用开发难度，提升开发者开发体验。


## 架构及关键模块
