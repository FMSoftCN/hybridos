【主题】合璧操作系统设置应用的网络管理行者设计  
【摘要】本文阐述了基于hiBus守护进程inetd的实现方式，以及动态库的接口设计。  
【版本】1.0  
【作者】耿岳  
【日期】2020 年 12 月  
【状态】草稿  

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

本文涉及到的飞漫软件或其合作伙伴的注册商标或商标，有关其详细列表，请查阅文档末尾。

**目录**
- [inetd行者的软件结构](#inetd行者的软件结构)
- [配置文件](#配置文件)
   + [配置文件的内容](#配置文件的内容)
- [inetd行者接口](#inetd行者接口)
   + [inetd行者提供的远程过程](#inetd行者提供的远程过程)
   + [inetd行者供的可订阅消息](#inetd行者提供的可订阅消息)
- [inetd行者工作流程](#inetd行者工作流程)
- [设备引擎需要完成的接口](#设备引擎需要完成的接口)
- [附：商标声明](#附商标声明)


## inetd行者的软件结构

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
- inetd daemon：负责对网络设备的监控、连接、中断、切换等。通过hiBus Server，为APP层提供设备事件、以及远程过程；
- libxxx.so：每个动态库完成一种网络类型的具体操作；
- Linux Kernel / Drivers：对设备的底层操作。

在不同平台，inetd daemon层代码不会发生任何变化。芯片厂家或用户只需按照动态库接口，完成相应功能即可。



## 配置文件

inetd行者的各项设置，依赖于配置文件。

### 配置文件的内容

配置文件有如下内容：

```c
[library]
wifi=wifi                   // wifi library, libwifi.so
#ethernet=                  // ethernet library, libethernet.so
#mobile=                    // mobile library, libmobile.so

[wifi]
device0_name=wlp5s0         // WiFi device name
priority=2                  // the priority of wifi
start=disabled               // whether turn on wifi when device is started
scan_time=30                // interval of scan time. unit: second
signal_time=10              // inervval of check signal strength. unit: second

[ethernet]
device0_name=eth0           // ethernet device0 name
device1_name=eth1           // ethernet device1 name
priority=3                  // the priority of ethernet
start=enabled               // whether turn on ethernet when device is started

[mobile]
priority=1                  // the priority of mobile 
start=disabled              // whether turn on mobile when device is started


```

## inetd行者接口

inetd行者APP管理和操作WiFi，提供了的远程过程及可订阅事件如下。


### inetd行者提供的远程过程

在WiFi操作中，强制规定，远程过程的参数，以及执行结果，使用JSON格式字符串。下面仅对每个过程的参数及返回值，进行说明。

#### 打开WiFi设备

该过程负责打开指定的WiFi设备。如果打开设备成功，根据配置判断是否发起WiFi热点搜索过程。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiOpenDevice`
- parameter：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- retValue：
   + `errCode`：返回错误编码，200为执行正确；
   + `errMsg`：错误信息；
```json
    { 
        "errCode":200,
        "errMsg":"OK"
    }
```

#### 关闭WiFi设备

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiCloseDevice`
- parameter：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- retValue：
   + `errCode`：返回错误编码，200为执行正确；
   + `errMsg`：错误信息；
```json
    { 
        "errCode":200,
        "errMsg":"OK"
    }
```


#### 查询网络设备状态

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiGetDeviceStatus`
- parameter：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- retValue：
   + `status`：网络设备状态。取值为 on / off；
```json
    { 
        "status":"on"
    }
```


#### 获得网络热点列表

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiGetHotspots`
- parameter：
   + `startScan`：获得列表后，inetd是否立刻发起搜索过程；
   + `fromIndex`：从列表的第几个开始；
   + `toIndex`：到列表的第几个结束；
```json
    {
        "startScan":true,
        "fromIndex":0,
        "toIndex":10
    }
```
- retValue：
   + `bssid`：
   + `ssid`：网络名称；
   + `frequency`：网络频率；
   + `signalStrength`：网络信号强度；
   + `capabilities`：可用的加密方式；
   + `isConnected`：当前是否连接。
```json
    [
        {
            "bssid": "f0:b4:29:24:18:eb",
            "ssid": "fmsoft-dev",
            "frequency": "2427MHZ",
            "signalStrength": 65,
            "capabilities": ["WPA-PSK-CCMP+TKIP","WPA2-PSK-CCMP+TKIP","WPS","ESS"],
            "isConnected":true
        },
        {
            .....
        }
    ]
```

该过程将立刻返回inetd维护的当前网络列表。网络列表根据信号强度排列，当前连接的网络，排在第一个。


#### 连接网络热点

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiConnect`
- parameter：
   + `ssid`：网络名称；
   + `password`：网络密码；
   + `autoConnect`：网络中断后是否自动连接；
   + `default`：是否设置为默认网络，下次开机时自动连接。inetd行者需保存该设置到存储器中。；
```json
    {
        "SSID":"fmsoft-dev",
        "password":"hybridos-hibus",
        "autoConnect":true,
        "default":true
    }
```
- retValue：
   + `errCode`：返回错误编码，200为执行正确；
   + `errMsg`：错误信息；
```json
    { 
        "errCode":200,
        "errMsg":"OK"
    }
```

#### 中断网络连接

wifiDisconnect

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiDisconnect`
- parameter：
   + 无；
- retValue：
   + `errCode`：返回错误编码，200为执行正确；
   + `errMsg`：错误信息；
```json
    { 
        "errCode":200,
        "errMsg":"OK"
    }
```

#### 获得当前网络详细信息

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiGetNetworkInfo`
- parameter：
   + 无；
- retValue：
   + `ssid`：网络名称；
   + `encryption`：加密方式；
   + `signalStrength`：信号强度；
   + `MAC`：MAC地址；
   + `IP`：IP地址；
   + `frenquency`：网络信号频率；
   + `speed`：网络速度；
   + `gateway`：网关。

```json
    { 
        "ssid":"fmsoft-dev",
        "encryption":"WPA2",
        "signalStrength":65，
        "MAC":"AB:CD:EF:12:34:56",
        "IP":"192.168.1.128",
        "frenquency":"5 GHz",
        "speed":"650 Mbps"
        "gateway":"192.168.1.1"
    }
```

### inetd行者提供的可订阅消息

#### 网络设备发生变化 

- 泡泡名称：`NETWORKDEVICECHANGED`
- bubbleData：
   + `device`：网络设备名称；
   + `status`：设备状态，取值为 on / off；
```json
    { 
        "device":"device name",
        "status":"on"
    }
```
- 使用描述：
   + 当网络设备工作状态发生变化时，发送此消息。


#### 网络热点发生变化

- 泡泡名称：`HOTSPOTCHANGED`
- bubbleData：
   + `bssid`：
   + `ssid`：网络SSID；
   + `encryption`：网络是否加密；
   + `capabilities`：网络许可加密方式；
   + `signalStrength`：取值范围在0——100之间；
   + `available`：网络是否可被搜索到；
```json
    { 
        "bssid": "f0:b4:29:24:18:eb",
        "ssid":"fmsoft-dev",
        "encryption":true,
        "capabilities": ["WPA-PSK-CCMP+TKIP", "WPA2-PSK-CCMP+TKIP", "WPS", "ESS"]
        "signalStrength":65,
        "available":true
    }
```
- 使用描述：
   + 当网络信号强度在两次检查过程中，变化值在20以上，视为该网络状态发生变化；
   + 在一次WiFi搜索时，某个网络不能再被搜索到，视为该网络状态发生变化；
   + 在一次WiFi搜索时，某个网络的加密方式发生变化，视为该网络状态发生变化？？？
   + 在一次WiFi搜索时，某个网络的BSSID发生变化，视为该网络状态发生变化？？？
   + 当`available`为`true`时，表明其为一个新搜索到的网络；为`false`时，表明该网络不可再被搜索到。其余情况该项为`NULL`；
   + 当`ssid`为`NULL`时，表示一次搜索过程完毕。


#### 网络信号强度

- 泡泡名称：`SIGNALSTRENGTHCHANGED`
- bubbleData：
   + `ssid`：网络SSID；
   + `signalStrength`：取值范围在0——100之间。
```json
    {
        "ssid":"fmsoft-dev",
        "signalStrength":65
    }
```
- 使用描述：
   + `signalStrength`不为0时，表明名为“fmsoft-dev”的网络已经连接；
   + `signalStrength`为0时，表明名为“fmsoft-dev”的网络中断；
   + 当前网络中断后，不会发送该事件。


## inetd行者工作流程

inetd行者工作流程如下：

1. 两次fork操作，成为守护进程；
2. 调用`hibus_connect_via_unix_socket()`，建立与hiBus服务器的连接；
3. 读取配置文件，获得各项参数；
4. 根据配置文件，装载设备引擎。调用`__wifi_device_ops_get()`函数，获取`hiWiFiDeviceOps`结构指针；
5. 调用`hibus_register_procedure()`注册远程过程；
6. 调用`hibus_register_event()`注册可被订阅事件；
7. 调用`hiWiFiDeviceOps->open()`函数，初始化WiFi设备，连接默认网络；
8. 设置超时时间，循环调用`hibus_wait_and_dispatch_packet()`函数：
   1. 根据配置文件，计算时间，定时调用`hiWiFiDeviceOps->start_scan()`、`hiWiFiDeviceOps->get_signal_strength()`函数；
   2. 定时调用`hiWiFiDeviceOps->get_hotspots()`函数，轮询网络状态；
9. 调用`hibus_revoke_event()`撤销事件（用不到）、调用`hibus_revoke_procedure()`撤销远程过程（用不到）；
10. 调用`hibus_disconnect()`中断与hiBus服务器的连接（用不到）；
11. 调用`dlclose()`，关闭动态库。



## 设备引擎需要完成的接口

每个设备引擎必须完成如下描述的接口，供inetd行者调用。


在头文件 inetd.h 中，有如下声明：

```c
struct _wifi_context;
typedef struct _wifi_context wifi_context;

typedef struct _wifi_hotspot
{
	unsigned char ssid[40];
	char mac_address[32];
	char IP_address[24];
	char gateway[24];
	char encryption[32];
	char frenquency[32];
	char speed[32];
	int  signal_strength;
	bool isConnect;
} wifi_hotspot;

typedef struct _hiWiFiDeviceOps
{
	wifi_context * (* open) (const char * device_name);
	int (* close) (wifi_context * context);
	int (* connect) (const char * ssid, const char *password, wifi_context * context);
	int (* disconnect) (wifi_context * context);
	int (* get_signal_strength) (wifi_context * context);    
	int (* start_scan) (wifi_context * context);
    int (* stop_scan) (wifi_context * context);
	int (* get_hotspots) (wifi_context * context, wifi_hotspot ** hotspots);
	void (* set_interval) (int scan_interval, int signal_interval);
} hiWiFiDeviceOps;
```

设备引擎需要实现`hiWiFiDeviceOps`结构中的全部函数。这些函数仅被inetd行者调用。

#### 函数集的获得

inetd行者首先调用如下函数，获得操作WiFi设备所需的全部函数指针。

```c
const hiWiFiDeviceOps * __wifi_device_ops_get();
```
- 参数：
   + 无 
- 返回值：`hiWiFiDeviceOps`结构指针。如返回值为NULL，表示该函数执行失败。


#### 设备的打开

根据WiFi设备名，完成对该设备的初始化。并根据配置文件，搜索并连接默认网络。

```c
wifi_context * open (const char * device_name);
```

- 参数：
   + `device_name`：网络设备名； 
- 返回值：
   + `wifi_context`结构指针，是为设备引擎工作的上下文。该结构由设备引擎自行声明及定义。如果返回值为NULL，表示该函数执行失败。 


#### 设备的关闭

关闭WIFI当前连接，同时完成设备相关软、硬件的资源回收。

```c
int close(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：设备正常关闭；
   + `-1`：设备操作错误代码。 


#### 网络的连接

```c
int connect(wifi_context * context, const char * ssid, const char *password);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `ssid`：网络SSID； 
   + `password`：网络的密码，如为NULL，表示不需要密码； 
- 返回值：
   + `0`：连接指定网络成功；
   + `-1`：连接操作错误代码。 


#### 网络的断开

```c
int disconnect(wifi_context * context);

```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：断开连接成功；
   + `-1`：断开连接操作错误代码。 


#### WIFI强度

```c
int get_signal_strength(wifi_context * context); 
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + 当前网络信号强度，取值范围为0——100。


#### 开始WiFi网络扫描

该函数是一个异步过程。inetd行者调用该函数后立刻返回。设备引擎开始扫描，其扫描结果将通过inetd行者调用`hiWiFiDeviceOps->get_hotspots()`函数返回。

```c
int start_scan(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：开始网络扫描成功；
   + `-1`：设备操作错误代码。 


#### 停止WiFi网络扫描

该函数终止正在进行的WiFi网络扫描过程。

```c
int stop_scan(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：终止网络扫描成功；
   + `-1`：设备操作错误代码。 


#### 获得热点信息

inetd行者定时调用该函数，获得WiFi热点的信息。`hotspots`数组由设备引擎维护，负责内存空间的开辟及回收。
当设备引擎搜索热点完毕后，最后需要发送一个 ssid[0] = 0 的`wifi_hotspot`结构，以表示搜索过程完毕。


```c
int get_hotspots (wifi_context * context, wifi_hotspot ** hotspots);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `hotspots`：`wifi_hotspot`结构数组头指针的指针； 
- 返回值：
   + `hotspots`数组的个数。



##### 设置定时间隔

inetd行者根据当前网络状态，动态改变设备引擎扫描网络、检查网络信号强度的时间间隔。

```c
void set_interval (wifi_context * context, int scan_interval, int signal_interval);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `scan_interval`：扫描网络时间间隔； 
   + `signal_interval`：检查网络信号强度时间间隔； 
- 返回值：
   + 无。


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

