# 合璧操作系统设备模拟设计

【主题】合璧操作系统设备模拟设计  
【概要】本文阐述合璧操作系统设备模拟的设计  
【版本】1.0  
【作者】薛淑明  
【日期】2020 年 12 月  
【状态】草稿  

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

本文涉及到的飞漫软件或其合作伙伴的注册商标或商标，有关其详细列表，请查阅文档末尾。

**目录**

- [基本框架及术语](#基本框架及术语)
- [各模块的设计](#各模块的设计)
   + [hiAceJs中hibus的js封装](#hiacejs中hibus的js封装)
   + [local-simulator](#local_simulator)
   + [hibus-simulator](#hibus_simulator)
   + [hibus-wrapper](#hibus_wrapper)
- [设备操控](#设备操控)
   + [WiFi](#wifi)
   + [Battery](#battery)
- [附：商标声明](#附商标声明)


## 基本框架及术语

下图给出了基于hiBus的设备访问的架构图：

```
 --------------------------------------------------------------------------------
|        App        |        App        |        App        |      ...           |
 --------------------------------------------------------------------------------
|           wifi.js         |        battery.js       |            ...           |
 --------------------------------------------------------------------------------
|                                hibus-wrapper.js                                |
 --------------------------------------------------------------------------------
|                hiAceJs                |           hibus-simulator.js           |
 --------------------------------------------------------------------------------
|                 hiBus                 |                                        |
 ---------------------------------------|         local-simulator.json           |
|    WiFi Device   |   Battery Device   |                                        |
 -------------------------------------------------------------------------------
```

在 HybridOS 中，App 通过 hiBus 来访问设备。在上面的架构图中，wifi.js 和 battery.js 分别
是针对WiFi设备和电池设备的操作封装。 

hibus-wrapper.js 是 hiBus 基本功能的包装，包括连接的建立、断开，过程调用，事件订阅等内容。
在真实设备上运行时，hibus-wrapper.js 会通过由 hiAceJs 提供的 system.hibus 来访问 hiBus总线，
进而操作真实的设备。在开发环境（包括模拟器）运行时，hibus-wrapper.js 会调用 hibus-simulator.js 
的相关接口，以便返回模拟数据。

hibus-simulator.js 目前主要通过 local-simulator.json 返回模拟数据。

## 各模块的设计

### hiAceJs中hibus的js封装

在hiAceJs中，将hiBus的相关接口，封装成js对象(system.hibus)，注入JS引擎(JerryScript)，以供应用层使用。


```c++
// nativeapi_hibus.h
class NativeapiHiBus {
public:
    NativeapiHiBus() {}
    ~NativeapiHiBus() {}

    // appName property
    static void RegistPropertyAppName();
    static JSIValue GetterAppName(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
    static JSIValue SetterAppName(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);

    // runnerName property
    static void RegistPropertyRunnerName();
    static JSIValue GetterRunnerName(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
    static JSIValue SetterRunnerName(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);

    // pathSocket property
    static void RegistPropertyPathSocket();
    static JSIValue GetterPathSocket(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);

    // status property
    static void RegistPropertyStatus();

    static void RegistProperty();

    // connect function
    static JSIValue Connect(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
    // disconnect function
    static JSIValue Disconnect(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
    // send function
    static JSIValue Send(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
    // read function
    static JSIValue Read(const JSIValue thisVal, const JSIValue *args, uint8_t argsNum);
};


// nativeapi_hibus.cpp

void InitHiBusModule(JSIValue exports)
{
    RegistProperty();
    JSI::SetModuleAPI(exports, "connect", NativeapiHiBus::connect);
    JSI::SetModuleAPI(exports, "disconnect", NativeapiHiBus::disconnect);
    JSI::SetModuleAPI(exports, "send", NativeapiHiBus::send);
    JSI::SetModuleAPI(exports, "read", NativeapiHiBus::read);
}

void NativeapiHiBus::RegistProperty()
{
    RegistPropertyAppName();
    RegistPropertyRunnerName();
    RegistPropertyPathSocket();
    RegistPropertyStatus();
}

JSIValue NativeapiHiBus::Connect(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum)
{
    ...
}

JSIValue NativeapiHiBus::Disconnect(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum)
{
    ...
}

JSIValue NativeapiHiBus::Send(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum)
{
    ...
}

JSIValue NativeapiHiBus::Read(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum)
{
    ...
}


```

### local-simulator

local-simulator.json 提供了本地的模拟数据，用户可以修改该文件，已提供模拟数据

```json
{
    "procedure" : [
        {
            "name":"procedureName1",
            "data": {
                ...
            }
        },
        {
            "name":"procedureName2",
            "data": {
                ...
            }
        }
    ],

    "event" : [
        {
            "name":"eventName1",
            "data": {
                ...
            }
        },
        {
            "name":"eventName1",
            "data": {
                ...
            }
        }
    ]
}
```

### hibus-simulator

hibus-simulator.js 提供了hibus的接口的模拟实现，用户在开发过程中可以修改local-simulator.json文件，
以提供不同的模拟数据。


```js
import localData from '../../common/local-simulator.json';

export default {
    connect() {
    }

    disconnect() {
    }

    subscribeEvent(appName, runnerName, eventName, funcName) {
    }

    unsubscribeEven(appName, runnerName, eventName, funcName) {
    }

    callProcedure(endpoint, methodName, param, timeout, callback) {
    }

    checkPackets(timeout) {
        // get data from localData
        // callback
    }
}
```

### hibus-wrapper

hibus-wrapper 是对 hibus 接口的一层包装，这层包装的目的是为了在DevEoc 开发环境中，在不具备
hiAceJs的情况下，可以正常的进行开发的测试工作。

在真实设备中，hibus-wrapper 调用 hiAceJs 提供的 system.hibus 模块的相关接口，在DevEoc 开发时，
hibus-wrapper.js 调用 hibus-simulator.js提供的接口。


例如，hibus提供了一个printInfo过程，则在hibus-wrapper.js 中实现如下:

```js
import hibus from '@system.hibus';
import hibusSimulator from './hibus-simulator'

export default {

    connect() {
        if (hibus) {
            hibus.connect();
        }
        else {
            hibusSimulator.connect();
        }
    }

    disconnect() {
        if (hibus) {
            hibus.disconnect();
        }
        else {
            hibusSimulator.disconnect();
        }
    }

    subscribeEvent(appName, runnerName, eventName, funcName) {
        if (hibus) {
            hibus.send(...);
        }
        else {
            hibusSimulator.subscribeEvent(appName, runnerName, eventName, funcName);
        }
    }

    unsubscribeEven(appName, runnerName, eventName, funcName) {
        if (hibus) {
            hibus.send(...);
        }
        else {
            hibusSimulator.unsubscribeEven(appName, runnerName, eventName, funcName);
        }
    }

    callProcedure(endpoint, methodName, param, timeout, callback) {
        if (hibus) {
            hibus.send(...);
        }
        else {
            hibusSimulator.callProcedure(appName, runnerName, eventName, funcName);
        }
    }

    checkPackets(timeout) {
        if (hibus) {
            hibus.read(...);
            // callback
            ...
        }
        else {
            hibusSimulator.checkPackets(...);
        }
    }

}
```

在App编程使用时，比如按钮的click事件过程clickButton 如下:

```js
import device from '@system.device';
import hibus from '../../common/hibus-wrapper';

export default {
    clickButton() {
        hibus.connect();
    }
}

```

## 设备操控

### WiFi

底层WiFi相关服务提供一系列接口和事件用于对WiFi进行操控。

#### WiFi开启

该过程完成对WIFI设备的初始化。如根据配置文件，搜索并连接默认网络。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/open`
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "open",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```

#### WiFi关闭

该过程关闭WIFI当前连接，同时完成设备相关软、硬件的资源回收。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/close`
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "close",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```

#### 连接热点

该过程将根据参数连接指定的热点。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/connect`
- 参数：
   + `ssid`：网络SSID
   + `password`：网络的密码，如为NULL，表示不需要密码
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "connect",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```

#### 断开网络连接

该过程将断开网络连接。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/disconnect`
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "disconnect",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```

#### 获取WiFi信号强度 

该过程将断开网络连接。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/signalstrength`
- 返回值：成功时返回信号强度，取值获围0~100。
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "signalstrength",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": 70
}
```

#### 扫描WiFi热点

该过程将启动WiFi扫描，扫描结果需要通过过程`@localhost/cn.fmsoft.hybridos.settings/inetd/gethotspots`获取

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/startscan`
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "startscan",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```


#### 获取WiFi热点列表

该过程将获取WiFi扫描结果。

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/gethotspots`
- 返回值：成功时返回扫描到的WiFi热点列表
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "gethotspots",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": [
        {
            "bssid": "f0:b4:29:24:18:eb",
            "ssid": "dev001",
            "frequency": "2427MHZ",
            "signalStrength": 70,
            "capabilities": "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][WPS][ESS]",
        },
        {
            "bssid": "ea:a8:aa:24:bc:ae",
            "ssid": "dev002",
            "frequency": "2427MHZ",
            "signalStrength": 75,
            "capabilities": "[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][WPS][ESS]",
        },
    ],
}
```

#### 设置定时间隔

该过程用于设置定时间隔

- 过程名称：`@localhost/cn.fmsoft.hybridos.settings/inetd/setinterval`
- 参数：
   + `scan`：网络扫描的时间间隔。单位：秒
   + `password`：检测网络信号强度时间间隔。单位：秒
- 返回值：无。客户端依据结果的 `retCode` 判断是否成功：
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromMethod": "setinterval",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
}
```

#### WiFi信号改变事件

当前WiFi信号强度改变时，产生 `SIGNALSTRENGTHCHANGED` 事件:

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/inetd",
    "fromBubble": "SIGNALSTRENGTHCHANGED",
    "bubbleData": {
        "signalStrength": 75
    }
}
```

### Battery

底层电池相关服务会在电池状态变化(充电状态，电量等)将产生电池状态改变事件,
App可以通过订阅该事件，获取电池状态信息。

#### 电池状态改变事件

当电池状态发生改变时，产生 `BATTERYCHANGED` 事件:

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.settings/powerd",
    "fromBubble": "BATTERYCHANGED",
    "bubbleData": {
        "plugged": [ true | false ],
        "health": "90%",
        "temperature": "55℃",
        "voltage": "4.7V",
        "level": 75
    }
}
```



## 附：商标声明

_注意：除本说明之外，原样复制，放到文档最后。_

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

