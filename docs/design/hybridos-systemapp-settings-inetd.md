# 基于hiBus守护进程inetd的概要设计

耿岳

本文阐述了基于hiBus守护进程inetd的实现方式，以及动态库的接口设计。

- [守护进程inetd的软件结构](#守护进程inetd的软件结构)
- [配置文件](#配置文件)
   + [配置文件的内容](#配置文件的内容)
- [守护进程](#守护进程)
   + [守护进程工作流程](#守护进程工作流程)
   + [守护进程为动态库提供的接口](#守护进程为动态库提供的接口)
   + [守护进程提供的远程调用](#守护进程提供的远程调用)
   + [守护进程提供的可订阅消息](#守护进程提供的可订阅消息)
- [动态库接口](#动态库接口)


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

​        device：为wifi设备名；

​        operation：0表示关闭，1表示打开，2表示状态查询；

​        status：0表示关闭，1表示打开；

​        ret_code: 表示开关成功与否，或错误原因。详见 inetd.h。



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

​        signal：该网络信号强度，取值范围在0——100之间。

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

​        ret_code: 表示连接成功，或者失败的原因。详见 inetd.h。



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

​        status：1表示打开，0表示关闭；

​        reason：表示设备开关原因，详见 inetd.h。



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

​        0：网络关闭；

​        1：发现新网络；

​        2：搜索网络过程结束；

​        10：网络连接成功；

​        11：网络连接中断。



##### wifi_event_signal

```
名称：wifi_event_signal
参数：
	{
		"SSID":"fmsoft-dev",
		"signal":65，
	}
```

只针对已经连接的网络，才会发送该事件。



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



## 动态库接口

每个动态库，必须完成下面的接口

##### 函数集的获得

在头文件 inetd.h 中，有如下声明：

```
typedef struct _HibusInvokeOps
{
    int (* open) (void);
    int (* close) (void);
    void (* wifi_status) (void);
    void (* wifi_scan) (wifi_node * wifi, int * number);
    void (* wifi_signal) (void);
    int (* wifi_connect) (void);
    int (* wifi_disconnect) (void);
} HibusInvokeOps;
```

动态库需要实现HibusInvokeOps结构中的全部函数。这些函数将被守护进程直接调用，或者被其他端点（endpoint）通过hiBus，远程调用。

守护进程首先调用如下函数，获得工作过程中所需要的全部函数指针。

```
HibusInvokeOps * wifi_ops_get();
参数：
	无；
返回值：
	HibusInvokeOps结构指针。	
```



##### 设备的打开

```
int open_device(int * fd_device);
参数：
	fd_device[out]：打开设备的文件描述符指针；
返回值：
	0：设备工作正常
	其他值：打开设备失败
```

该函数完成对WIFI设备的初始化。如根据配置文件，搜索并连接默认网络，设置HiBus通信中，用户自定义数据结构等。

在初始化过程中，如需要打开具体设备，fd_device返回打开设备的文件描述符。如不需要，将文件描述符赋值为-1，或不做任何操作即可。

##### 设备的关闭

```
int close_device(int fd_device);
参数：
	fd_device[in]：打开设备的文件描述符；
返回值：
	0：设备正常关闭
	其他值：关闭设备失败
```

该函数关闭WIFI当前连接，同时完成设备相关软、硬件的资源回收。

##### 设备的读操作

```
void device_read(void);
参数：
	无
返回值：
	无
```

如果在open_device()函数中返回的fd_device是一个合法的文件描述符（不等于 -1），则守护进程将通过select系统调用，监视该文件描述符。一旦该端口有数据，则调用动态库的device_read()函数。动态库需对该文件描述符进行读操作，并处理数据。

如果在open_device()函数中返回的fd_device为-1，则不会调用该函数。

##### WIFI扫描

```
void wifi_scan(wifi_node * wifi, int * number);
参数：
	wifi_node * wifi[out]：wifi网络描述结构头指针；
	int * number[out]：发现的wifi网络个数；	
返回值：
	无
```

守护进程将定时（根据配置文件）调用该函数。该函数负责扫描当前网络，并调用hibus_fire_event()，向hiBus发送“wifi_event_scan”事件。“wifi_event_scan”事件参数见后面的消息接口部分。

##### WIFI强度

```
void wifi_signal(void);
参数：
	无
返回值：
	无
```

守护进程将定时（根据配置文件）调用该函数。该函数负责查询当前连接的信号强度，根据信号强度：

- 如当前无任何网络连接，且没有设置默认连接，不做任何操作，直接返回；
- 如当前网络有连接，则发送“wifi_event_signal”事件，事件参数包含当前网络信号强度；
- 如当前网络中断，则发送"wifi_event_broken"事件；
- 如已经中断的网络信号恢复，且为自动连接，则重新连接网络，并发送"wifi_event_connect"事件；
- 如已经中断的网络信号恢复，且并未设置自动连接，则发送"wifi_event_resume"事件。


