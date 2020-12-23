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
- [设备接口设计](#设备接口设计)
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
|                 hiBus                 |                      |     Remote      |
 ---------------------------------------| local-simulator.json |     Cloud       |
|    WiFi Device   |   Battery Device   |                      |     Service     |
 -------------------------------------------------------------------------------
```

在 HybridOS 中，App 通过 hiBus 来访问设备。在上面的架构图中，wifi.js 和 battery.js 分别
是针对WiFi设备和电池设备的操作封装。

hibus-wrapper.js 是 hiBus 基本功能的包装，包括连接的建立、断开，过程调用，事件订阅等内容。

在真实设备上运行时，hibus-wrapper.js 会通过由 hiAceJs 提供的 system.hibus 来访问 hiBus总线，
进而操作真实的设备。

在开发环境（包括模拟器）运行时，hibus-wrapper.js 会调用 hibus-simulator.js 的相关接口，以便
返回模拟数据。

hibus-simulator.js 目前主要通过 local-simulator.json 返回模拟数据；将来可以连接远程云服务
从而获取更佳的服务和体验。

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
以提供不同的 模拟数据。 将来可以连接远程云服务从而获取更佳的服务和体验。


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


例如，hibus提供了一个printInfo函数，则在hibus-wrapper.js 中实现如下:

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

在App编程使用时，比如按钮的click事件函数clickButton 如下:

```js
import device from '@system.device';
import hibus from '../../common/hibus-wrapper';

export default {
    clickButton() {
        hibus.connect();
    }
}

```

## 设备接口设计

### WiFi

### Battery

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

