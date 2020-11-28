# Actual Shooting: HybridOS Runs on Two Domestic SoC Development Boards

After a struggle of nearly two months, the development team of HybridOS completed the porting of HybridOS device side (R2011) to two SoC processors.

These two development boards are respectively provided by FMSoft's two chip partners, Allwinner and Rockchip:

1. Allwinner R818 AXP305 development board, main hardware configuration:
   - Quad-core ARM Cortex-A53 CPU 1.6GHz with GPU.
   - DDR3 1GB
   - 16GB eMMC
   - Screen resolution: 800x1280 (landscape)
1. Rockchip PX30 mini EVB development board, main hardware configuration:
   - Quad-core ARM Cortex-A35 CPU 1.3GHz with GPU.
   - DDR3 2GB
   - 16GB eMMC
   - Screen resolution: 1280x800 (portrait)

The real shooting video for HybridOS on Allwinner R818:

(Insert hybridos-r818.mp4 video)

The real shooting video for HybridOS on Rockchip PX30:

(Insert the hybridos-px30.mp4 video)

HybridOS 设备侧 R2011 版本在以上两块开发板上，主要使用的软件组件为：

1. MiniGUI 5.0.3，多进程模式，合成图式：
   - 视频中应用的启动条、状态栏、背景动画等，基于合成图式实现。
   - 使用中应用的缩小展示、切换、管理等，使用了 MiniGUI 在合成图式下的定制合成器技术开发。
   - 在瑞芯 PX30 开发板上，使用 DRM 图形引擎；在全志 R818 开发板上使用 FB 图形引擎。
   - 采用的是纯软件渲染效果，暂未启用 GPU。在使用 DRM 图形引擎时，动画流畅顺滑，但在使用 FB 图形引擎时，有丢帧现象。
1. hiShell：用于启动应用、切换应用、管理应用的外壳程序。
1. hiWebKit：用于支撑 HTML5 及 HybridOS 扩展特性的 WebKit 扩展版本：
   - 其中用于展示手表表盘、秒表的页面，采用 hiWebKit 的 `hiview` 扩展标签实现。
   - 其中用于展示仪表盘的页面，使用了 hiWebKit 的 `himeter` 和 `hihand` 扩展标签实现。
   - 仔细观察表盘及仪表盘中的指针及条带变化，在两款开发板上的运行效果都非常棒。

如果读者手头有这两块开发板，可以阅读如下内容下载 HybridOS 设备侧 R2011 版本的已编译映像文件，放到开发板上运行做实际的体验。

## 全志 R818 AXP305 开发板

需要使用 GCC 7.5 或以上工具链编译基础系统，或者直接使用我们编译好的基础系统（47MB）重新刷机。下载链接如下:

<https://hybridos.fmsoft.cn/downloads/r2011/tina-r818-evb2-gcc7-5.img.gz>

注：下载后请使用 `gunzip` 命令解压使用；解压后尺寸为 55MB。

首先按如下步骤配置开发板上的网络：

1) 将设备用 USB 线连接到 PC 上
2) 在 PC 的终端使用 adb 命令打开设备的控制台

```shell
$ adb shell
root@TinaLinux:/#
```

3) 在控制台使用 `wifi_scan_results_test` 命令扫描附近的 WiFi 热点：

```shell
root@TinaLinux:/# wifi_scan_results_test
```

4) 在设备控制台使用 `wifi_connect_ap_test` 命令连接到指定热点：

```shell
root@TinaLinux:/# wifi_connect_ap_test <your ssid> <your password>
```

然后从 HybridOS 官网下载 HybridOS 设备侧 R2011 版本针对该开发板的映像文件（69MB）到开发用 PC：

<https://hybridos.fmsoft.cn/downloads/r2011/r818.tar.gz>

之后按如下步骤安装 HybridOS：

1) 将设备用 USB 连接到 PC 上。
2) 在 PC 终端使用 `adb` 命令将下载的 `r818.tar.gz` 文件推送到设备上 `/mnt/UDISK` 目录下：

``` shell
$ adb push r818.tar.gz /mnt/UDISK/
```

3) 在 PC 终端使用 adb 命令打开设备的控制台：

```shell
$ adb shell
root@TinaLinux:/#
```

４) 在设备控制台执行以下命令进行安装和运行：

```shell
root@TinaLinux:/# cd /mnt/UDISK
root@TinaLinux:/mnt/UDISK# tar zxvf r818.tar.gz  # 解压后会看到一个sysroot目录
root@TinaLinux:/mnt/UDISK# cd sysroot
root@TinaLinux:/mnt/UDISK/sysroot# . ./setup.sh  # 安装
root@TinaLinux:/mnt/UDISK/sysroot# . ./run.sh  #  运行
```

## 瑞芯 PX30 mini EVB 开发板

编译 HybridOS，需要使用 GCC 7.0 或以上版本，但 PX30 SDK 所提供的交叉编译工具链，版本为 `gcc-linaro-6.3.1`。您可以使用 `gcc-linaro-7.0` 或以上版本重新编译 SDK。交叉编译工具链下载地址为：

<https://hybridos.fmsoft.cn/downloads/r2011/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz>

您也可以直接下载我们使用 `gcc-linaro-7.5` 提前编译好 SDK，以节约您的宝贵时间。其映像文件的下载地址为：

<https://hybridos.fmsoft.cn/downloads/r2011/rockdev.tar.gz>

下载后请解压缩，并覆盖原 SDK 中的 `rockdev` 目录，然后使用 SDK 工具烧写到开发板上：

```
# ./rkflash.sh
```

然后从 HybridOS 官网下载 HybridOS 设备侧 R2011 版本针对该开发板的映像文件（69MB）到开发用 PC：

<https://hybridos.fmsoft.cn/downloads/r2011/px30.tar.gz>

之后按如下步骤安装 HybridOS：

1) 请参考 <https://blog.csdn.net/gys_20153235/article/details/80516560> 在 PC 机上配置 NFS 服务。以下步骤假设 NFS 的挂载目录为 `/share`。

2) 启动 NFS 服务后执行下面的命令：

```shell
# cd /share
# tar zxf r818.tar.gz
```

3) 参考瑞芯提供的 SDK 文档 `《PX30 MINI EVB硬件操作指南_20180710.pdf》`，将 PC 机通过串口与开发板连接，然后进入串口控制台，并在 PC 机的串口控制台上编辑 `/etc/wpa_supplicant.conf` 文件：

```
ctrl_interface=/var/run/wpa_supplicant
ap_scan=1
update_config=1

network={
        ssid="<YOUR SSID>"
        psk="<YOUR PASSWORD>"
        key_mgmt=WPA-PSK
}
```

4) 请根据实际情况正确填写文件中的 SSID 和 PSK 等内容，然后保存退出。在串口控制台上输入命令，以启动 WiFi 并测试网络：

```shell
# wifi_start.sh
# ping hybridos.fmsoft.cn
```

5) 在串口控制台上输入命令：

```
# mount -t nfs -o nolock PC_ip:/share /mnt
# cd /userdata
# cp /mnt/sysroot . -a
# cd sysroot
# ./setup.sh
# reboot
```

6) 当 PX30 开发板重新启动后，就可以看到 HybridOS 的界面，并可以通过触摸屏进行操作。

## Next Plan

In the end of 2020, the HybridOS team will complete the development of the remaining components of HybridOS Device Side, including:

1. hiBus, the local data bus server. Through this server, we can obtain system status change information (such as battery level, WiFi signal strength, etc.) in your hiApps, and can also complete certain system functions by sending task requests, such as connecting to a designated WiFi hotspot.
1. hiACEJS, a lightweight JavaScript app framework. HiACEJS is a derivative of the lightweight JavaScript app framework in OpenHarmoney 2.0 and provides lightweight Web-like app support for some scenarios. In hiACEJS, we will provide support for hiBus to provide a more flexible development framework. In the future, we will also integrate the breakthrough technologies we implemented in hiWebKit into the framework,
1. Two demonstrations which use hiWebKit and hiACEJS respectively; one for smart speakers and one for household appliances.
1. A complete device-side software stack, component system (including cross-compilation), documentation and sample codes of the integrated operating system.

Remember, HybridOS follows the "open source and open process" principle, **release means you can see the source code at the same time**. In fact, the source code of HybridOS is mirrored on GitHub in real time.

**To support HybridOS, please visit the HybridOS repository on GitHub, and light up the star of the HybridOS project on GitHub!**

<https://github.com/FMSoftCN/hybridos>

