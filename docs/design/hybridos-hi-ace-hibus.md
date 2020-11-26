# hiACEJS中hiBus概要设计

耿岳

本文阐述了在hiACEJS中，hiBus功能的实现方式，及概要设计。

- [hiACEJS中hiBus软件结构](#hiACEJS中hiBus软件结构)
- [hiBus Interface](#hiBus Interface)
   + [hiBus_handler 类](#hiBus_handler类)
   + [代码举例](#代码举例)
- [hiBus connection](#hiBus connection)


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
  - 向hiBus 总线发送事件；
  - 做为事件订阅者，向hiBus 总线订阅事件，并设置事件处理的回调函数；
  - 作为过程处理器，设置过程调用函数；
  - 作为过程调用者，向hiBus 总线发送过程调用请求；
  - 读hiBus总线数据，解析并调用可能的事件处理函数，以及过程调用函数。
- hiBus connection，是对JS引擎的扩展，为JS提供了到hiBus的连接与读写的具体实现。该部分不参与任何业务处理。只处理raw data。



## hiBus Interface

### hiBus_handler类

hiBus Interface由hiBus_handler对象实现。hiBus_handler类如下：

```js
class hiBus_handler {
    var hiBusConnection;    // hiBus object for connection to hiBus
    var appName;            // app name
    var runnerName;         // runner name
    
    constructor(app, runner) { ...... }                 // constructor 
    fireEvent(event, data) { ...... }                   // send evnet to hiBus
    subscribeEvent(app, runner, event, func) { ...... } // subscribe event to hiBus
    revokeEvent(app, runner, event) { ...... }          // revoke event to hiBus
    registerProcedure(method, func) { ......}           // register Procedure to hiBus
    revokeProcedure(method) { ...... }	                // revoke Procedure to hiBus
    callProcedure(app, runner, method, param, timeout) { ...... }        // call a remote procedure, synchronously
    callProcedureAndWait(app, runner, method, param, timeout) { ...... } // call a remote procedure, asynchronously
    hibusHandler() { ...... }   			             // read hiBus, and handle each data package 
}
```



hiBus_handler类的构造函数，在该函数中，创建hiBusConnection对象。

```js
constructor(app, runner) ;
参数：
    app：应用名称；
    runner：行者名称；
返回值：
    无。如果hiBus连接成功，则hiBusConnection为hiBus对象，否则hiBusConnection为undefined。
```



产生一个事件，并发到hiBus总线上。

```js
fireEvent(name, data);
参数：
    name：事件名称；
    data：事件数据（Json格式）；
返回值：
    无。
```



订阅一个事件，并发到hiBus总线

```js
subscribeEvent(app, runner, event, func);
参数：
    app：产生事件的endpoint应用名；
    runner：产生事件的endpoint行者名；
    event：订阅事件的事件名；
    func：订阅事件的回调函数；
返回值：
    无。
```



取消订阅一个事件，并发到hiBus总线

```js
revokeEvent(app, runner, event);
参数：
    app：产生事件的endpoint应用名；
    runner：产生事件的endpoint行者名；
    event：订阅事件的事件名；
返回值：
    无。
```



向hiBus注册过程

```js
registerProcedure(method, func) ;
参数：
    method：method_name；
    func：过程调用的回调函数；
返回值：
    无。最终向hiBus发送Json数据时，填入hiBus_handler类的appName及runnerName属性。
```



向hiBus发送撤销过程

```js
revokeProcedure(method);
参数：
    method：method_name；
返回值：
    无。
```



向hiBus发送异步远程过程调用请求

```js
callProcedure(app, runner, method, param, timeout);
参数：
    app：远程调用的endpoint应用名；
    runner：远程调用的endpoint行者名；
    method：method_name；
    param：远程过程参数（Json格式）；
    timeout：超时时间，单位毫秒；
返回值：
    无。
```



向hiBus发送同步远程过程调用请求

```js
callProcedureAndWait(app, runner, method, param, timeout) ;
参数：
    app：远程调用的endpoint应用名；
    runner：远程调用的endpoint行者名；
    method：method_name；
    param：远程过程参数（Json格式）；
    timeout：超时时间，单位毫秒；
返回值：
    无。
```



hiBus读入数据处理

```js
hibusHandler();
参数：
	无
返回值：
	无
```



用户回调函数，都具有如下的形式：

```js
callbackFunction(msgData)	{ ........ }
```

参数msgData为一个Json对象。对其的解析由用户回调函数完成，解析内容参见hiBus通信协议。

### 代码举例

用户在JS代码中，使用该接口，有类似如下代码：

```js
// 创建hiBus_handler对象
var hiBus = new hiBus_handler("appagent", "setting");

// 订阅电池监控应用的“battery-quantity”消息
hiBus.subscribeEvent("battery-daemon", "battery-runner", "battery-quantity", display(msgData) { ...... });

// 读取hiBus总线，解析数据，如果“battery-quantity”消息存在，则执行display(msgData) {}
hiBus.hibusHandler();
```



## hiBus connection

hiBus connection是对JS引擎的扩展，为JS提供了到hiBus的连接与读写的具体实现。

该部分不参与任何业务处理。换句话说，该部分代码读写接口，只处理raw data，不负责任何打包和解析。

该部分代码为JS函数对象的c实现。因此返回值的理解不是c或者c++代码，需要以JS函数来理解。

hiBusConnection类包含三个JS函数的实现，说明如下：



hiBusConnection类的构造函数，创建到hiBus总线的连接

```c
bool hiBusConnection(char * app_ame, char * runner_name);
参数：
    app_name：创建该对象的应用名；
    runner_name：创建该对象的行者名；
返回值：
    成功返回true，失败返回false。
```



hiBus总线的读操作

```c
int hiBusReadRaw(char * buffer, int length);
参数：
    buffer：读缓冲区；
    length：读缓冲区长度；
返回值：
    实际读到的字节数。如果读操作失败，返回-1。
```



hiBus总线的写操作

```c
int hiBusWriteRaw(char * buffer, int length);
参数：
    buffer：写缓冲区；
    length：写缓冲区长度；
返回值：
    实际写入hiBus总线的字节数。如果写操作失败，返回-1。
```



