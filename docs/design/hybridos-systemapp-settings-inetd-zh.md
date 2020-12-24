【主题】基于hiBus守护进程inetd的概要设计  
【摘要】本文阐述了基于hiBus守护进程inetd的实现方式，以及动态库的接口设计。  
【版本】1.0.1  
【作者】David  
【日期】2020.12.24  
【状态】Draft    

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

本文涉及到的飞漫软件或其合作伙伴的注册商标或商标，有关其详细列表，请查阅文档末尾。

**目录**
- [1 守护进程inetd的软件结构](#1-守护进程inetd的软件结构)
- [2 配置文件](#2-配置文件)
   + [2.1 配置文件的内容](#2.1-配置文件的内容)
- [3 守护进程接口](#3-守护进程接口)
   + [3.1 守护进程提供的远程过程](#3.1-守护进程提供的远程过程)
   + [3.2 守护进程提供的可订阅消息](#3.2-守护进程提供的可订阅消息)
- [4 守护进程工作流程](#4-守护进程工作流程)
- [5 动态库需要完成的接口](#5-动态库需要完成的接口)
- [附：商标声明](#附商标声明)


## 1 守护进程inetd的软件结构

目前各个系统的网络连接方式主要有三种：无线（WiFi）、有线（Ethernet）、手机网络（Mobile）。守护进程inetd主要负责管理这些网络连接，包括：监控、连接、切换等。现阶段首先实现WiFi的各项功能。

inetd在系统中的位置如下图：

```c
 ------------------------------------------------------------
|        APP1        |        APP2        |       APP3       |
 ------------------------------------------------------------
|                      hiBus Server(hiBusd)                  |
 ------------------------------------------------------------ 
|                         inetd daemon                       |
 ------------------------------------------------------------
|    libwifi.so    |   libethernet.so   |    libmobile.so    |
 ------------------------------------------------------------
|                  Linux Kernel/Drivers                      |
 ------------------------------------------------------------
```

整个系统包含如下：

- APP层：可以是JS代码，也可以是C代码，通过hiBus总线，完成用户对网络的管理操作；
- hiBus Server：负责数据通信、消息分发、及连接的管理；
- inetd daemon：负责对网络设备的监控、连接、中断、切换等。通过hiBus Server，为APP层提供设备事件、以及远程过程。其只是一个流程框架，具体任务由下面的动态库层完成；
- libxxx.so：每个动态库完成一种网络类型的具体操作；
- Linux Kernel / Drivers：对设备的底层操作。

在不同平台，inetd daemon层代码不会发生任何变化。芯片厂家或用户只需按照动态库接口，完成相应功能即可。



## 2 配置文件

守护进程的各项设置，依赖于配置文件。

### 2.1 配置文件的内容

配置文件有如下内容：

```
[library]
wifi=wifi                   // wifi library, libwifi.so
#ethernet=                  // ethernet library, libethernet.so
#mobile=                    // mobile library, libmobile.so

[wifi]
start=enabled               // whether turn on wifi when device is started
scan_time=30                // interval of scan time. unit: second
signal_time=10              // inervval of check signal strength. unit: second

[ethernet]

[mobile]
```



## 3 守护进程接口

守护进程为APP管理和操作WiFi，提供了的远程过程及可订阅事件如下。



### 3.1 守护进程提供的远程过程

在WiFi操作中，强制规定，远程过程的参数，以及执行结果，使用JSON格式字符串。下面仅对每个过程的参数及返回值，进行说明。

##### wifiOperateDevice

```
名称：
	wifiOperateDevice
参数：
	{
		"device":"device_name",
		"operation":"on"
	}
返回值：
	{
		"reason": 1000
	}
```

该过程负责打开或者关闭WiFi设备。打开设备如果成功，立刻发起WiFi热点搜索过程。

其中:

​		device：WiFi设备名；

​		operation：on / off；

​		reason: 表示开关成功与否，或错误原因。详见 inetd.h。



##### wifiGetDeviceInfo

```
名称：
	wifiGetDeviceInfo
参数：
	{
		"device":"device name",
	}
返回值：
	{
	    "status":"on"
	}
```

该过程用于查询WiFi设备的打开状态。

其中:

​		device：为WiFi设备名；

​		status：on / off。



##### 不可见网络的单独接口???



##### wifiGetHotspots

```
名称：
	wifiScanHotspots
参数：
	{
		"startScan":true
	}
返回值：
	[
		{
			"SSID":"fmsoft-dev",
			"encryptionType":“WPA",
			"signalStrength":65，
			"isConnected":true
		},
		{
			.....
		}
	]
```

该过程将立刻返回inetd维护的当前网络列表。如果"startScan"为true，inetd立刻发起网络搜索过程，通过事件networkHotspotChange，上报搜索结果。

其中：

​		startScan：是否要求inetd重新发起网络搜索；

​		signalStrength：整数，网络信号强度，取值范围在0——100之间；



##### wifiConnectHotspot

```
名称：
	wifiConnectHotspot
参数：
	{
		"SSID":"fmsoft-dev",
		"password":"hybridos-hibus",
		"autoconn":"enabled",
		"default":"enabled"
	}
返回值：
	{
		"reason": 1000
	}

```

其中:

​        autoconn：表示网络中断后是否自动连接；

​        default：表示是否设置为默认网络，下次开机时自动连接。动态库需保存该设置到存储器中。

​        reason: 表示连接成功，或者失败的原因。详见 inetd.h。



##### wifiDisconnect

```
名称：
	wifiDisconnect
参数：
	无
返回值：
	{
		"reason": 1000
	}
```

其中：

​		reason: 表示连接成功，或者失败的原因。详见 inetd.h。



##### wifiGetNetworkInfo

```
名称：
	wifiGetNetworkInfo
参数：
	无
返回值：
	{
		"SSID":"fmsoft-dev",
		"encryption":true,
		"signal":65，
		"MAC":"AB:CD:EF:12:34:56",
		"IP":"192.168.1.128",
		"type":"WPA2",
		"frenquency":"5 GHz",
		"speed":"650 Mbps"
		“gateway”:"192.168.1.1"
		"reason": 1000
	}
```

该过程用于获得当前连接的WiFi网络的具体信息。

其中：

​        reason: 表示该过程失败的原因。详见 inetd.h。



### 3.2 守护进程提供的可订阅消息

##### networkDeviceChange

```
名称：networkDeviceChange
参数：
	{
		"device":"device name",
		"status":"on",
		"reason":1000
	}
```

其中：

​		status：on / off；

​		reason：表示设备状态变化原因，详见 inetd.h。



##### networkStatusChange

```
名称：networkStatusChange
参数：
	{
		"nettype":"wifi";
		"SSID":"fmsoft-dev",
		"status":"on"
		"reason":1000
	}
```

其中：

​		nettype：网络类型。可选值为：wifi / ethernet / mobile；

​		status：on / off；

​		reason：表示设备状态变化原因，详见 inetd.h。



##### networkHotspotChange

```
名称：networkHotspotChange
参数：
	{
		"SSID":"fmsoft-dev",
		"encryption":true,
		"changeType":"visible",
		"status":"on"
		"reason":1000
	}
```

其中：

​		changeType：WiFi热点变化类型。可选值为：visible / connect / scan，对应可查询、连接、搜索；

​		status：对于"changeType"为"scan"时，on表示开始搜索，off表示搜索结束。



##### signalStrenthChange

```
名称：signalStrenthChange
参数：
	{
		"SSID":"fmsoft-dev",
		"signalStrength":65，
	}
```

其中：

​		signalStrength：取值范围在0——100之间。只针对已经连接的网络，才会发送该事件。



## 4 守护进程工作流程

inetd守护进程，仅仅搭建了一个框架。其具体业务的执行，将全部调用动态库提供的接口完成。守护进程的流程如下：

1. 两次fork操作，成为守护进程；
2. 调用hibus_connect_via_unix_socket()，建立与hiBus服务器的连接；
3. 读取配置文件，获得各项参数；
4. 根据配置文件，装载动态库。调用动态库的__wifi_get_Ops()函数，获取hiWiFiManagerOps结构；
5. 调用hibus_register_procedure()，注册远程过程；
6. 调用hibus_register_event()，注册可被订阅事件；
7. 调用hiWifiManagerOps.open()函数，初始化WiFi设备，连接默认网络；
8. 设置超时时间，循环调用hibus_wait_and_dispatch_packet()函数：
   1. 根据配置文件，计算时间，定时调用hiWiFiManagerOps.wifiScan()、hiWiFiManagerOps.wifiSignalStrength()函数；
   2. 定时调用hiWifiManagerOps.wifiGetStatus()函数，轮询设备的工作状态；
9. 调用hibus_revoke_event()撤销事件（用不到）、调用hibus_revoke_procedure()撤销远程过程（用不到）；
10. 调用hibus_disconnect()中断与hiBus服务器的连接（用不到）；
11. 调用dlclose()，关闭动态库。



## 5 动态库需要完成的接口

每个动态库，必须完成下面的接口。



##### 函数集的获得

在头文件 inetd.h 中，有如下声明：

```
typedef struct __wifi_hotspot
{
	unsigned char ssid[40];
	char mac_address[32];
	char IP_address[24];
	char gateway[24];
	char encryption[32];
	char frenquency[32];
	char speed[32];
	int signalStrength;
	bool isConnect;
} wifi_hotspot;

typedef struct __hiWiFiManagerOps
{
	int (* open) (const char * device_name, wifi_context ** context);
	int (* close) (wifi_context * context);
	int (* wifiConnect) (const char * ssid, const char *password, wifi_context * context);
	int (* wifiDisconnect) (wifi_context * context);
	int (* wifiScan) (wifi_context * context);
	int (* wifiSignalStrength) (wifi_context * context);    
	int (* wifiGetHotspots) (wifi_status * param, wifi_context * context);
	void (* wifiSetInterval) (int scan, int signalStrength);
} hiWiFiManagerOps;
```

动态库需要实现hiWiFiManagerOps结构中的全部函数。这些函数仅被守护进程直接调用。

守护进程首先调用如下函数，获得工作过程中所需要的全部函数指针。

```
const hiWiFiManagerOps * __wifi_get_Ops();
参数：
	无；
返回值：
	hiWiFiManagerOps。	
```



##### 设备的打开

```
int (* open) (const char * device_name, wifi_context ** context);
参数：
	const char * device_name：网络设备名；
	wifi_context ** context：动态库工作的上下文。wifi_context结构由动态库自行声明及定义；
返回值：
	0：设备工作正常
	其他值：打开设备失败，详见头文件 inetd.h。
```

该函数完成对WIFI设备的初始化。如根据配置文件，搜索并连接默认网络。



##### 设备的关闭

```
int close(wifi_context * context);
参数：
	wifi_context * context：动态库工作的上下文；
返回值：
	0：设备正常关闭
	其他值：关闭设备失败，详见头文件 inetd.h。
```

该函数关闭WIFI当前连接，同时完成设备相关软、硬件的资源回收。



##### 网络的连接

```
int wifiConnect(const char * ssid, const char *password, wifi_context * context);
参数：
	const char * ssid：网络SSID；
	const char *password：网络的密码，如为NULL，表示不需要密码；
	wifi_context * context：动态库工作的上下文；
返回值：
	0：网络连接成功
	其他值：网络连接失败，具体值详见inetd.h。
```



##### 网络的断开

```
int wifiDisconnect(wifi_context * context);

参数：
	wifi_context * context：动态库工作的上下文；
返回值：
	0：网络断开连接成功
	其他值：网络断开连接失败，具体值详见inetd.h。
```



##### WIFI强度

```
int wifiSignalStrength(wifi_context * context); 
参数：
	wifi_context * context：动态库工作的上下文；
返回值：
	int strength：当前网络信号强度，取值范围为0——100。
```



##### WIFI扫描

```
int wifiScan(wifi_context * context);
参数：
	wifi_context * context：动态库工作的上下文；
返回值：
	0：网络断开连接成功
	其他值：网络断开连接失败，具体值详见inetd.h。
```

该函数是一个异步过程。inetd调用该函数后立刻返回。动态库开始扫描，其扫描结果通过inetd调用wifi_get_info()函数返回。



##### 获得热点信息

```
int wifiGetHotspots(wifi_hotspot * hotspots);
参数：
	wifi_hotspot * hotspots：wifi_hotspot数组的头指针；
返回值：
	hotspots数组的个数；
```

hotspots数组由动态库维护，负责内存空间的开辟及回收。

当搜索热点完毕后，最后需要发送一个 ssid[0] = 0 的wifi_hotspot结构，以表示搜索过程完毕。



##### 设置定时间隔

```
void wifiSetInterval (int scan, int signalStrength);
参数：
	int scan：网络扫描的时间间隔。单位：秒；
	int signalStrength：检测网络信号强度时间间隔。单位：秒。
返回值：
	无
```

## 附：商标声明

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

