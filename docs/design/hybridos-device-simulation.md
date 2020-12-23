# 合璧操作系统设备模拟设计

薛淑明

本文阐述合璧操作系统设备模拟的设计。

- [基本框架及术语](#基本框架及术语)
- [各模块的设计](#各模块的设计)
   + [hiAceJs中hibus的js封装](#hiacejs中hibus的js封装)
   + [hibus-simulator](#hibus-simulator)
   + [hibus-wrapper](#hibus-wrapper)


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


### hibus-simulator

hibus-simulator.js 提供了hibus的接口的模拟实现，用户在开发过程中可以修改该js文件，以提供不同的
模拟数据。
将来，可以连接云服务提供更强大的功能模拟。

例如，hibus提供了一个printInfo函数，测在hibus-simulator.js 中模拟实现如下:

```js
export default {
    printInfo() {
        console.log('print Info from hibus-simulator');
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

    printInfo() {
        if (hibus) {
            hibus.printInfo();
        }
        else {
            hibusSimulator.printInfo();
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
        hibus.printInfo();
    }
}

```


