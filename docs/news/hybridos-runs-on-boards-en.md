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

On above two development boards, we run the device side of HybridOS (R2011), and the major software components are:

1. MiniGUI 5.0.3, Multi-process mode, composite schema:
  - The docker bar, status bar and background animation in the video are implemented based on the compositing schema.
  - To manage the apps, we developed a customize compositor for MiniGUI.
  - On the PX30 development board, MiniGUI uses DRM graphics engine, while on R818, MiniGUI uses FB graphics engine.
  - Software-only rendering is used, and GPU is not enabled yet.
2. HiShell: Shell for launching, switching, and managing apps.
3. HiWebKit: The WebKit extension version used to support HTML5 and the HybridOS extension features:
  - Among them, the page used to display the watch and stopwatch is implemented by hiWebKit's `hiview` tag.
  - The page for displaying the dashboard uses `himeter` and `hihand` tags of hiWebKit.

If you have the two development boards at hand, you can read the following words to download the compiled image files of the device side of HybridOS (R2011) and run it on the development board for the tremendous experience.

## Allwinner R818 AXP305 Board

You need to build the base system using GCC 7.5 or more tool chains, or just use the base system (47MB) we built. Below is the download link:

<https://hybridos.fmsoft.cn/downloads/r2011/tina-r818-evb2-gcc7-5.img.gz>

[Note] After downloading, please use the `gunzip` command to decompress the file first; The uncompressed size is 55MB.

First configure the network on the development board as follows:

1) Connect the device to the PC using a USB cable
2) Open the console of the device from PC by using the `adb` command:

```shell
$ adb shell
root@TinaLinux:/#
```

3) Scan the WiFi hotspots by using `wifi_scan_results_test`:

```shell
root@TinaLinux:/# wifi_scan_results_test
```

4) Run `wifi_connect_ap_test` to connect to a specific WiFi hotspot:

```shell
root@TinaLinux:/# wifi_connect_ap_test <your ssid> <your password>
```

Then, download the HybridOS device side R2011 image file (69MB) for the development board from the official website of HybridOS to your PC:

<https://hybridos.fmsoft.cn/downloads/r2011/r818.tar.gz>

After this, follow the steps to install HybridOS:

1) Connect the board to your PC via USB.
2) Run `adb` on your PC and push the `r818.tar.gz` file to `/mnt/UDISK` directory on the board:

``` shell
$ adb push r818.tar.gz /mnt/UDISK/
```

3) Run `adb` on your PC to open the console of the board:

```shell
$ adb shell
root@TinaLinux:/#
```

４) Run the following commands on the board to install and run HybridOS:

```shell
root@TinaLinux:/# cd /mnt/UDISK
root@TinaLinux:/mnt/UDISK# tar zxvf r818.tar.gz
root@TinaLinux:/mnt/UDISK# cd sysroot
root@TinaLinux:/mnt/UDISK/sysroot# . ./setup.sh
root@TinaLinux:/mnt/UDISK/sysroot# . ./run.sh
```

## Rockchip PX30 mini EVB

To compile HybridOS, you need to use GCC 7.0 or above, but the cross-compilation tool chain provided by the PX30 SDK is the version `gcc-linara-6.3.1`. You can recompile the SDK by using `gcc-linara-7.0` or above. The download address of the cross-compilation tool chain is:

<https://hybridos.fmsoft.cn/downloads/r2011/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz>

You can also directly download the SDK we compiled in advance by using `gcc-linara-7.5` to save your time. The download address of this image file is:

<https://hybridos.fmsoft.cn/downloads/r2011/rockdev.tar.gz>

After downloading, please unzip and overwrite the `rockdev/` directory in the original SDK, and then burn it to the development board by using SDK tools:

```
# ./rkflash.sh
```

Then, download the HybridOS device side R2011 image file (69MB) for the board from the official website of the HybridOS to your PC:

<https://hybridos.fmsoft.cn/downloads/r2011/px30.tar.gz>

Then install HybridOS in the following steps:

1) Start a NFS server on your PC. We assume the exported name of your NFS is `/share`.

2) Run the following command on your PC:

```shell
# cd /share
# tar zxf r818.tar.gz
```

3) Refer to the Rockchip's SDK document, connect PC to the board via UART, open the UART console, and edit the file `/etc/wpa_supplicant.conf` on the board:

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

4) Please fill in the `ssid`, `psk`, and other contents in the file correctly according to the actual situation, and then save and exit. Enter the following command on the console to start WiFi and test the network:

```shell
# wifi_start.sh
# ping hybridos.fmsoft.cn
```

5) Run the following commands on the UART console:

```shell
# mount -t nfs -o nolock <the IP of your PC>:/share /mnt
# cd /userdata
# cp /mnt/sysroot . -a
# cd sysroot
# ./setup.sh
# reboot
```

6) When the PX30 development board is restarted, you can see the HybridOS interface and operate it through the touch screen.

## Next Plan

In the end of 2020, the HybridOS team will complete the development of the remaining components of HybridOS Device Side, including:

1. hiBus, the local data bus server. Through this server, we can obtain system status change information (such as battery level, WiFi signal strength, etc.) in your hiApps, and can also complete certain system functions by sending task requests, such as connecting to a designated WiFi hotspot.
1. hiACEJS, a lightweight JavaScript app framework. HiACEJS is a derivative of the lightweight JavaScript app framework in OpenHarmoney 2.0 and provides lightweight Web-like app support for some scenarios. In hiACEJS, we will provide support for hiBus to provide a more flexible development framework. In the future, we will also integrate the breakthrough technologies we implemented in hiWebKit into the framework,
1. Two demonstrations which use hiWebKit and hiACEJS respectively; one for smart speakers and one for household appliances.
1. A complete device-side software stack, component system (including cross-compilation), documentation and sample codes of the integrated operating system.

Remember, HybridOS follows the "open source and open process" principle, **release means you can see the source code at the same time**. In fact, the source code of HybridOS is mirrored on GitHub in real time.

**To support HybridOS, please visit the HybridOS repository on GitHub, and light up the star of the HybridOS project on GitHub!**

<https://github.com/FMSoftCN/hybridos>

