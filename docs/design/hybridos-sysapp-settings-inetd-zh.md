# 合璧操作系统设置应用中网络管理行者的设计

【主题】合璧操作系统设置应用中网络管理行者的设计  
【摘要】本文描述了基于hiBus的行者@localhost/cn.fmsoft.hybridos.settings/inetd，所提供的远程过程及可订阅消息。同时对设备引擎必须完成的接口进行了阐述。  
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
- [inetd行者提供给应用的接口](#inetd行者提供给应用的接口)
   + [inetd行者提供的远程过程](#inetd行者提供的远程过程)
      * [打开网络设备](#打开网络设备)
      * [关闭网络设备](#关闭网络设备)
      * [查询网络设备状态](#查询网络设备状态)
      * [开始扫描网络热点](#开始扫描网络热点)
      * [停止网络热点扫描](#停止网络热点扫描)
      * [连接网络热点](#连接网络热点)
      * [中断网络连接](#中断网络连接)
      * [获得当前网络详细信息](#获得当前网络详细信息)
   + [inetd行者供的可订阅消息](#inetd行者提供的可订阅消息)
      * [网络设备发生变化](#网络设备发生变化)
      * [搜索到新的网络热点](#搜索到新的网络热点)
      * [当前网络信号强度发生变化](#当前网络信号强度发生变化)
- [inetd行者工作流程](#inetd行者工作流程)
- [设备引擎需要完成的接口](#设备引擎需要完成的接口)
   + [WiFi设备引擎](#wifi设备引擎)
      * [函数集的获得](#函数集的获得)
      * [打开设备](#打开设备)
      * [关闭设备](#关闭设备)
      * [连接网络](#连接网络)
      * [断开网络](#断开网络)
      * [获取WiFi信号强度](#获取wifi信号强度)
      * [开始WiFi网络扫描](#开始wifi网络扫描)
      * [停止WiFi网络扫描](#停止wifi网络扫描)
      * [获得热点信息](#获得热点信息)
- [附：商标声明](#附商标声明)


## inetd行者的软件结构

目前各个系统的网络连接方式主要有三种：无线网（WiFi）、以太网（Ethernet）、手机网络（Mobile）。系统设置中的inetd行者，主要负责管理这些网络，包括：连接、监控、切换等。

现阶段首先实现WiFi网络管理的各项功能。

inetd行者在系统中的位置如下图：

```c
 ------------------------------------------------------------
|        APP1        |        APP2        |       APP3       |
 ------------------------------------------------------------
|                      hiBus Server(hiBusd)                  |
 ------------------------------------------------------------ 
|              cn.fmsoft.hybridos.settings/inetd             |
 ------------------------------------------------------------
|    libwifi.so    |   libethernet.so   |    libmobile.so    |
 ------------------------------------------------------------
|                  Linux Kernel/Drivers                      |
 ------------------------------------------------------------
```

整个系统包含如下：

- APP层：可以是JS代码，也可以是C代码，通过hiBus总线，完成用户对网络的操作；
- hiBus Server：负责数据通信、消息分发、及连接的管理；
- cn.fmsoft.hybridos.settings/inetd：inetd行者，负责对网络设备的连接、中断、切换等。通过hiBus Server，为APP层提供设备事件、以及远程过程；
- libxxx.so：网络设备引擎。每个引擎完成一种类型的网络操作；
- Linux Kernel / Drivers：对设备的底层操作。

在不同平台，inetd行者代码不会发生任何变化。芯片厂家或用户只需按照设备引擎接口，完成相应的功能即可。

## 配置文件

inetd行者的各项设置，记录在配置文件里。

### 配置文件的内容

配置文件内容如下：

```c
[device]
device0_name=wlp5s0         // name of network device0
device1_name=eth0           // name of network device1
device2_name=eth1           // name of network device2

[wlp5s0]
type=wifi                   // device type
engine=libwifi.so           // library of device engine
priority=2                  // priority in switch devices
start=disabled              // do not start device when power on
scan_time=30                // interval of scan time. unit: second
signal_time=10              // inervval of check signal strength. unit: second

[eth0]
type=ethernet               // device type
engine=libethernet.so       // library of device engine
priority=1                  // priority in switch devices
start=enabled               // start device when power on

```

## inetd行者提供给应用的接口

inetd负责管理和操作各个网络设备，所提供的远程过程及可订阅事件，如下所述。

### inetd行者提供的远程过程

强制规定：远程过程的参数，以及执行结果，使用JSON格式字符串。下面仅对每个过程的参数及返回值，进行说明。

#### 打开网络设备

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/openDevice`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 关闭网络设备

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/closeDevice`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name"
    }
```
- 返回值：
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 查询网络设备状态

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/getNetworkDevicesStatus`
- 参数：
   + 无
- 返回值：
   + `data`：返回的数据：
     + `device`：网络设备名；
     + `type`：网络设备类型；
     + `status`：网络设备状态；
   + `errMsg`：错误信息；
   + `errCode`：此过程只返回0，表示执行成功。
```json
    { 
        "data":[
                    {
                        "device":"eth0",
                        "type":"<wifi|ethernet|mobile>",
                        "status":"<down|up>"
                    },
                    {
                         ......
                    }
               ],
        "errCode": 0,
        "errMsg": "OK"
    }
```
如没有查到网络设备，则`data`为空数组。


#### 开始扫描网络热点

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiStartScanHotspots`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `data`：返回的数据：
     + `bssid`：
     + `ssid`：网络名称；
     + `frequency`：网络频率；
     + `signalStrength`：网络信号强度；
     + `capabilities`：可用的加密方式；
     + `isConnected`：当前是否连接。
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法。
```json
    {
        "data": [
                    {
                         "bssid": "f0:b4:29:24:18:eb",
                         "ssid": "fmsoft-dev",
                         "frequency": "2427MHZ",
                         "signalStrength": 65,
                         "capabilities": ["WPA-PSK-CCMP+TKIP","WPA2-PSK-CCMP+TKIP","WPS","ESS"],
                         "isConnected":true
                    },
                    {
                         ......
                    }
                ],
        "errCode":0,
        "errMsg":"OK"
    }
```

如没有网络热点，则`data`为空数组。

该过程将立刻返回inetd维护的当前网络列表。网络列表根据信号强度从大到小排列。当前连接的网络，排在第一个。

该过程返回后，inetd行者随即发起WiFi网络扫描。扫描结果通过`WIFINEWHOTSPOTS`泡泡发送给应用。因此调用此过程前，应用务必订阅`WIFINEWHOTSPOTS`事件。

扫描结束后，inetd行者将停止发送`WIFINEWHOTSPOTS`泡泡，因此应用需取消订阅`WIFINEWHOTSPOTS`事件。


#### 停止网络热点扫描 

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiStopScanHotspots`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法。
```json
    {
        "errCode":0,
        "errMsg":"OK"
    }
```
inetd行者将停止正在进行的热点扫描操作，并停止发送`WIFINEWHOTSPOTS`泡泡。因此应用调用该过程后，需取消订阅`WIFINEWHOTSPOTS`事件。


#### 连接网络热点

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiConnect`
- 参数：
   + `device`：网络设备名称；
   + `ssid`：网络名称；
   + `password`：网络密码；
   + `autoConnect`：网络中断后是否自动连接；
   + `default`：是否设置为默认网络，下次开机时自动连接。
```json
    {
        "device":"device_name",
        "ssid":"fmsoft-dev",
        "password":"hybridos-hibus",
        "autoConnect":true,
        "default":true
    }
```
- 返回值：
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法；
     + ECONNREFUSED：因密码不正确，导致连接被拒绝。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 中断网络连接

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiDisconnect`
- 参数：
   + `device`：网络设备名称；
```json
    { 
        "device":"device_name",
    }
```
- 返回值：
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + ENODEV：没找到相应的设备；
     + EACCES：没有操作设备权限；
     + EINVAL：参数不合法。
```json
    { 
        "errCode":0,
        "errMsg":"OK"
    }
```

#### 获得当前网络详细信息

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/wifiGetNetworkInfo`
- 参数：
   + 无；
- 返回值：
   + `data`：返回的数据：
     + `device`：网络设备名称；
     + `ssid`：网络名称；
     + `encryptionType`：加密方式；
     + `signalStrength`：信号强度；
     + `MAC`：MAC地址；
     + `IP`：IP地址；
     + `frenquency`：网络信号频率；
     + `speed`：网络速度；
     + `gateway`：网关。
   + `errMsg`：错误信息；
   + `errCode`：返回错误编码，0表示执行成功。可能的错误编码有：
     + EACCES：没有操作设备权限。
```json
    { 
        "data":{
                    "device":"device_name",
                    "ssid":"fmsoft-dev",
                    "encryptionType":"WPA2",
                    "signalStrength":65,
                    "MAC":"AB:CD:EF:12:34:56",
                    "IP":"192.168.1.128",
                    "frenquency":"5 GHz",
                    "speed":"650 Mbps",
                    "gateway":"192.168.1.1"
                },
        "errCode":0,
        "errMsg":"OK"
    }
```

如没有查到当前网络详细信息，则`data`为`NULL`。

### inetd行者提供的可订阅消息

#### 网络设备发生变化 

- 泡泡名称：`NETWORKDEVICECHANGED`
- bubbleData：
   + `device`：网络设备名称；
   + `type`：网络类型；
   + `status`：设备状态。
```json
    { 
        "device":"device_name",
        "type":"<wifi|ethernet|mobile>",
        "status":"<down|up>"
    }
```
- 使用描述：
   + 当网络设备工作状态发生变化时，发送此消息。

#### 搜索到新的网络热点

- 泡泡名称：`WIFINEWHOTSPOTS`
- bubbleData：
   + `bssid`：
   + `ssid`：网络SSID；
   + `encryption`：网络是否加密；
   + `capabilities`：设备支持的加密方式；
   + `signalStrength`：取值范围在0——100之间。
```json
    [    
        { 
            "bssid": "f0:b4:29:24:18:eb",
            "ssid":"fmsoft-dev",
            "encryption":true,
            "capabilities": ["WPA-PSK-CCMP+TKIP", "WPA2-PSK-CCMP+TKIP", "WPS", "ESS"]
            "signalStrength":65
        },
        {
            ......
        }
    ]
```
- 使用描述：
   + 当`bubbleData`数组长度为0时，表示一次搜索过程完毕。

#### 当前网络信号强度发生变化

- 泡泡名称：`WIFISIGNALSTRENGTHCHANGED`
- bubbleData：
   + `ssid`：网络SSID；
   + `signalStrength`：网络信号强度，取值范围在0——100之间。
```json
    {
        "ssid":"fmsoft-dev",
        "signalStrength":65
    }
```
- 使用描述：
   + `signalStrength`为0时，表明网络中断；
   + 当网络连接成功后，才开始发送该泡泡。可通过是否接受到该泡泡，判断网络是否连接（适用于网络中断自动连接的情况）；
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
11. 调用`dlclose()`，关闭设备引擎。

## 设备引擎需要完成的接口

### WiFi设备引擎

设备引擎必须完成如下描述的所有接口，供inetd行者调用。

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
    char encryptionType[32];
    char frenquency[32];
    char speed[32];
    int  signal_strength;
    bool isConnect;
} wifi_hotspot;

typedef struct _hiWiFiDeviceOps
{
    wifi_context * (* open) (const char * device_name);
    int (* close) (wifi_context * context);
    int (* connect) (wifi_context * context, const char * ssid, const char *password);
    int (* disconnect) (wifi_context * context);
    int (* get_signal_strength) (wifi_context * context);    
    int (* start_scan) (wifi_context * context);
    int (* stop_scan) (wifi_context * context);
    int (* get_hotspots) (wifi_context * context, wifi_hotspot ** hotspots);
} hiWiFiDeviceOps;
```

#### 函数集的获得

inetd行者首先调用如下函数，获得操作WiFi设备所需的全部函数指针。

```c
const hiWiFiDeviceOps * __wifi_device_ops_get(void);
```
- 参数：
   + 无 
- 返回值：`hiWiFiDeviceOps`结构指针。如返回值为`NULL`，表示该函数执行失败。


#### 打开设备

根据WiFi设备名，完成对该设备的初始化。

```c
wifi_context * open (const char * device_name);
```

- 参数：
   + `device_name`：网络设备名； 
- 返回值：
   + `wifi_context`结构指针，为设备引擎工作的上下文。该结构由设备引擎自行声明及定义。如果返回值为`NULL`，表示该函数执行失败。 

#### 关闭设备

关闭WiFi设备，同时完成相关软、硬件的资源回收。

```c
int close(wifi_context * context);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：设备正常关闭；
   + `-1`：设备操作错误代码。 

#### 连接网络

```c
int connect(wifi_context * context, const char * ssid, const char *password);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `ssid`：网络SSID； 
   + `password`：网络的密码，如为`NULL`，表示不需要密码； 
- 返回值：
   + `0`：连接指定网络成功；
   + `-1`：连接操作错误代码。 

#### 断开网络

```c
int disconnect(wifi_context * context);

```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + `0`：断开连接成功；
   + `-1`：断开连接操作错误代码。 

#### 获取WiFi信号强度

```c
int get_signal_strength(wifi_context * context); 
```

- 参数：
   + `context`：设备引擎工作的上下文； 
- 返回值：
   + 当前网络信号强度，取值范围为0——100。

#### 开始WiFi网络扫描

WiFi网络扫描是一个异步过程。inetd行者调用该函数后立刻返回。设备引擎开始扫描，其扫描结果将通过inetd行者调用`hiWiFiDeviceOps->get_hotspots()`函数返回。

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

inetd行者定时调用该函数，获得WiFi热点的信息。

```c
int get_hotspots (wifi_context * context, wifi_hotspot ** hotspots);
```

- 参数：
   + `context`：设备引擎工作的上下文； 
   + `hotspots`：`wifi_hotspot`结构数组头指针的指针； 
- 返回值：
   + `hotspots`数组的个数。

`hotspots`数组由设备引擎维护，负责内存空间的开辟及回收。

当设备引擎搜索热点完毕后，在hotspots数组最后添加一个元素，并赋值`signal_strength = 0`，表示搜索过程完毕。

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

