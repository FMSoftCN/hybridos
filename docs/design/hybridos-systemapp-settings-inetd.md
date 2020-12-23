# 基于hiBus守护进程inetd的概要设计

耿岳

本文阐述了基于hiBus守护进程inetd的实现方式，以及动态库的接口设计。

- [守护进程inetd的软件结构](#守护进程inetd的软件结构)
- [配置文件](#配置文件)
   + [配置文件的内容](#配置文件的内容)
- [守护进程接口](#守护进程接口)
   + [守护进程提供的远程调用](#守护进程提供的远程调用)
   + [守护进程提供的可订阅消息](#守护进程提供的可订阅消息)
- [守护进程工作流程](#守护进程工作流程)
- [动态库需要完成的接口](#动态库需要完成的接口)


## 守护进程inetd的软件结构

目前各个系统的网络连接方式主要有三种：无线（Wifi）、有线（Ethernet）、手机网络（Mobile）。守护进程inetd主要负责管理这些网络连接，包括：监控、连接、切换等。现阶段首先实现WIFI的各项功能。

inetd在系统中的位置如下图：

```c
 ------------------------------------------------------------
|        APP1        |        APP2        |       APP3      |
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

- APP层：可以是JS代码，也可以是C代码，通过HiBus总线，完成用户对网络的管理操作；
- hiBus Server：负责数据通信、消息分发、及连接的管理；
- inetd daemon：负责对网络设备的监控、连接、中断、切换等。通过hiBus Server，为APP层提供设备事件、以及远程过程调用。其只是一个流程框架，具体任务由下面的动态库层完成；
- libxxx.so：每个动态库完成一种网络类型的具体操作；
- Linux Kernel / Drivers：对设备的底层操作。

在不同平台，inetd daemon层代码不会发生任何变化。芯片厂家或用户只需按照动态库接口，完成相应功能即可。



## 配置文件

守护进程的各项设置，依赖于配置文件。

#### 配置文件的内容

配置文件有如下内容：

```
[library]
lib_wifi=wifi               // wifi library, libwifi.so
lib_ethernet=               // ethernet library, libethernet.so
lib_mobile=                 // mobile library, libmobile.so

[wifi]
start_status=1              // whether turn on wifi when device is started
scan_time=30				// interval of scan time. unit: second
signal_time=10              // inervval of check signal strength. unit: second

[ethernet]

[mobile]
```



## 守护进程接口

守护进程为APP管理和操作WIFI，提供了以下远程过程调用，及可订阅事件。

#### 守护进程提供的远程调用

在WIFI操作中，强制规定，远程过程调用的参数，以及执行结果，使用JSON格式字符串。下面仅对每个过程调用的参数及返回值，进行说明。

##### wifiDeviceOperation

```
名称：
	wifiDeviceOperation
参数：
	{
		"device":"device name"
		"operation":1
	}
返回值：
	{
	    "status":
		"ret_code": 1000
	}
```

该调用将打开或者关闭wifi设备。打开设备如果成功，立刻发起搜索wifi热点过程，发送wifi_event_scan事件。

其中:

​		device：为wifi设备名；

​		operation：0表示关闭，1表示打开，2表示状态查询；

​		status：0表示关闭，1表示打开；

​		ret_code: 表示开关成功与否，或错误原因。详见 inetd.h。



##### wifiScanHotspot

```
名称：
	wifiScanHotspot
参数：
	无
返回值：
	[
		{
			"SSID":"fmsoft-dev",
			"encryption":true,
			"signal":65，
			"status":1
		},
		{
			.....
		}
	]
```

其中：

​		status：为1时，表示当前连接，为0时，表示未连接；

​		signal：该网络信号强度，取值范围在0——100之间。

该过程调用将立刻返回inetd维护的当前网络列表，然后inetd发起网络搜索过程，通过并通过事件wifi_event_scan，上报搜索结果。



##### wifiConnect

```
名称：
	wifiConnect
参数：
	{
		"SSID":"fmsoft-dev",
		"password":"hybridos-hibus",
		"autoconn":1,
		"default":1		
	}
返回值：
	{
		"ret_code": 1000
	}

```

其中:

​        autoconn：为1时，表示网络中断自动连接；为0时，表示提示用户，不自动重连；

​        default：为1时，表示默认网络。下次开机时自动连接，动态库需保存该设置到存储器中。

​        ret_code: 表示连接成功，或者失败的原因。详见 inetd.h。



##### wifiDisconnect

```
名称：
	wifiDisconnect
参数：
	无
返回值：
	{
		"ret_code": 1000
	}
```

其中：

​		ret_code: 表示连接成功，或者失败的原因。详见 inetd.h。



##### wifiGetNetworkInfo

```
名称：
	wifiGetInfo
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
		"ret_code": 1000
	}
```

该过程用于获得当前连接的wifi网络的具体信息。其中：

​        ret_code: 表示连接成功，或者失败的原因。详见 inetd.h。



#### 守护进程提供的可订阅消息

##### wifi_event_device

```
名称：wifi_event_broken
参数：
	{
		"device":"device name",
		"status":1,
		"reason":
	}
```

其中：

​		status：1表示打开，0表示关闭；

​		reason：表示设备开关原因，详见 inetd.h。



##### wifi_event_network

```
名称：wifi_event_network
参数：
	{
		"reason":1
		"SSID":"fmsoft-dev",
		"encryption":true,
		"signal":65，
		"status":1
	}
```

其中reason可取以下值：

​		0：网络关闭；

​		1：发现新网络；

​		2：搜索网络过程结束；

​		10：网络连接成功；

​		11：网络连接中断。



##### wifi_event_signal

```
名称：wifi_event_signal
参数：
	{
		"SSID":"fmsoft-dev",
		"signal":65，
	}
```

signal取值范围在0——100之间。只针对已经连接的网络，才会发送该事件。



## 守护进程工作流程

inetd守护进程，仅仅搭建了一个框架。其具体业务的执行，将全部调用动态库提供的接口完成。守护进程的流程如下：

1. 两次fork操作，成为守护进程；
2. 调用hibus_connect_via_unix_socket()，建立与HiBus服务器的连接；
3. 读取配置文件，获得各项参数；
4. 根据配置文件，装载动态库。调用动态库的__wifi_functions_get()函数，获取hiWifiManagerOps结构；
5. 调用hibus_register_procedure()，注册远程过程调用；
6. 调用hibus_register_event()，注册可被订阅事件；
7. 调用hiWifiManagerOps.open()函数，初始化WIFI设备，连接默认网络；
8. 设置超时时间，循环调用hibus_wait_and_dispatch_packet()函数：
   1. 根据配置文件，计算时间，定时调用hiWifiManagerOps.wifiScanHotspot()、hiWifiManagerOps.wifiSignalStrength()函数；
   2. 定时调用hiWifiManagerOps.wifiGetStatus()函数，轮询设备的工作状态；
9. 调用hibus_revoke_event()撤销事件（用不到）、调用hibus_revoke_procedure()撤销远程过程调用（用不到）；
10. 调用hibus_disconnect()中断与HiBus服务器的连接（用不到）；
11. 调用dlclose()，关闭动态库。



## 动态库需要完成的接口

每个动态库，必须完成下面的接口

##### 函数集的获得

在头文件 inetd.h 中，有如下声明：

```
typedef struct HiWifiManagerOps
{
    int (* open) (const char * device_name);
    int (* close) (const char * device_name);
    int (* wifi_connect) (const char * ssid, const char *password);
    int (* wifi_disconnect) (const char * ssid);
    void (* wifi_scan) (wifi_node * wifi, int * number);
    int (* wifi_signal) (void);    
    int (* wifi_status) (wifi_status * param);
} hiWifiManagerOps;
```

动态库需要实现HibusInvokeOps结构中的全部函数。这些函数将被守护进程直接调用，或者被其他端点（endpoint）通过hiBus，远程调用。

守护进程首先调用如下函数，获得工作过程中所需要的全部函数指针。

```
HiWifiManagerOps * __wifi_functions_get();
参数：
	无；
返回值：
	HiWifiManagerOps。	
```



##### 设备的打开

```
int open(const char * device_name);
参数：
	const char * device_name：网络设备名；
返回值：
	0：设备工作正常
	其他值：打开设备失败
```

该函数完成对WIFI设备的初始化。如根据配置文件，搜索并连接默认网络。



##### 设备的关闭

```
int close(const char * device_name);
参数：
	const char * device_name：网络设备名；
返回值：
	0：设备正常关闭
	其他值：关闭设备失败
```

该函数关闭WIFI当前连接，同时完成设备相关软、硬件的资源回收。



##### 网络的连接

```
int wifi_connect(const char * ssid, const char *password);
参数：
	const char * ssid：网络SSID；
	const char *password：网络的密码，如为NULL，表示不需要密码；
返回值：
	0：网络连接成功
	其他值：网络连接失败，具体值详见inetd.h。
```



##### 网络的断开

```
int (* wifi_disconnect) (const char * ssid);

参数：
	const char * ssid：网络SSID；
返回值：
	0：网络断开连接成功
	其他值：网络断开连接失败，具体值详见inetd.h。
```



##### WIFI强度

```
int wifi_signal(void);
参数：
	无
返回值：
	int strength：当前网络信号强度。
```



##### WIFI扫描

```
void wifi_scan(wifi_hotspot * wifi, int * number);
参数：
	wifi_node * wifi[out]：wifi网络描述结构头指针；
	int * number[out]：发现的wifi网络个数；	
返回值：
	无
```



##### WIFI状态

```
int (* wifi_status) (wifi_status * param);
参数：
	wifi_status * param：状态变化结构列表的头指针；
返回值：
	int status：间隔两次wifi_status调用之间，wifi状态变化的个数；
```

该函数将被inetd循环调用。以下几个状态的改变，必须通过该函数上报：

- 设备的状态变化：打开、关闭；
- 网络连接状态的变化：连接、中断；
- 网络搜索工作状态的变化：开始、结束。