# 合璧操作系统设备端数据总线概要设计

魏永明

本文阐述合璧操作系统设备端数据总线（hiBus）的设计。

- [基本框架](#基本框架)
- [协议及接口](#协议及接口)
   + [套接字接口](#套接字接口)
   + [协议](#协议)
      * [过程调用请求](#过程调用请求)
      * [过程调用结果](#过程调用结果)
      * [转发过程调用请求给过程处理器](#转发过程调用请求给过程处理器)
      * [产生事件](#产生事件)
      * [收到事件](#收到事件)
      * [中继](#中继)
   + [hiBus 内置过程](#hibus-内置过程)
      * [注册过程](#注册过程)
      * [撤销过程](#撤销过程)
      * [注册事件](#注册事件)
      * [撤销事件](#撤销事件)
      * [订阅事件](#订阅事件)
      * [取消事件订阅](#取消事件订阅)
      * [列出已注册过程](#列出已注册过程)
      * [列出已注册事件](#列出已注册事件)
      * [列出事件的订阅者](#列出事件的订阅者)
- [hiBus 服务器的架构及关键模块](#hibus-服务器的架构及关键模块)
   + [架构](#架构)
   + [关键模块](#关键模块)
   + [调试工具](#调试工具)
   + [客户端接口](#客户端接口)
- [安全性](#安全性)
- [演进](#演进)

## 基本框架

下图给出了 HybridOS 设备侧的系统架构图：

```
 ---------------------------------------------------------------------------------
| DockerBar, StatusBar,   |                   |           | Input Method Window,  |
| Screen Lock, Launcher,  | App Main Windows  |  N/A      | System Alert Window,  |        - main windows
| Notification...         |                   |           | ...                   |
 ---------------------------------------------------------------------------------
|      System Manager     |     App Agent     | Wallpaper | mginit w/ compositor  |____    - processes
 ---------------------------------------------------------------------------------     |
|                   hiWebKit                  |                                   |    |
 ---------------------------------------------                                    |    |
|  MiniGUI, hiCairo, hiMesa, SQLite, FreeType, HarfBuzz, LibPNG, LibJPEG, ...     |    | hiBus
|                                                                                 |    |
 ---------------------------------------------------------------------------------     |
|                           HybridOS servers and user daemons                     |____|
 ---------------------------------------------------------------------------------
|                          Python runtime environment (optional)                  |
|                             LibZ, CURL, LibGcrypt, ...                          |
|                               C/C++ runtime environment                         |
 ---------------------------------------------------------------------------------
|                               Linux Kernel/Drivers                              |
 ---------------------------------------------------------------------------------
```

在 HybridOS 中，始终贯彻着一个重要的设计思想：数据驱动。而不论是单一应用场景还是多应用场景，hiBus 将成为 HybridOS 连接 App 和底层功能模块的纽带；甚至在将来，成为连接局域网内不同设备节点的纽带。

hiBus 的一些思想来自于 OpenWRT 的 uBus，比如通过 JSON 格式传递数据。但相比 uBus，hiBus 具有如下重要的改进：

1. 提供两种类型的底层连接通道：本地的 Unix Domain Socket 和 Web Socket，以方便不同模块、节点之间可以通过 Web Socket 连接到 hiBus 上。
1. 提供基本的安全机制，以决定某个应用或者某个远程节点能否订阅（subscribe）特定的事件，以及能否调用（call）某个特定的过程（procedure）。
1. 考虑到在未来，hiBus 可通过 Web Socket 向局域网中的其他 IoT 设备节点提供服务。因此，需要在事件订阅以及调用远程过程时使用包含主机或设备名称的统一资源标识符（URI）。
1. 分离事件和远程过程调用端口，使用不同的套接字端口，以解决模块同时提供事件和远程过程调用服务时出现的问题。

我们保留如下特别的主机名称、应用名称：

- `localhost`：指本机。（概念有误，移除——通过 Unix Domain Socket 或本机 IP 地址（127.0.0.1）连接到 hiBus 的本机模块。）
- `cn.fmsoft.hybridos.hibus`：保留的应用名称，指 hiBus 本身，注册或注销事件或过程时，向该应用发送指定的过程调用。该应用也可提供一些一般性的系统操作命令以及事件。

不同的软件模块可扮演不同的角色或同时扮演多个角色：

1. 事件发生器（event generator）：通过套接字接口连接到 hiBus 服务器，然后向服务器注册应用名称以及事件名称。
1. 过程处理器（procedure handler）：通过套接字接口连接到 hiBus 服务器，然后向服务器注册应用名称以及方法名称。
1. 事件订阅者（event subscriber）：通过套接字接口连接到 hiBus 服务器，然后向服务器订阅指定的事件，之后将收到该事件发生器上新发生的事件。
1. 过程调用者（procedure caller）：通过套接字接口连接到 hiBus 服务器，然后向服务器发送过程调用请求，之后同步等待或者异步等待处理结果。

## 协议及接口

### 套接字接口

1. Unix Doman Socket 端口：
   - `/var/run/hibus.event`：事件端口，用于发送或接收事件。
   - `/var/run/hibus.rpc`：过程调用端口，用于发起过程调用。
1. Web Socket 端口（HybridOS 保留 7000 ~ 7999 端口）：
   - `7700`：事件端口，用于发送或接收事件。
   - `7701`：过程调用端口，用于发起远程过程调用。
   - `7702`：中继端口，用于不同主机服务器之间的请求中继。

未来，当 hiBus 服务器运行在 Web Socket 端口上时，有能力为其他设备节点提供事件及过程调用服务。

### 协议

#### 过程调用请求

发起执行特定过程（procedure）的请求时，向 hiBus 的过程调用端口写入如下 JSON 数据：

```json
{
    "packetType": "call",
    "requestId": "<hased_request_identifier>",
    "procedure": "<host_name>/<app_name>/<procedure_name>",
    "expectedTime": 30000,
    "authenInfo": {
        ...
    },
    "parameter": {
        ...
    },
}
```

其中，
- `packetType` 表示数据包类型，这里用 `call`，表示过程调用。
- `requestId` 是调用方提供的一个唯一性字符串，用于在调用方标识特定的调用请求。
- `procedure` 是过程名称，包括主机名、应用名称和方法名称。
- `expectedTime` 是期望的执行时间，毫秒为单位，为 0 表示不限。
- `authenInfo` 是可选的身份验证信息（如果该过程需要额外的身份验证的话）。
- `parameter` 是该过程的执行参数。

#### 过程调用结果

hiBus 服务器会首先将过程调用请求转发给过程处理器，根据过程处理器的处理结果返回结果标识符、请求标识符、状态码以及可能的结果给调用者。相应的数据包格式如下：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "fromHost": "<host_name_processed_this_call>",
    "fromApp": "<app_name_processed_this_call>",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `result`，表示过程调用结果。
- `resultId` 是 hiBus 服务器为每个过程调用分配的一个全局唯一字符串，用于跟踪特定的调用。
- `requestId` 是调用方发起过程调用时提供的标识符。
- `fromHost` 表示处理该调用的主机名称。
- `fromApp` 表示处理该调用的应用名称。
- `timeDiff` 是自收到原始请求到返回该结果的时间差；单位秒，浮点数。
- `retCode` 取 HTTP 状态码子集，可取如下值：
   - 200 Ok：表示过程正常执行并返回了结果。
   - 202 Accepted：表示已提交参数给过程，但该过程可能耗时较长，所以暂时没有返回结果。
   - 400 Bad Request：表示 JSON 格式解析错误。
   - 401 Unauthorized：表示该过程需要验证用户身份，但未提供。
   - 403 Forbidden：表示没有权限调用该过程。
   - 404 Not Found：表示请求的过程无效。
   - 405 Method Not Allowed：表示请求参数不被允许。
   - 406 Not Acceptable：表示请求参数不被接受。
   - 409 Confilct：表示冲突；比如重复注册过程或事件时。
   - 423 Locked：表示被锁定；比如要撤销的过程尚未结束某个执行。
   - 500 Internal Server Error：表示服务器内部错误。
   - 501 Not Implemented：表示未实现。
   - 502 Bad Gateway：表示执行该调用的过程发生异常。
   - 503 Service Unavailable：表示服务不可用。
   - 504 Gateway Timeout：表示执行该调用的过程超时。
   - 507 Insufficient Storage：表示遇到内存或存储不足的问题。
- `retValue` 包含过程调用的返回值。只有 `retCode` 为 200 时，返回的数据中才包含有结果数据。

在正常情况下，调用者会首先收到 202 状态码，然后在未来的某个时间点收到 200 状态码。只有状态码为 200 时，才表示过程执行完毕并包含有结果信息。

通常，当 `retCode` 为 202 时，调用方应根据 `resultId` 来监视后续从服务器返回的数据包，从而获得最终的执行状态或结果。

注意，过程处理器返回给服务器的结果数据包格式，和上述数据包类似，但缺少 `fromHost`、`fromApp` 以及 `timeDiff` 等字段：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "retCode": 200,
    "result": {
        ...
    }
}
```

#### 转发过程调用请求给过程处理器

hiBus 服务器收到执行特定过程的请求后，首先做如下检查：

- 解析调用请求；存在无效数据时，比如无效的过程名称，未提供参数，或者参数不符合要求等，返回 400、405 或者 406 状态码。
- 指定的过程是否存在；若不存在，立即返回 404。
- 调用者是否拥有执行该过程的权限；若没有权限或者未提供要求的身份验证信息，则分别返回 403、401 状态码。

如果以上检查均无问题，则转发过程调用请求到过程所在的应用，然后向调用者返回 202 状态码。

转发过程调用请求时，服务器会在调用请求数据包中增加由服务器生成的用于追踪该调用的全局唯一性标识符 `resultId`。如下所示：

```json
{
    "packetType": "call",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "timeDiff": 0.5432,
    "methodName": "<method_name>",
    "authenInfo": {
        ...
    },
    "parameter": {
        ...
    },
}
```

过程处理器返回给服务器的结果数据包同 `过程调用结果` 中的描述，然后由服务器转发给调用者。

一般而言，为防止出现过程处理器的重入情形，服务器应保证在一个过程调用连接上（对应于单个应用），一次只处理一个过程，故而需要使用队列来维护多个过程调用请求。
但调用者可发起多个请求，然后根据 `resultId` 和 `requestId` 来跟踪这些请求返回的结果数据。

#### 产生事件

当事件产生器产生一个新的事件后，向事件端口写入如下格式的 JSON 数据。产生的事件数据包通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<hased_event_identifier>",
    "eventName": "<event_name>",
    "forHost": "<the_host_receiving_this_packet>",
    "forApp": "<the_app_receiving_this_packet>",
    "payload": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId`：事件发生器为每个事件分配的一个全局唯一字符串。
- `eventName`：事件名称。
- `forHost`：可以接收该事件的主机名称，可指定多个主机（用逗号分隔），可包含通配符。
- `forApp` 是可以接收该事件的应用名称，可指定多个主机（用逗号分隔），可包含通配符。
- `payload` 包含真正的事件数据。

#### 收到事件

订阅事件之后，从事件端口读入数据即可获得已订阅的事件数据。收到的事件数据通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<hased_event_identifier>",
    "eventName": "<event_name>",
    "fromHost": "<the_host_generating_this_event>",
    "fromApp": "<the_app_generating_this_packet>",
    "timeDiff": 0.1234,
    "payload": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId` 是事件发生器为每个事件分配的一个全局唯一字符串。
- `eventName` 是事件名称。
- `fromHost` 表示发生该事件的主机名称。
- `fromApp` 表示发生该事件的应用名称。
- `timeDiff` 表示服务器收到该事件到转发该事件的时间差；单位秒，浮点数。
- `payload` 包含真正的事件数据。

#### 中继

注：此功能当前不做实现。

通过 Web Socket 处理连接时，可在不同 hiBus 服务器实例之间建立中继服务，从而实现跨主机的远程过程调用。

当过程调用的目标主机非本机时，hiBus 服务器可将该请求转发目标主机所在的 hiBus 服务器，然后将结果转发给调用者。

此时，最终收到调用请求的服务器将扮演调用者角色向目标服务器发起调用请求。

### hiBus 内置过程

hiBus 服务器通过内置过程实现注册过程/事件等功能。

#### 注册过程

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/registerProcedure`
- 参数：
   + `methodName`：待注册过程的方法名称；由于主机名和应用名是隐含的，所以无需指定。
   + `forHost`：可以调用该方法的主机名称，可指定多个主机（逗号分隔），亦可使用通配符。
   + `forApp`：可以调用该方法的应用名称，可指定多个主机（逗号分隔），亦可使用通配符。
- 返回值：无。客户端依据结果的 `retCode` 判断是否注册成功：
   + `409`：表示重复的过程名称/方法名称。
   + `200`：表示成功。

注册过程的数据包：

```json
{
    "packetType": "call",
    "requestId": "<hased_request_identifier>",
    "procedure": "localhost/cn.fmsoft.hybridos.hibus/registerProcedure",
    "expectedTime": 30000,
    "authenInfo": null,
    "parameter": {
        "methodName": "foo",
        "forHost": "localhost",
        "forApp": "*",
    },
}
```

成功情形下的结果数据包：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": null,
}
```

#### 撤销过程

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/revokeProcedure`
- 参数：
   + `methodName`：待撤销过程的方法名称；由于主机名和应用名是隐含的，所以无需指定。
- 返回值：无。客户端依据结果的 `retCode` 判断是否撤销成功，可能的值有：
   + `404`：表示未找到指定的过程。
   + `403`：表示待撤销的过程不是由调用方注册的，所以不能撤销。
   + `423`：表示有调用者正在等待该过程返回结果，不能撤销。
   + `200`：表示成功。

#### 注册事件

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/registerEvent`
- 参数：
   + `eventName`：待注册的事件名称；由于主机名和应用名是隐含的，所以无需指定。
   + `forHost`：可以订阅该事件的主机名称，可指定多个主机（逗号分隔），亦可使用通配符。
   + `forApp`：可以订阅该事件的应用名称，可指定多个主机（逗号分隔），亦可使用通配符。
- 返回值：无。客户端依据结果的 `retCode` 判断是否注册成功，可能的值有：
   + `409`：表示重复的事件名称/方法名称。
   + `200`：表示成功。

#### 撤销事件

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/revokeEvent`
- 参数：
   + `eventName`：待撤销过程的事件名称；由于主机名和应用名是隐含的，所以无需指定。
- 返回值：无。客户端依据结果的 `retCode` 判断是否撤销成功，可能的值有：
   + `404`：表示未找到指定的事件。
   + `403`：表示待撤销的事件不是由调用方注册的，所以不能撤销。
   + `200`：表示成功。

#### 订阅事件

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/subscribeEvent`
- 参数：
   + `event`：要订阅的完整事件名称，含主机名、应用名以及事件名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否订阅成功，可能的值有：
   + `404`：表示未找到指定的事件。
   + `403`：表示事件发生器不允许调用方订阅该事件。
   + `200`：表示成功。

#### 取消事件订阅

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/unsubscribeEvent`
- 参数：
   + `event`：要取消订阅的完整事件名称，含主机名、应用名以及事件名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否取消成功，可能的值有：
   + `404`：表示调用方未订阅指定的事件；或者该事件已经被撤销。
   + `200`：表示成功。

#### 列出已注册过程

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/listProcedures`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可调用的过程清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "localhost/cn.fmsoft.hybridos.networkManager/getHotSpots",
        "localhost/cn.fmsoft.hybridos.networkManager/connectToHotSpot",
    ],
}
```

#### 列出已注册事件

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/listEvents`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可订阅的事件清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "localhost/cn.fmsoft.hybridos.networkManager/getHotSpots",
        "localhost/cn.fmsoft.hybridos.networkManager/connectToHotSpot",
    ],
}
```

#### 列出事件的订阅者

- 过程名称：`localhost/cn.fmsoft.hybridos.hibus/listEventSubscribers`
- 参数：
   + `eventName`：事件名称，含完整的主机名即应用名。
- 返回值：成功时返回指定事件的订阅者的主机名及应用名清单。
- 常见错误状态码：
   + `404`：表示未找到指定的事件。
   + `403`：表示调用方无权查询指定事件的订阅者。
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "localhost/cn.fmsoft.hybridos.foo",
        "localhost/cn.fmsoft.hybridos.bar",
    ],
}
```

## hiBus 服务器的架构及关键模块

### 架构

### 关键模块

### 调试工具

### 客户端接口

## 安全性

## 演进

