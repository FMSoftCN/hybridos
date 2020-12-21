# 基于hiBus守护进程inetd的概要设计

耿岳

本文阐述了基于hiBus守护进程inetd的实现方式，以及动态库的接口设计。

- [守护进程inetd的软件结构](#守护进程inetd的软件结构)
- [配置文件](#配置文件)
   + [配置文件的路径](#配置文件的路径)
   + [配置文件的内容](#配置文件的内容)
- [守护进程](#守护进程)
- [动态库接口](#动态库接口)
   + [守护进程调用接口](#守护进程调用接口)
   + [远程过程调用接口](#远程过程调用接口)
   + [可订阅的消息](#可订阅的消息)


## 守护进程inetd的软件结构

守护进程inetd在系统中的位置如下图：

```c
 ------------------------------------------------------------
|          APP1       |       APP2       |      APP3         |
 ------------------------------------------------------------
|                      hiBus Server(hiBusd)                  |
 ------------------------------------------------------------ 
|                         inetd daemon                       |
 ------------------------------------------------------------
|  libwifi.so  |  libwifi-simulator.so  |  libotherwifi.so   |
 ------------------------------------------------------------
|                  Linux Kernel/Drivers                      |
 ------------------------------------------------------------
```

整个系统包含如下：

- APP层：这些APP可以是JS代码，也可以是C代码，都通过HiBus总线，与HiBus服务器通信；
- hiBus Server：负责数据通信、消息分发、及连接的管理；
- inetd daemon：负责对wifi设备的监控，通过hiBus Server，为APP层提供设备事件、以及远程过程调用。其只是一个流程框架，具体任务由下面的动态库层完成；
- libxxx.so：通过hiBus为APP层提供事件、远程过程调用；为inetd daemon提供接口，完成指定功能；
- Linux Kernel / Drivers：对设备的底层操作。

在不同平台，inetd daemon层代码不会发生任何变化。芯片厂家或用户只需按照动态库接口，完成相应功能即可。inetd daemon会根据配置文件，装载正确的动态库并运行。比如在开发前期，使用wifi模拟库，以调试各个APP。在开发后期，使用libwifi.so，对真实硬件进行操作。



## 配置文件

守护进程的各项设置，依赖于配置文件。

#### 配置文件的路径

配置文件的名称固定为：inetd.cfg。其路径可从以下两种方式获得：

- 环境变量：INETD_CFG_PATH
- 与守护进程inetd相同的路径下。

#### 配置文件的内容

配置文件有如下内容：

```
[system]
library_path=/home/projects/hibusdaemon/lib		// path of library 
lib=wifi										// name of library, libwifi.so
scan_time=30									// interval of scan time. unit: second
signal_time=10									// inervval of check signal strength. unit: second

[procedure]
for_host=localhost                              // NULL for all hosts
for_app=cn.fmsoft.hybridos.appagent				// NULL for all applications

[event]
for_host=localhost								// NULL for all hosts
for_app=cn.fmsoft.hybridos.appagent				// NULL for all applications
```



## 守护进程

inetd守护进程，仅仅搭建了一个框架。其具体业务的执行，将全部调用动态库提供的接口完成。守护进程的流程如下：

1. 两次fork操作，成为守护进程；
2. 调用hibus_connect_via_unix_socket()，建立与HiBus服务器的连接；
3. 读取配置文件，获得各项参数；
4. 根据配置文件，装载动态库。调用动态库的wifi_ops_get()函数，获取HibusInvokeOps结构；
5. 调用hibus_register_event()，注册远程过程调用；
6. 调用hibus_register_event()，注册可被订阅事件；
7. 调用动态库的open_device()函数，初始化WIFI设备，连接默认网络，并获得设备的文件描述符（如果有）；
8. 创建定时器；
9. 循环调用select，监视定时器、hiBus socket、设备文件描述符：
   1. 定时器：根据配置文件，定时调用动态库的wifi_scan()、wifi_signal()函数；
   2. hiBus socket：调用hibus_wait_and_dispatch_packet()，处理hiBus传输过来的事件、远程过程调用；
   3. 设备文件描述符（如果有）：调用动态库的device_read()函数，对读数据进行处理；
10. 调用hibus_revoke_event()撤销事件（用不到）、调用hibus_revoke_procedure()撤销远程过程调用（用不到）；
11. 调用hibus_disconnect()中断与HiBus服务器的连接（用不到）；
12. 调用dlclose()，关闭动态库。



## 动态库接口

#### 守护进程调用接口

##### 函数集的获得

在头文件 inetd.h 中，有如下声明：

```
typedef char * (* hibus_method_handler)(hibus_conn* conn, const char* from_endpoint, const char* to_method, 
                                        const char* method_param, int *err_code);

typedef struct _HibusInvokeOps
{
	int (* open_device) (int * fd_device);
    int (* close_device) (int fd_device);
    void (* device_read) (void);
    void (* wifi_scan) (void);
    void (* wifi_signal) (void);
    hibus_method_handler wifi_scan_handler;
    hibus_method_handler wifi_connect_handler;
    hibus_method_handler wifi_disconnect_handler;
} HibusInvokeOps;

```

动态库需要实现HibusInvokeOps结构中的全部函数。这些函数将被守护进程直接调用，或者被其他端点（endpoint）通过hiBus，远程调用。

守护进程首先调用如下函数，将hibus_conn结构指针传给动态库，并获得工作过程中所需要的全部函数指针。

```
HibusInvokeOps * wifi_ops_get(const hibus_conn* conn);
参数：
	hibus_conn* conn[in]：hibus_conn结构指针；
返回值：
	HibusInvokeOps结构指针。	
```

守护进程已经和hiBus建立了连接，所以hibus_conn* conn的值需要动态库保存，用于和hiBus通信。

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

如果在open_device()函数中返回的fd_device是一个合法的文件描述符（不等于 -1），则守护进程将通过select系统调用，监视该文件描述符。一旦该端口有数据，则调用动态库的device_read()函数。动态库需对该文件描述符进行读操作，并处理数据。然后根据数据，发送事件到HiBus总线。

如果在open_device()函数中返回的fd_device为-1，则不会调用该函数。

##### WIFI扫描

```
void wifi_scan(void);
参数：
	无
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



#### 远程过程调用接口

所有远程过程调用，都具有如下形式：

```
typedef char* (*hibus_method_handler)(hibus_conn* conn, const char* from_endpoint, const char* to_method,
                const char* method_param, int *err_code);
参数：
	hibus_conn* conn[in]：hibus_conn结构指针；
	const char* from_endpoint[in]：远程过程调用端点；
	const char* to_method[in]：调用的方法名称；
	const char* method_param[in]：调用方法的参数；
	int *err_code[out]：方法执行的错误编码；
返回值：
	char *:方法的执行结果
```

在WIFI操作中，强制规定，远程过程调用的参数，以及执行结果，使用JSON格式字符串。下面仅对每个过程调用的参数及返回值，进行说明。



##### wifi_scan_handler

```
参数：
	const char* method_param：NULL
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

​		status：为1时，表示当前连接，为0时，表示未连接。



##### wifi_connect_handler

```
参数：
	{
		"SSID":"fmsoft-dev",
		"encryption":"hybridos-hibus",
		"type":"WPA2",
		"autoconn":1,
		"default":1		
	}
返回值：
	{
		"status":"OK",		
	}
err_code:
	1001：密码错误
	1002：网络信号弱
```

其中:

​		autoconn：为1时，表示网络中断自动连接；为0时，表示提示用户，不自动重连；

​		default：为1时，表示默认网络。下次开机时自动连接，动态库需保存该设置到存储器中。



##### wifi_disconnect_handler

```
参数：
	{
		"SSID":"fmsoft-dev",
		"autoconn":false
	}
返回值：
	{
		"status":"OK",		
	}
err_code:
	1101：
	1102：
```

autoconn可以没有，默认值为false。



#### 可订阅的消息

调用下面的HiBus API，发送一个事件：

```
int hibus_fire_event (hibus_conn* conn, const char* bubble_name, const char* bubble_data);
参数：
	hibus_conn* conn：hibus_conn结构指针；
	const char* bubble_name：事件名称；
	const char* bubble_data：事件参数
返回值：
	error code。
```

在WIFI操作中，强制规定，发送事件的参数，使用JSON格式字符串。下面仅对各个可订阅事件，进行说明。

##### wifi_event_scan

```
名称：wifi_event_scan
参数：
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

其参数与远程过程调用wifi_scan_handler()的返回值相同。



##### wifi_event_signal

```
名称：wifi_event_signal
参数：
	{
		"SSID":"fmsoft-dev",
		"signal":65，
		"MAC":"AB:CD:EF:12:34:56",
		"IP":"192.168.1.128",
		"type":"WPA2",
		"frenquency":"5 GHz",
		"speed":"650 Mbps"
	}
```

只针对现有连接，发送该事件。



##### wifi_event_broken

```
名称：wifi_event_broken
参数：
	{
		"SSID":"fmsoft-dev",
		"reason":1001
	}
```

其中"reason"，数值与含义同远程过程调用wifi_connect_handler()的err_code。



##### wifi_event_connect

```
名称：wifi_event_connect
参数：
	{
		"SSID":"fmsoft-dev",
		"signal":65，
		"MAC":"AB:CD:EF:12:34:56",
		"IP":"192.168.1.128",
		"type":"WPA2",
		"frenquency":"5 GHz",
		"speed":"650 Mbps"
	}
```

无论是在何种情况下，只要与WIFI网络建立连接，即发送此事件。



##### wifi_event_resume

```
名称：wifi_event_resume
参数：
	{
		"SSID":"fmsoft-dev",
		"signal":65		
	}
```

