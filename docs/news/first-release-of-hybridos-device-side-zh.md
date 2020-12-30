# 合璧操作系统设备侧 1.0 正式发布！

2020 年 12 月 31 日，飞漫软件发布了合璧操作系统设备侧（HybridOS Device Side）1.0 版本（R2012-DS）。

## 实拍运行效果

在了解具体的技术细节之前，我们先看下面的实拍视频。

下面的视频给出了 HybridOS 运行在两款支持 Linux 的 SoC 开发板上的效果。这两款开发板，分别由飞漫的两个芯片合作伙伴全志及瑞芯提供：

1. 全志 R818 AXP305 开发板，主要硬件配置：
   - Quad-core ARM Cortex-A53 CPU 1.6GHz，含 GPU。
   - DDR3 1GB
   - 16GB eMMC
   - 屏幕分辨率：800x1280（横屏）
1. 瑞芯 PX30 开发板，主要硬件配置：
   - Quad-core ARM Cortex-A35 CPU 1.3GHz，含 GPU。
   - DDR3 2GB
   - 16GB eMMC
   - 屏幕分辨率：1280x800（竖屏）

（插入 hybridos-on-boards-r2012.mp4 视频。视频下载地址：<https://hybridos.fmsoft.cn/downloads/hybridos-on-boards-r2012.zip>）

若要尝试自行编译针对上述开发板的合璧操作系统，读者可阅读[《实拍：合璧操作系统运行在两款国产 SoC 开发板上》](https://www.fmsoft.cn/blog/soc)。

## 关键组件

在合璧操作系统 R2012-DS 中，包括如下针对嵌入式或智能物联网设备的关键组件。

### HybridOS 图形栈

HybridOS 图形栈基于成熟的开源基础软件构成，提供了软件或硬件加速的 2D/3D 实现，其中包括如下软件包：

1. hiCairo：这是 Cairo 的 HybridOS 派生版本，在 Cairo 最新版本之上增加了对 MiniGUI 平台的支持。
1. hiDRM：libDRM 的 HybridOS 派生版本，增加了一个接口，用于 GPU 集成。
1. hiDRMDrivers：这个函数库包含 MiniGUI 基于 Linux DRM 硬件加速图形引擎的各芯片应用级驱动程序，目前提供了针对 Intel 集成显卡的支持，如 i915、i965 等。
1. hiMesa：这是 Mesa 的 HybridOS 派生版本，主要增加了 EGL 的 MiniGUI 平台实现。有了 EGL 的 MiniGUI 平台实现，可在 HybridOS 上直接整合 OpenGL、OpenGL ES 以及 OpenVG 等 2D/3D 图形应用。

有关详情，可阅读文章[《HybridOS 第一波开源组件发布》](https://www.fmsoft.cn/blog/hybridos-1)获得详细介绍。

### MiniGUI 5.0

在合璧操作系统设备侧，MiniGUI 主要扮演窗口系统的角色。MiniGUI 5.0 带来了三个全新的特性，以支持开发者开发具有更好视觉和交互效果的应用：

1. 合成图式。合成图式为窗口系统提供各种视觉效果和新奇交互效果带来了可能。在合成图式之前，大部分窗口系统使用共享缓冲区图式，通过管理和维护窗口的层叠关系以及相互之间的剪切来实现多窗口的管理。传统的共享缓冲区图式很难在多进程环境下实现不规则窗口、半透明或模糊叠加效果，而合成图式则可以轻松解决这些问题，而且还可以方便实现窗口切换时的动画效果。
1. 八种主窗口 Z 序级别。在 MiniGUI 5.0 中，我们增强了 MiniGUI 的窗口管理器以支持某些特殊的主窗口类型。这项增强功能使我们可以创建一些特殊的应用，该应用可以作为锁屏、通知栏、程序坞或者启动器使用。
1. 虚拟窗口。MiniGUI 5.0 增强了消息处理模块，以独立模式或者多进程模式运行的 MiniGUI 程序亦可跨线程传递消息，另外还引入了虚拟窗口的概念。虚拟窗口是一个特殊的窗口对象，没有可见的窗口区域。但是，在其他线程中创建虚拟窗口之后，可以使用 MiniGUI 的消息传递机制在可见的窗口线程和虚拟窗口所在的线程之间邮寄或发送消息。此增强功能为基于 MiniGUI 的应用开发提供了非常有用的基础设施，以开发设计良好的多线程应用程序。

除了以上三个主要的增强之外，MiniGUI 5.0 还调整了一些底层架构，重构了一些底层模块。有兴趣的读者可以阅读[《MiniGUI 的新里程碑：5.0.0 版发布了！》](https://www.fmsoft.cn/blog/minigui-5-0-0)获得详细介绍。

### hiBus

### hiShell

### hiWebKit

hiWebKit 是知名开源浏览器引擎 WebKit 的合璧操作系统派生版本，其中包括了合璧操作系统开发团队近一年来在现有 HTML/CSS 标准和规范基础上所做的诸多扩展：

1. 可扩展复杂控件的支持。这个特性相当于为浏览器提供了一个轻量级的插件机制。开发者可以利用这个特性，开发符合自己需要的复杂控件，避免使用对性能影响较为严重的方案，如传统插件。
1. 引入新的标签及 CSS 属性用于支持旋转滚轮控件。开发者可以利用这个特性，轻松编写用于选择时间、行政区划等的滚轮控件。
1. 引入新的标签及 CSS 属性用来支持旋转指针控件。开发者可以利用这个特性，轻松编写可以展现指针式进度的界面，如汽车仪表盘。
1. 元素间属性或内容的绑定机制。通过定义 HTML 元素之间的绑定关系，由用户代理自动完成被绑定元素之间的属性或者内容同步。开发者可以利用这个特性，不使用 JavaScript 即可建立两个元素之间的内容或者属性的响应式更新。
1. HTML 内容的本地动态生成技术。通过数据映射技术，使得用户代理对 DOM 树的动态调整无须脚本介入就可实现。开发者可以利用这个特性，直接在 JSON 数据基础上生成 HTML 文档内容，而无需借助 JavaScript 脚本或者后台脚本程序。
1. 用于直接访问 SQL 数据库的扩展图式（LSQL、RSQL 协议）。开发者可以利用这个特性，直接使用 SQL 语句来访问本地或者远程的 SQL 数据库，而无需开发后台脚本。
1. 用于执行本地系统命令的扩展图式（LCMD 协议）。开发者可以利用这个特性，直接在浏览器中调用本地命令行程序，并将其输出作为网页的输入就地生成页面内容，而无需后台脚本。

### hiACEJS

## 关联阅读

有关 hiWebKit 扩展技术的细节以及给开发者带来的好处，在专辑[《合璧操作系统的技术突破》](https://mp.weixin.qq.com/mp/appmsgalbum?action=getalbum&album_id=1442713925182767106&__biz=MzA5MTYwNTA3MA==#wechat_redirect)中有介绍，感兴趣的读者可以点击阅读。

### 代码仓库

要尝试 hiWebKit，请访问合璧操作系统在 GitHub 上的代码仓库（**麻烦随手点亮 HybridOS 项目仓库的星星哦**）：

<https://github.com/FMSoftCN/hybridos>

并按照如下 REAMD 文档中的介绍构建 HybridOS 设备端的代码：

<https://github.com/FMSoftCN/hybridos/tree/master/device-side>


---

原文链接：<https://store.fmsoft.cn/campaign/denoteoss-lt>

