# hiBus在DevEco Studio环境中使用方案设计


本文阐述hiBus在DevEco Studio环境中使用方案设计。

- [基本框架及术语](#基本框架及术语)
- [各模块的设计](#各模块的设计)
   + [hiAceJs中hibus的js封装](#hiacejs中hibus的js封装)
   + [hibus-simulator.js](#hibus-simulator.js)
   + [hibus-wrapper.js](#hibus-wrapper.js)


## 基本框架及术语

下图给出了 hiBus 在DevEco中开发和实际设备运行的架构图：

```
 ---------------------------------------------------------------------------------
|            App          |        App        |        App        |    ...        |
 ---------------------------------------------------------------------------------
|                              hibus-wrapper.js                                   |
 ---------------------------------------------------------------------------------
|           import  @system.hibus       |           hibus-simulator.js            |
 ---------------------------------------------------------------------------------
|                   hiAceJs             |                                         |
 ---------------------------------------              simulator data
|                   hiBus               |                                         |
 ---------------------------------------------------------------------------------
|                   Device              |             DevEco Studio               |
 ---------------------------------------------------------------------------------
```

为了在 DevEco 环境使用hibus，设计了 hibus-wrapper.js, 该js是对hibus接口的包装，在执行的
时候，它会据据运行的环境选择是执行真正的hibus调用或者执行 hibus-simulator.js 里的模拟接
口。

## 各模块的设计

### hiAceJs中hibus的js封装

在hiAceJs中，将hibus的相关接口，封装成js对象，注入JS 引擎(JerryScript)，以供应用层使用。

例如，注入一个printInfo函数供上层使用:

```C++
// nativeapi_hibus.h
class NativeapiHiBus {
public:
    NativeapiHiBus() {}
    ~NativeapiHiBus() {}

    static JSIValue printInfo(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum);
};


// nativeapi_hibus.cpp

void InitHiBusModule(JSIValue exports)
{
    JSI::SetModuleAPI(exports, "printInfo", NativeapiHiBus::printInfo);
}

JSIValue NativeapiHiBus::printInfo(const JSIValue thisVal, const JSIValue* args, uint8_t argsNum)
{
    fprintf(stderr, "nativeapi_hibus printInfo isCalled\n");
    return JSI::CreateUndefined();
}

```


### hibus-simulator.js

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

### hibus-wrapper.js

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


