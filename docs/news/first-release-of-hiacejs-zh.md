# 又一力作：合璧操作系统关键组件 hiACEJS 已于近日发布

2020 年 12 月 11 日，飞漫软件发布了合璧操作系统（HybridOS）的另一个组件 hiACEJS。

这是飞漫开发团队自发布合璧操作系统开源协作计划后，继 MiniGUI 5.0、HybridOS 图形栈、hiWebKit 之后的又一力作。

hiACEJS 是 OpenHarmony 中 ACELite 的合璧操作系统派生版本，作为运行在合璧操作系统上的一个轻量级应用框架提供给开发者选择。

使用 hiACEJS，可直接使用华为的 DevEcho Studio 开发 ACELite 的应用，编译打包后即可通过 hiACEJS 直接运行在合璧操作系统上：

（插入 hybridos-hiacejs.mp4 视频。视频下载地址：<https://hybridos.fmsoft.cn/downloads/hybridos-hiacejs-201213.zip>）

但合璧的 hiACEJS 实现，比原始的 ACELite 有如下更多的技术特性：

- 在基于 Linux 内核的合璧操作系统上，借助 MiniGUI 5.0 的多进程模式，可同时运行多个 ACELite 应用实例。
- 利用合璧系统中的 hiBus 总线系统，可轻松利用合璧系统中其他组件的功能，而不需要开发 JavaScript 本地绑定功能。
- 利用合璧系统中的 hiShell，可实现应用切换的动画过渡效果。

hiiACEJS 的公开源代码仓库已就绪，可通过如下链接访问（**请随手点亮 GitHub 上 hiACEJS 项目的星星**）：

<https://github.com/FMSoftCN/hiACEJS>

目前，飞漫开发团队正在进一步完善 hiACEJS 和 hiBus 等合璧操作系统的关键组件。我们将于本月底如期发布合璧操作系统设备端代码的第一个正式版本。

点击文末原文链接，为合璧操作系统项目以及其他飞漫主持的开源项目呐喊助威吧！

---

原文链接：<https://store.fmsoft.cn/campaign/denoteoss-lt>

