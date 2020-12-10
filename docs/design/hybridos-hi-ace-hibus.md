# hiACEJS中hiBus概要设计

耿岳

本文阐述了在hiACEJS中，hiBus功能的实现方式，及概要设计。

- [hiACEJS中hiBus软件结构](#hiacejs中hibus软件结构)
- [hiBus Interface](#hibus-interface)
   + [HiBus类](#hibus类)
   + [代码举例](#代码举例)
- [HiBusConn类](#hibusconn类)


## hiACEJS中hiBus软件结构

在hiACEJS中，对hiBus的实现的软件结构如下图：

```c
 ------------------------------------------------------------
|              User JS code, hiBus Interface(JS)             |
 ------------------------------------------------------------
|         JS engine (jerryscript), hiBus connection(c)       |
 ------------------------------------------------------------ 
|      HybridOS servers and user daemons (hiBus daemon)      |
 ------------------------------------------------------------
|          Python runtime environment (optional)             |
|                LibZ, CURL, LibGcrypt, ...                  |
|                C/C++ runtime environment                   |
 ------------------------------------------------------------
|                  Linux Kernel/Drivers                      |
 ------------------------------------------------------------
```

整个代码结构包含了三部分：

- 用户JS代码，调用hiBus Interface，完成hiBus总线所定义的全部功能；

- hiBus Interface，该代码为公共代码，为每个页面调用。为用户JS代码提供接口，包括：
  - 建立到hiBus 总线的连接；
  - 做为事件订阅者，向hiBus 总线订阅事件，并设置事件处理的回调方法；
  - 作为过程调用者，向hiBus 总线发送过程调用请求；
  - 读hiBus总线数据，解析并调用可能的事件处理方法，以及过程调用方法。
- hiBus connection，是对JS引擎的扩展，为JS提供了到hiBus的连接与读写的具体实现。该部分不参与任何业务处理。只处理raw data。



## hiBus Interface

### HiBus类

#### 构造函数

HiBus类的构造函数，通过该函数，创建HiBus对象。

```
HiBus(appName, runnerName[, pathSocket]);
参数：
    appName：应用名称；
    runnerName：行者名称；
    pathSocket：domain socket路径。如没有该参数，则使用默认路径“/var/run/hibus.sock”；
返回值：
    返回一个HiBus对象。
```



#### 常量

| Constant              | Value |
| --------------------- | ----- |
| HiBus.SYNCREMOTECALL  | 0     |
| HiBus.ASYNCREMOTECALL | 1     |



#### 属性

HiBus.hiBusConn：HiBusConn对象，用于建立到HiBus的连接，以及数据读写操作。



#### 方法

订阅一个事件，并发到hiBus总线

```
HiBus.subscribeEvent(appName, runnerName, eventName, funcName);
参数：
    appName：产生事件的应用名；
    runnerName：产生事件的行者名；
    eventName：订阅事件的事件名；
    funcName：订阅事件的回调方法，该方法由用户提供，参数为一个Json对象；
返回值：
    订阅成功返回true，失败返回false。
```



取消订阅一个事件，并发到hiBus总线

```
HiBus.unsubscribeEvent(appName, runnerName, eventName);
参数：
    appName：产生事件的应用名；
    runnerName：产生事件的行者名；
    eventName：订阅事件的事件名；
返回值：
    取消订阅，成功返回true，失败返回false。
```



向hiBus发送远程过程调用请求

```
HiBus.callProcedure(endpoint, methodName, param, timeout, callback);
参数：
    endpoint：远程调用的endpoint名；
    methodName：远程调用的方法名；
    param：远程过程参数（Json对象），如无需参数，该参数为一空对象{}；
    timeout:远程过程调用的超时值，单位毫秒。如果是0，表示异步请求，忽略callback.type，立刻返回；
    callback：用于处理远程过程调用结果的对象，有如下结构：
        {
            type:sync,                  // 请求类型：同步、异步
            success: funcSuccess,       // 回调成功后执行的回调方法，该方法由用户提供，参数为Json对象；
            error: funcError,           // 回调失败后执行的回调方法，该方法由用户提供，参数为请求的返回值；
        }
        如果该值为null，则表示该过程调用是个异步过程，且不关心返回值。即使从hiBus获得过程调用的返回值后，也不做任何处理。
返回值：
    无。
```



hiBus读入数据处理。在用户代码中，需定时、或按业务需要，调用该方法。该方法读取HiBus总线传递给该端点的数据，并完成相应处理工作。

```
HiBus.checkPackets(timeout);
参数：
	timeout：读hiBus超时值，如果是0，则读端口后立刻返回。
返回值：
	无
```

#### 用户回调方法

除了远程过程调用的错误回调方法外，用户提供的回调方法都具有如下的形式：

```
callbackFunction(dataJson)	{ 
    ........ 
}
```

参数dataJson为一个Json对象。对其的解析由用户回调方法完成，解析内容参见hiBus通信协议。

远程过程调用的错误回调方法，有如下形式：

```
funcError(retCode)	{ 
    ........ 
}
```

参数retCode为一个整数型返回值。


#### 代码举例

用户在JS代码中，使用该接口，有类似如下代码：

```
// 创建HiBus对象
var hiBus = new HiBus("cn.fmsoft.hybridos.appagent", "target0");

// 订阅浏览器第一个窗口中，URL发生变化的消息
hiBus.subscribeEvent("cn.fmsoft.hybridos.appagent", "target0", "URLCHANGE", display(msgData) { ...... });

// 读取hiBus总线，解析数据，如果收到"URLCHANGE"消息，则执行display(msgData) {}
hiBus.checkPackets();
```



## HiBusConn类

HiBusConn类是对JS引擎的扩展，为JS提供了到hiBus的连接与读写服务。该类的对象，作为HiBus类的一个属性，对用户不可见。

该部分代码为JS方法对象的c实现。其c接口视不同JS引擎而不同。下面是对其JS接口的说明。

#### 构造函数

HiBusConn类的构造函数，通过该函数，创建HiBusConn对象，同时建立对HiBus服务器的连接。

```
HiBusConn(appName, runnerName, pathSocket);
参数：
    appName：应用名称；
    runnerName：行者名称；
    pathSocket：domain socket路径。
返回值：
    返回一个HiBusConn对象。
```



#### 属性

HiBusConn.appName：建立连接端点的应用名；

HiBusConn.runnerName：建立连接端点的行者名；

HiBusConn.pathSocket：domain socket路径；

HiBusConn.status：当前连接状态。



#### 方法

建立到HiBus总线的连接

```
HiBusConn.connect()；
参数：
    无
返回值：
    连接成功返回true，连接失败返回false。
```



中断到HiBus总线的连接

```
HiBusConn.disconnect();
参数：
    无；
返回值：
    连接成功返回true，连接失败返回false。
```



发送数据包到HiBus总线

```
HiBusConn.send(packet, length);
参数：
    packet：字符串对象，为JSON结构的序列化。其具体结构，见《合璧操作系统设备端数据总线概要设计》及各软件详细设计；
    length：packet长度；
返回值：
    返回写入HiBus总线的数据长度，如是-1，表示写错误。
```



从HiBus总线读取一个数据包

```
HiBusConn.read(timeout);
参数：
    timeout：读取HiBus总线数据的超时值；
返回值：
    如果读取成功，则返回对象的类型为字符串类型；如果失败，返回对象的类型为undefined。
```



#### C接口

使用jerryscript引擎，c接口具有固定的格式，如下：

```
static jerry_value_t xxxxx_handler (const jerry_value_t this_obj,
                                    const jerry_value_t func_obj,
                                    const jerry_value_t args[],
                                    const jerry_length_t argc)
参数：
	this_obj：
	func_obj：HiBusConn构造函数对象；
	args：接口参数列表；
	argc：接口参数个数。
```

