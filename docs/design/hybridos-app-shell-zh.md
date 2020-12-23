# 合璧操作系统设备端界面外壳的设计

【主题】合璧操作系统设备端界面外壳的设计  
【摘要】本文阐述合璧操作系统设备端界面外壳的设计  
【版本】1.0   
【作者】魏永明  
【日期】2020 年 9 月  
【状态】定稿  

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

**目录**

- [单一应用场景](#单一应用场景)
   + [第一阶段](#第一阶段)
   + [第二阶段](#第二阶段)
   + [第三阶段](#第三阶段)
   + [还需要完善的功能](#还需要完善的功能)
- [复杂应用场景](#复杂应用场景)
- [附：商标声明](#附商标声明)


## 单一应用场景

工业控制面板、家用电器的智能控制面板等属于本文所述的单一应用场景。这种应用场景通常具有如下几个特点：

1. 具有较大的屏幕尺寸，通常为横屏。
1. 无图形硬件加速，不需要多媒体支持。
1. 无实体按键，用户使用触摸屏操作，常见操作主要是点击和单指拖拽。
1. 除了输入 WiFi 密码等情形外，极少需要输入法来输入一般文字等。
1. 基本的多任务切换能力。
1. 无需支持第三方应用的下载和安装。
1. 需要提供固件升级功能。

在合璧操作系统中，使用 MiniGUI 5.0（多进程模式）作为窗口系统，一些系统界面仍使用 C/C++ 开发，而所有的应用使用 hiWebKit 来渲染其界面。在 GUI 层面，一共有四个进程：

1. MiniGUI 的 `mginit`。该进程扮演窗口管理器运行，使用 C/C++ 开发，包括如下模块：
   1. 定制的合成器（compositor），用于提供主窗口启动、关闭和主窗口展示及前后台切换的特效。
   1. （可选）输入法。
1. MiniGUI 的动态背景进程。该进程动态展示屏幕背景，可展示一个简单的 HybridOS 动画效果。
1. 系统管理进程，拥有较高的系统管理权限，该进程负责创建应用子进程，并创建和维护如下主窗口（称为系统主窗口，system main window）：
   1. 一个用作 Docker 的主窗口，用于呼出主功能（或启动器）、系统设置，使用不同的形式展示应用主窗口等。
   1. 一个用作状态栏（Status Bar）的主窗口，用于展示当前活动主窗口的标题（及图标）、时间以及系统电量等状态。
   1. 锁屏（HybridOS Screen Lock），可在 hiWebKit 支持下运行。
   1. 启动器（HybridOS Launcher），可在 hiWebKit 支持下运行。
   1. 通知（HybridOS Notification），可在 hiWebKit 支持下运行。
1. 应用进程，在 hiWebKit 的支持下运行，使用扩展的 Web 前端技术开发：
   1. 应用进程创建的主窗口称为应用主窗口（app main window）；所有应用主窗口由单个 hiWebKit 进程实例管理，属于同一进程地址空间。
   1. 同一时刻只有一个前台应用主窗口展示在屏幕上（全屏），其他后台的应用主窗口对应的页面，进入节约内存状态。
   1. 后台应用主窗口被唤醒后，恢复页面状态，原来的前台主窗口称为后台应用主窗口，进入节约内存状态。

下图给出了单一应用场景下的系统架构图：

```
 ---------------------------------------------------------------------------------
| DockerBar, StatusBar,   |                   |           | Input Method Window,  |
| Screen Lock, Launcher,  | App Main Windows  |  N/A      | System Alert Window,  |        - main windows
| Notification...         |                   |           | ...                   |
 ---------------------------------------------------------------------------------
|      System Manager     |     App Agent     | Wallpaper | mginit w/ compositor  |____    - processes
 ---------------------------------------------------------------------------------     |
|                   hiWebKit                  |                                   |    |
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

下图给出了正常运行情况下的界面效果：

![正常运行情况下的界面效果](hybridos-gui-shell.png)

上图中，交互要求如下：

- 状态栏在创建新窗口或者设置新的前台主窗口时展开，展示两秒后向上收起。
- 用户点击 Docker Bar 后，Docker 主窗口会向右展开，“向左”图标动态改变方向变成“向左”按钮。此时用户可点击其上的图标来使用其他功能；状态栏会同步向下展开。
- 除向左的收起按钮之外，展开的 Docker Bar 上包含四个功能按钮：主页面（Home）、切换主窗口、系统设置、关机。
- 用户长时间不操作 Docker Bar 时，将自动收起；此时，状态栏同步收起。

进入切换应用主窗口模式时，界面效果类似 iOS：

![切换应用主窗口模式下的界面效果](hybridos-switch-main-windows.jpg)

简单起见，我们做如下处理：

1. 仅展示应用主窗口。
1. 不展示应用图标，取而代之为关闭按钮，点击可关闭对应的主窗口（强制关闭）。

切换应用主窗口时的，交互要求如下：

- 状态栏收起。
- Docker Bar 收起，仅展示“返回”按钮。
- 使用 Cover Flow 效果展示所有应用主窗口内容，用户可左右滑动。
- 点击某个主窗口，将激活该主窗口，系统动画恢复到正常运行情况。
- 点击正面主窗口顶部的“关闭”按钮会强制关闭该主窗口。
- 向上滑动某主窗口，可强制关闭该主窗口（可选）。

### 第一阶段

以上设计可分阶段完成。第一阶段主要包括：

1. mginit 进程
   - 支持应用主窗口切换动画效果的合成器
1. MiniGUI 合成器图式下的动态背景渲染进程
1. 系统管理进程：
   - 简单 StatusBar，仅显示当前最顶应用主窗口的标题以及系统时间的
   - 简单 DockerBar
1. 应用进程：
   - 功能入口页面
   - 若干 hiWebKit 特色功能展示页面
   - 一般性网站。

### 第二阶段

1. mginit 进程
   - 支持应用主窗口切换动画效果的合成器
1. MiniGUI 合成器图式下的动态背景渲染进程
1. 系统管理器进程：
   - 增强的 StatusBar，显示当前活动主窗口的标题、图标以及系统时间、信号强度、电池电量等信息
   - 简单的 DockerBar
   - 一个基于 hiWebKit 的锁屏页面
1. hiBus 服务器进程，用于处理来自应用进程的功能请求。
1. WebSocket 服务器进程，用于 hiBus 请求和响应的代理。
1. 应用进程：
   - 功能入口页面。
   - 若干 hiWebKit 特色功能展示页面
   - 使用 WebSocket 实现控制功能
   - 系统设置页面，可展示滚轮等 hiWebKit 中的控件用法，并使用 WebSocket 实现系统设置功能
   - 访问一般性网站

### 第三阶段

1. mginit 进程
   - 输入法
   - 支持应用主窗口切换动画效果的合成器
1. MiniGUI 合成器图式下的动态背景渲染进程
1. 系统管理器进程：
   - 增强的 StatusBar，显示当前活动主窗口的标题、图标以及系统时间、信号强度、电池电量等信息
   - 简单的 DockerBar
   - 一个基于 hiWebKit 的锁屏页面
1. hiBus 服务器进程，用于处理来自应用进程的系统请求。
1. WebSocket 服务器进程，用于 hiBus 请求和响应的代理。
1. 应用进程：
   - 功能入口页面。
   - 若干 hiWebKit 特色功能展示页面
   - 使用 WebSocket/JavaScript 实现控制功能
   - 使用 hiBus/HVML 标签实现控制功能
   - 访问一般性网站
1. HybridOS 构建脚本提供打开/关闭 JavaScript 支持、硬件加速、WebGL、多媒体支持等的选项。

### 还需要完善的功能

根据以上描述，我们还需要完成如下功能特性：

1. 通过 hiBus 和系统守护进程交换数据，并动态更新页面内容。
1. 一个简单的 WebSocket 服务器进程，用于转发 hiBus 命令及响应。
1. hiWebKit 可关闭 JavaScript。
1. 应用主窗口进入节约内存状态以及恢复正常状态。
1. 基本的进程特权管理，用于系统管理器。

## 复杂应用场景

（待续）

## 附：商标声明

_注意：除本说明之外，原样复制，放到文档最后。_

本文提到的产品、技术或者术语名称，涉及北京飞漫软件技术有限公司在中国或其他地区注册的如下商标：

1) 飛漫

![飛漫](https://www.fmsoft.cn/application/files/cache/thumbnails/87f47bb9aeef9d6ecd8e2ffa2f0e2cb6.jpg)

2) FMSoft

![FMSoft](https://www.fmsoft.cn/application/files/cache/thumbnails/44a50f4b2a07e2aef4140a23d33f164e.jpg)

3) 合璧

![合璧](https://www.fmsoft.cn/application/files/cache/thumbnails/9c57dee9df8a6d93de1c6f3abe784229.jpg)
![合壁](https://www.fmsoft.cn/application/files/cache/thumbnails/f59f58830eccd57e931f3cb61c4330ed.jpg)

4) HybridOS

![HybridOS](https://www.fmsoft.cn/application/files/cache/thumbnails/5a85507f3d48cbfd0fad645b4a6622ad.jpg)

5) HybridRun

![HybridRun](https://www.fmsoft.cn/application/files/cache/thumbnails/84934542340ed662ef99963a14cf31c0.jpg)

6) MiniGUI

![MiniGUI](https://www.fmsoft.cn/application/files/cache/thumbnails/54e87b0c49d659be3380e207922fff63.jpg)

6) xGUI

![xGUI](https://www.fmsoft.cn/application/files/cache/thumbnails/7fbcb150d7d0747e702fd2d63f20017e.jpg)

7) miniStudio

![miniStudio](https://www.fmsoft.cn/application/files/cache/thumbnails/82c3be63f19c587c489deb928111bfe2.jpg)

