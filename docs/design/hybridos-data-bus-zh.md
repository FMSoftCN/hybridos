# 合璧操作系统设备端数据总线概要设计

魏永明

本文阐述合璧操作系统设备端数据总线（hiBus）的设计。

- [基本框架及术语](#基本框架及术语)
- [协议及接口](#协议及接口)
   + [套接字接口](#套接字接口)
   + [协议](#协议)
      * [身份识别](#身份识别)
      * [过程调用请求](#过程调用请求)
      * [过程调用结果](#过程调用结果)
      * [转发过程调用请求给过程处理器](#转发过程调用请求给过程处理器)
      * [产生事件](#产生事件)
      * [收到事件](#收到事件)
      * [跨设备连接的思考](#跨设备连接的思考)
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
- [演进规划](#演进规划)

## 基本框架及术语

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
1. 考虑到在未来，hiBus 可通过 Web Socket 向局域网中的其他 IoT 设备节点提供服务。因此，需要在事件订阅以及调用远程过程时使用包含主机名称信息。
1. 重新设计的通讯协议。

术语：

1. 主机（host）：指合璧操作系统设备侧赖以运行的一个计算机主机。
1. 应用（app）：合璧操作系统中，一个应用由多个不同的模块组成，这些模块可能使用不同的编程语言开发，在系统中以 GUI 进程或者守护进程的形式运行。
1. 模块（module）：模块用来区分一个应用的不同组件。在合璧操作系统中，一个应用可以有多个模块，通常以独立的进程或者线程存在。比如专门用于处理底层设备控制的模块，专门用于处理人机交互的模块。
1. 事件（event）/泡泡（bubble）：事件名称中包含了主机名称、应用名称、模块名称以及泡泡（即事件发生器）名称。
1. 过程（procedure）/方法（method）：过程名称中包含了主机名称、应用名称、模块名称以及方法名称。

在 hiBus 中，一个应用的不同模块都可以作为客户端连接到 hiBus 服务器上；单个应用的不同连接对应一个唯一的模块名称，每个模块都可以扮演如下单个角色或多个角色：

1. 泡泡（bubble）/事件发生器（event generator）：注册事件后，发生新的事件后向服务器写入事件数据。
1. 方法（method）/过程处理器（procedure handler）：注册过程后，等待调用请求、处理，然后发送处理结果。
1. 事件订阅者（event subscriber）：向服务器订阅指定的事件后，将收到对应的事件。
1. 过程调用者（procedure caller）：向服务器发送过程调用请求，之后同步等待或者异步等待处理结果。

下图描述了某个应用的两个模块。一个是控制模块，该模块和底层硬件交互，用于完成指定的控制指令，并根据设备的工作状态产生适当的事件；另外一个是交互模块，用于向用户展示底层设备的工作状态，并发送操控指令给控制模块。这两个模块均连接到 hiBus 服务器。

```
             -------------------------------
            |  hiWebKit-based GUI module    |
            |-------------------------------|
            |   Event subscribers           |
            |   Procedure callers           |
             -------------------------------
                            |
                            | WebSocket connection
                            |
                     --------------
                    | hiBus Server |
                     --------------
                            |
                            | Unix Domain Socket connection
                            |
             ------------------------------
            |   C/C++-based control module |
            |------------------------------|
            |   Event bubbles              |
            |   Procedure handlers         |
             ------------------------------
```

我们使用如下的端点（endpoint）名称来指定一个应用、模块、过程或事件：

- 应用：`@<host_name>/<app_name>`
- 模块：`@<host_name>/<app_name>/<module_name>`
- 过程：`@<host_name>/<app_name>/<module_name>/<method_name>`
- 事件：`@<host_name>/<app_name>/<module_name>/<bubble_name>`

其中，

- `<host_name>` 符合 FQDN（Fully Qualified Domain Name）规范。
- `<app_name>` 一个字符串，字母开头，可包含字母、数字和句点，句点不能连续出现；不区分大小写，但习惯上使用全小写字母。类似 Android 的应用名称。
- `<module_name>`、`<method_name>` 和 `<bubble_name>` 符合常见编程语言的变量名规范，但不区分大小写。

我们保留如下特别的主机名称和应用名称：

- `localhost`：指本机。
- `cn.fmsoft.hybridos.hibus`：保留的应用名称，指 hiBus 本身，注册或注销事件或过程时，向该应用发送指定的过程调用。该应用也可提供一些一般性的系统操作命令以及事件。


## 协议及接口

本协议的设计考虑到了跨设备，多模块的情形，但当前的 hiBus 服务器版本仅处理本机情形。

### 套接字接口

1. Unix Doman Socket 端口：
   - `/var/run/hibus`：通讯端口，用于过程调用、发送和接收事件。
1. Web Socket 端口（HybridOS 保留 7000 ~ 7999 端口）：
   - `7700`：通讯端口，用于过程调用、发送和接收事件。
   - `7701`：保留；或可用于中继功能。

理论上，当 hiBus 服务器运行在 Web Socket 端口且绑定到非本机回环（loopback）地址上时，有能力为其他设备节点提供事件及过程调用服务。

### 协议

#### 身份识别

当客户端（某个应用的某个模块）连接到上述套接字端口之后，服务器将首先发送一个挑战码（Challenge Code）给客户端，客户端发送身份信息给服务器，服务器据此确定客户端代表的应用名称。

客户端的身份信息主要包括主机名称、应用名称、模块名称，以及用于证明应用名称正确性的验证信息。

当前版本，Unix Domain Socket 和 Web Socket 均运行在本机上，不对外提供服务，因此，主机名称的确定策略非常简单：

1. 当使用 Unix Domain Socket 端口连接服务器时，主机名称将被始终确定为 `localhost`。
1. 当使用 Web Socket 端口连接服务器时，客户端的主机名称按如下规则确定：
   - IPv4 的回环地址（`127.0.0.1`，或 `127.0.0.0` 到 `127.255.255.255`），均被映射为 `localhost`。
   - IPv6 的回环地址（`0:0:0:0:0:0:0:1`，或 `::1`），被映射为 `localhost`。
   - 对其他 IP 地址，由客户端在进行身份验证时指定；在发生冲突的情况下，服务器添加后缀名（编号）后返回给客户端。未来，应通过名称服务完成反向解析。

应用名称的验证，需要客户端提供如下信息：

1. 应用名称本身。
1. 使用应用私钥对服务器发给客户端的挑战码进行签名。

服务器发送给客户端的挑战码使用如下数据包格式：

```json
{
    "packageType": "auth",
    "challengeCode": "..."
}
```

客户端发送给服务器端如下的身份验证数据包：

```json
{
    "packageType": "auth",
    "hostName": "localhost",
    "appName": "cn.fmsoft.hybridos.network",
    "moduleName": "self",
    "signature": "...",
}
```

服务器端将根据客户端发送的身份验证数据包，根据指定应用安装到系统中时提供的公钥执行验签操作，若通过，则给客户端发送如下的验证通过数据包：

```json
{
    "packageType": "authPassed",
    "reassignedHostName": "localhost",
}
```

其中，
- `reassignedHostName`：用于返回重新分配的主机名称，或者根据名称反向解析服务获得的正规主机名。

如果验证未通过，服务器将发送如下验证失败数据包，然后断开连接：

```json
{
    "packageType": "authFailed",
    "errCode": 409,
    "extraMsg": "Conflict"
}
```

其中，
- `errCode`：用于验证失败的错误码，可能的取值有（来自 HTTP 状态码）：
    + 400（Bad Request）：数据包格式错误。
    + 401（Unauthorized）：验签失败。
    + 404（Not Found）：系统中未安装指定应用。
    + 409（Conflict）：模块名冲突。
- `errMsg`：包含简单的错误信息。

在身份验证失败的情况下，服务器将立即断开连接。
在身份验证未通过之前，客户端发送到服务器所有非用于身份验证的数据包，都将被忽略并致使服务器立即断开连接。

注：在单一应用场景下，由于不安装第三方应用，所以应用的身份验签可以做简化处理；而在多应用场景下，需要一个额外的应用管理器配合身份验证。

#### 过程调用请求

发起执行特定过程（procedure）的请求时，向 hiBus 的过程调用端口写入如下 JSON 数据：

```json
{
    "packetType": "call",
    "callId": "<hased_call_identifier>",
    "procedure": "@<host_name>/<app_name>/<module_name>/<method_name>",
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
- `callId` 是调用方提供的一个唯一性字符串，用于在调用方标识特定的调用请求。
- `procedure` 是过程名称，包括主机名、应用名称、模块名称和方法名称。
- `expectedTime` 是期望的执行时间，毫秒为单位，为 0 表示不限（内部上限可在运行时配置，一般取 30s）。
- `authenInfo` 是可选的用户身份验证信息（如果该过程需要额外的用户身份验证的话）。当前版本暂不考虑。
- `parameter` 是该过程的执行参数。

#### 过程调用结果

hiBus 服务器会首先将过程调用请求转发给过程处理器，根据过程处理器的处理结果返回结果标识符、请求标识符、状态码以及可能的结果给调用者。相应的数据包格式如下：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "fromHost": "<host_name_processed_this_call>",
    "fromApp": "<app_name_processed_this_call>",
    "fromModule": "<module_name_processed_this_call>",
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
- `callId` 是调用方发起过程调用时提供的标识符。
- `fromHost` 表示处理该调用的主机名称。
- `fromApp` 表示处理该调用的应用名称。
- `fromModdule` 表示处理该调用的模块名称。
- `timeDiff` 是自收到原始请求到返回该结果的时间差；单位秒，浮点数。
- `retCode` 取 HTTP 状态码子集，可取如下值：
   - 200 Ok：表示过程正常执行并返回了结果。
   - 202 Accepted：表示已提交参数给过程，但该过程可能耗时较长，所以暂时没有返回结果。
   - 400 Bad Request：表示 JSON 格式解析错误。
   - 401 Unauthorized：表示需要验证用户身份，但未提供。
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

在正常情况下，调用者会首先收到 202 状态码，然后在未来的某个时间点收到 200 状态码。只有状态码为 200 时，才表示过程执行完毕并包含有返回值（`retValue`）信息。

通常，当 `retCode` 为 202 时，调用方应根据 `resultId` 来监视后续从服务器返回的数据包，从而获得最终的执行状态或结果。

注意，过程处理器返回给服务器的结果数据包格式，和上述数据包类似，但缺少 `fromHost`、`fromApp` 以及 `timeDiff` 等字段，但包含 `timeConsumed` 字段，表示过程处理器执行该调用消耗的时间。

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "retCode": 200,
    "timeConsumed": 1.2345,
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

如果以上检查均无问题，则转发过程调用请求到过程所在的应用模块，然后向调用者返回 202 状态码。

转发过程调用请求时，服务器会在调用请求数据包中增加由服务器生成的用于追踪该调用的全局唯一性标识符 `resultId`。如下所示：

```json
{
    "packetType": "call",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
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
但调用者可发起多个请求，然后根据 `resultId` 和 `callId` 来跟踪这些请求返回的结果数据。

#### 产生事件

当事件发生器产生一个新的事件后，向服务器写入如下格式的 JSON 数据。产生的事件数据包通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<hased_event_identifier>",
    "bubbleName": "<bubble_name>",
    "bubbleData": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId`：事件发生器为每个事件分配的一个全局唯一字符串。
- `bubbleName`：事件名称。
- `bubbleData` 包含真正的事件数据。

注意，由于事件发生器所在的模块连接对应了其所在主机、应用以及模块，故而在产生的事件数据包中无需指定这些信息。

#### 收到事件

当服务器收到一个新的事件后，将把该事件转发给所有的订阅者。事件订阅者将从服务器获得该事件数据。收到的事件数据包通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<hased_event_identifier>",
    "timeDiff": 0.1234,
    "fromHost": "<host_name>",
    "fromApp": "<app_name>",
    "fromModule": "<module_name>",
    "bubbleName": "<bubble_name>",
    "bubbleData": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId` 是事件发生器为每个事件分配的一个全局唯一字符串。
- `timeDiff` 表示服务器收到该事件到转发该事件的时间差；单位秒，浮点数。
- `fromHost` 表示发生该事件的主机名称。
- `fromApp` 表示发生该事件的应用名称。
- `fromModule` 表示发生该事件的模块名称。
- `bubbleName` 是事件泡泡名称。
- `bubbleData` 包含真正的事件泡泡数据。

#### 跨设备连接的思考

注：当前版本仅处理本机上的连接。

跨设备（主机）连接有两种思路：

1. hiBus 服务器使用 Web Socket 在非回环地址上监听连接请求，另一个主机上的应用通过 Web Socket 直接连接到该服务器。问题：如何对另一台主机上的应用进行身份验证？
1. 在不同的 hiBus 服务器实例之间建立中继服务，所有针对另一个主机上的请求，由 hiBus 服务器之间通过中继完成，从而实现跨主机的远程过程调用或者事件订阅。此种设计下，身份验证只在 hiBus 服务器之间进行，各主机上的应用身份验证，由本机处理。这样的话，本机 Web Socket 的事件和过程调用端口均只在本地回环地址上提供服务。比如，当过程调用的目标主机非本机时，hiBus 服务器可将该请求转发给目标主机所在的 hiBus 服务器，然后将结果转发给调用者。

### hiBus 内置过程

hiBus 服务器通过内置过程实现注册过程/事件等功能。

#### 注册过程

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/registerProcedure`
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
    "callId": "<hased_call_identifier>",
    "procedure": "@localhost/cn.fmsoft.hybridos.hibus/builtin/registerProcedure",
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
    "callId": "<hased_call_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "fromModule": "builtin",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": null,
}
```

#### 撤销过程

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/revokeProcedure`
- 参数：
   + `methodName`：待撤销过程的方法名称；由于主机名和应用名是隐含的，所以无需指定。
- 返回值：无。客户端依据结果的 `retCode` 判断是否撤销成功，可能的值有：
   + `404`：表示未找到指定的过程。
   + `403`：表示待撤销的过程不是由调用方注册的，所以不能撤销。
   + `423`：表示有调用者正在等待该过程返回结果，不能撤销。
   + `200`：表示成功。

#### 注册事件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/registerEvent`
- 参数：
   + `bubbleName`：待注册的事件名称；由于主机名和应用名是隐含的，所以无需指定。
   + `forHost`：可以订阅该事件的主机名称，可指定多个主机（逗号分隔），亦可使用通配符。
   + `forApp`：可以订阅该事件的应用名称，可指定多个主机（逗号分隔），亦可使用通配符。
- 返回值：无。客户端依据结果的 `retCode` 判断是否注册成功，可能的值有：
   + `409`：表示重复的事件名称/方法名称。
   + `200`：表示成功。

#### 撤销事件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/revokeEvent`
- 参数：
   + `bubbleName`：待撤销过程的事件名称；由于主机名和应用名是隐含的，所以无需指定。
- 返回值：无。客户端依据结果的 `retCode` 判断是否撤销成功，可能的值有：
   + `404`：表示未找到指定的事件。
   + `403`：表示待撤销的事件不是由调用方注册的，所以不能撤销。
   + `200`：表示成功。

#### 订阅事件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/subscribeEvent`
- 参数：
   + `event`：要订阅的完整事件名称，含主机名、应用名以及泡泡名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否订阅成功，可能的值有：
   + `404`：表示未找到指定的事件。
   + `403`：表示事件发生器不允许调用方订阅该事件。
   + `200`：表示成功。

#### 取消事件订阅

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/unsubscribeEvent`
- 参数：
   + `event`：要取消订阅的完整事件名称，含主机名、应用名以及泡泡名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否取消成功，可能的值有：
   + `404`：表示调用方未订阅指定的事件；或者该事件已经被撤销。
   + `200`：表示成功。

#### 列出已注册过程

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listProcedures`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可调用的过程清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "fromModule": "builtin",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "localhost/cn.fmsoft.hybridos.networkManager/getHotSpots",
        "localhost/cn.fmsoft.hybridos.networkManager/connectToHotSpot",
    ],
}
```

#### 列出已注册事件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listEvents`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可订阅的事件清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "fromModule": "builtin",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "@localhost/cn.fmsoft.hybridos.networkManager/NETWORKCHANGED",
        "@localhost/cn.fmsoft.hybridos.networkManager/SIGNALCHANGED",
    ],
}
```

#### 列出事件的订阅者

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listEventSubscribers`
- 参数：
   + `eventName`：事件名称，含完整的主机名、应用名、模块名以及泡泡名。
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
    "callId": "<hased_call_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "fromModule": "builtin",
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "@localhost/cn.fmsoft.hybridos.foo/first",
        "@localhost/cn.fmsoft.hybridos.bar/second",
    ],
}
```

## hiBus 服务器的架构及关键模块

### 架构

### 关键模块

### 调试工具

### 客户端接口

## 演进规划

