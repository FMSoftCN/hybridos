# 在PX30平台上运行HybridOS

本文将介绍如何在瑞芯微PX30_mini_EVB_V11_20190507开发板上，安装HybridOS的步骤和过程。

耿岳

- [代码的获取](#代码的获取)
- [PX30 SDK的编译与烧写](#px30-sdk的编译与烧写)
   + [升级SDK](#升级sdk)
   + [升级交叉编译工具链](#升级交叉编译工具链)
   + [替换内部工具链](#替换内部工具链)
   + [修改代码](#修改代码)
   + [编译及烧写](#编译及烧写)
- [安装HybridOS二进制文件](#安装hybridos二进制文件)
   + [在PC机上配置NFS服务](#在pc机上配置NFS服务)
   + [配置开发板的WIFI](#配置开发板的wifi)
   + [安装HybridOS二进制代码](#安装hybridos二进制代码)


## 代码的获取

- 从瑞芯微网站上，获取PX30开发板SDK。具体下载地址及权限，请与瑞芯微技术支持部门联系。在本工程中，使用的SDK版本为：px30_linux_sdk_release_v1.3.0_20200224；
- 从北京飞漫软件技术有限公司，获取交叉编译工具链及HybridOS的二进制代码。下载地址为：
  - 交叉编译工具链：
  - HybridOS二进制代码：
  - gcc-7.5升级包：
  
  假设下载到：$HOME/Downloads。
  

## PX30 SDK的编译与烧写

请按照下列步骤，编译PX30 SDK并烧写进PX30开发板。

### 升级SDK

根据SDK中的文档：《Rockchip_PX30_Linux_SDK_Release_V1.3.0_20200224_CN.pdf》，获取并升级SDK源代码，假设源代码目录为：

```
# cd $HOME/px30/px30_linux_sdk_release_v1.3.0_20200224
```

### 升级交叉编译工具链

由于HybridOS的需要gcc-7以上版本，而SDK中只提供了gcc-6.3.1版本，因此需要更换交叉编译工具链

```
# cd prebuilts/gcc/linux-x86/aarch64/
# tar xvJf $HOME/Downloads/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
```

然后搜索以下文件：

```
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/buildroot/package/rockchip/rkwifibt/rkwifibt.mk
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/distro/envsetup.sh
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/external/camera_engine_rkisp/productConfigs.mk
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/external/security/rk_tee_user/build.sh
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/kernel/Makefile
$HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/u-boot/make.sh
```

将其中的

```
gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu
```

替换为

```
gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu
```

### 替换内部工具链

```
# cd $HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/buildroot/package
# rm -fr gcc
# tar zxf $HOME/Downloads/gcc.tar.gz
```

### 修改代码

因为更换了交叉编译工具链，所以会造成原有代码编译时错误。编译前修改文件：

```
# vim $HOME/px30/px30_linux_sdk_release_v1.3.0_20200224/external/recovery/update_engine/flash_image.cpp
```

将其中的：

```
    for (i =0; i < 256; i++) {
        if (strUUid == '\0') {
            break;
        }
```

修改为：

```
    for (i =0; i < 256; i++) {
        if (strUUid[0] == '\0') {
            break;
        }
```

### 编译及烧写

```
# cd $HOME/px30/px30_linux_sdk_release_v1.3.0_20200224
# ./build.sh
# sudo ./rkflash.sh
```

具体烧写的方法与步骤，清参考SDK文档：《Rockchip_PX30_Linux_SDK_Release_V1.3.0_20200224_CN.pdf》。

## 安装HybridOS二进制文件

### 在PC机上配置NFS服务

请参考 https://blog.csdn.net/gys_20153235/article/details/80516560 为PC机配置NFS服务，并假设NFS的挂载目录为：

```
/share
```

启动NFS服务后，执行下面的命令：

```
# cd /share
# tar zxf $HOME/Downloads/sysroot.tar.gz
```

### 配置开发板的WIFI

PX30_mini_EVB_V11_20190507开发板，既可以通过RJ45有线接口连接网络，也可以使用WIFI。开发板中已经使用DHCP配置了有线网络，直接插入网线即可连接局域网。在这里介绍如何设置开发板的WIFI。

参考SDK文档：《PX30 MINI EVB硬件操作指南_20180710.pdf》，将PC机通过串口与PX30连接。在PC机的串口控制台上输入命令：

```
# vi /etc/wpa_supplicant.conf
```

/etc/wpa_supplicant.conf文件有类似如下内容：

```
ctrl_interface=/var/run/wpa_supplicant
ap_scan=1
update_config=1

network={
        ssid="your_wifi_name"
        psk="your_wifi_password"
        key_mgmt=WPA-PSK
}
```

编辑该文件，根据用户的实际网络状况，正确填写文件中的ssid以及psk。然后保存退出。在串口控制台上输入命令：

```
# wifi_start.sh
# ping www.baidu.com
```

如果可以ping通百度，则证明WIFI配置成功。

### 安装HybridOS二进制代码

在串口控制台上输入命令：

```
# mount -t nfs -o nolock PC_ip:/share /mnt
# cd /userdata
# cp /mnt/sysroot . -a
# cd sysroot
# ./setup.sh
# reboot
```

当PX30开发板重新启动后，既可以看见HybridOS界面，并可以通过触摸屏进行操作。
