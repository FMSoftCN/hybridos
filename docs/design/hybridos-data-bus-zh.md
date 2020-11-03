# 合璧操作系统设备端数据总线概要设计

魏永明

本文阐述合璧操作系统设备端数据总线（hiBus）1.0 版本的设计。

- [基本框架及术语](#基本框架及术语)
- [协议及接口](#协议及接口)
   + [套接字接口](#套接字接口)
   + [协议](#协议)
      * [身份识别](#身份识别)
      * [过程调用请求](#过程调用请求)
      * [过程调用结果](#过程调用结果)
      * [转发过程调用请求给过程端点](#转发过程调用请求给过程端点)
      * [产生事件](#产生事件)
      * [收到事件](#收到事件)
      * [乒乓心跳](#乒乓心跳)
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
      * [回声](#回声)
- [架构及关键模块](#架构及关键模块)
   + [架构及服务器模块构成](#架构及服务器模块构成)
   + [命令行](#命令行)
   + [客户端接口](#客户端接口)
      * [全局类型](#全局类型)
      * [连接管理](#连接管理)
      * [辅助函数](#辅助函数)
      * [过程管理](#过程管理)
      * [事件管理](#事件管理)
      * [订阅事件](#订阅事件)
      * [调用过程](#调用过程)
   + [等待数据包及事件循环](#等待数据包及事件循环)
- [其他](#其他)
   + [简单的应用管理](#简单的应用管理)
   + [跨设备连接的思考](#跨设备连接的思考)


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

1. 提供两种类型的底层连接通道：本地的 Unix Domain Socket 和 Web Socket，以方便使用不同编程语言开发的模块都可以方便地连接到 hiBus 上。
1. 提供基本的安全机制，以决定某个应用或者某个远程节点能否订阅（subscribe）特定的事件，以及能否调用（call）某个特定的过程（procedure）。
1. 考虑到在未来，hiBus 可通过 Web Socket 向局域网中的其他 IoT 设备节点提供服务。因此，需要在事件订阅以及调用远程过程时包含主机名称信息。
1. 重新设计的通讯协议，可避免出现同一应用扮演不同角色时出现死锁情形。

术语：

1. 主机（host）：指合璧操作系统设备侧赖以运行的一个计算机主机。
1. 应用（app）：合璧操作系统中，一个应用由多个不同的行者组成，这些行者可能使用不同的编程语言开发，在系统中以进程或线程的形式运行。
1. 行者（runner）：行者用来区分一个应用的不同并行运行组件。在合璧操作系统中，一个应用可以有多个行者，通常以独立的进程或者线程形式存在。比如专门用于处理底层设备控制的行者，专门用于处理人机交互的行者。
1. 事件（event）/泡泡（bubble）：事件名称中包含了主机名称、应用名称、行者名称以及泡泡（即事件发生器）名称。
1. 过程（procedure）/方法（method）：过程名称中包含了主机名称、应用名称、行者名称以及方法名称。

在 hiBus 中，一个应用的不同行者都可以作为客户端连接到 hiBus 服务器上；单个应用的不同连接对应一个唯一的行者名称，每个行者都可以扮演如下单个角色或多个角色：

1. 泡泡（bubble）/事件发生器（event generator）：注册事件后，发生新的事件后向服务器写入事件数据。
1. 方法（method）/过程处理器（procedure handler）：注册过程后，等待调用请求、处理，然后发送处理结果。
1. 事件订阅者（event subscriber）：向服务器订阅指定的事件后，将收到对应的事件。
1. 过程调用者（procedure caller）：向服务器发送过程调用请求，之后同步等待或者异步等待处理结果。

下图描述了某个应用的两个行者。一个是控制行者，该行者和底层硬件交互，用于完成指定的控制指令，并根据设备的工作状态产生适当的事件；另外一个是交互行者，用于向用户展示底层设备的工作状态，并发送操控指令给控制行者。这两个行者均连接到 hiBus 服务器。

```
             -------------------------------
            |  hiWebKit-based GUI runner    |
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
            |   C/C++-based control runner |
            |------------------------------|
            |   Event bubbles              |
            |   Procedure handlers         |
             ------------------------------
```

我们使用如下的端点（endpoint）名称来指定一个行者（含有主机名、应用名及行者名）：

- 行者：`@<host_name>/<app_name>/<runner_name>`

使用如下的字符串指代一个过程或事件：

- 过程：`@<host_name>/<app_name>/<runner_name>/<method_name>`
- 事件：`@<host_name>/<app_name>/<runner_name>/<bubble_name>`

其中，

- `<host_name>` 符合 FQDN（Fully Qualified Domain Name）规范。长度不超过 127 字节。
- `<app_name>` 一个字符串，字母开头，可包含字母、数字和句点，句点不能连续出现；不区分大小写，但习惯上使用全小写字母。类似 Android 的应用名称。长度不超过 127 字节。
- `<runner_name>`、`<method_name>` 以及 `<bubble_name>` 符合常见编程语言的变量名规范，但不区分大小写。长度不超过 64 字节。

我们保留如下特别的主机名称和应用名称：

- `localhost`：指本机。
- `cn.fmsoft.hybridos.hibus`：保留的应用名称，指 hiBus 本身，注册或注销事件或过程时，向该应用发送指定的过程调用。该应用也可提供一些一般性的系统操作命令以及事件。


## 协议及接口

本协议的设计考虑到了跨设备，多行者的情形，但当前的 hiBus 服务器版本仅处理本机情形。

### 套接字接口

1. Unix Doman Socket 端口：
   - `/var/run/hibus.sock`：通讯端口，用于过程调用、发送和接收事件。
1. Web Socket 端口（HybridOS 保留 7000 ~ 7999 端口）：
   - `7700`：通讯端口，用于过程调用、发送和接收事件。
   - `7701`：保留；或可用于中继功能。

理论上，当 hiBus 服务器运行在 Web Socket 端口且绑定到非本机回环（loopback）地址上时，有能力为其他设备节点提供事件及过程调用服务。

### 协议

#### 身份识别

当客户端（某个应用的某个行者）连接到上述套接字端口之后，服务器将首先发送一个挑战码（Challenge Code）给客户端，客户端发送身份信息给服务器，服务器据此确定客户端代表的应用名称。

客户端的身份信息主要包括主机名称、应用名称、行者名称，以及用于证明应用名称正确性的验证信息。

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
    "runnerName": "self",
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
    + 409（Conflict）：行者名冲突。
- `errMsg`：包含简单的错误信息。

在身份验证失败的情况下，服务器将立即断开连接。
在身份验证未通过之前，客户端发送到服务器所有非用于身份验证的数据包，都将被忽略并致使服务器立即断开连接。

注：在单一应用场景下，由于不安装第三方应用，所以应用的身份验签可以做简化处理；而在多应用场景下，需要一个额外的应用管理器配合身份验证。

#### 过程调用请求

发起执行特定过程（procedure）的请求时，向 hiBus 服务器写入如下 JSON 数据：

```json
{
    "packetType": "call",
    "callId": "<hased_call_identifier>",
    "procedure": "@<host_name>/<app_name>/<runner_name>/<method_name>",
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
- `procedure` 是过程名称，包括主机名、应用名称、行者名称和方法名称。
- `expectedTime` 是期望的执行时间，毫秒为单位，为 0 表示不限（内部上限可在运行时配置，一般取 30s）。
- `authenInfo` 是可选的用户身份验证信息（如果该过程需要额外的用户身份验证的话）。当前版本暂不考虑。
- `parameter` 是该过程的执行参数。

#### 过程调用结果

hiBus 服务器会首先将过程调用请求转发给过程端点，根据过程处理器的处理结果返回结果标识符、请求标识符、状态码以及可能的结果给调用者。相应的数据包格式如下：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "fromHost": "<host_name_processed_this_call>",
    "fromApp": "<app_name_processed_this_call>",
    "fromRunner": "<runner_name_processed_this_call>",
    "timeConsumed": 0.5432,
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
- `fromModdule` 表示处理该调用的行者名称。
- `timeConsumed` 是方法处理器实际消耗的事件；单位秒，浮点数。
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

#### 转发过程调用请求给过程端点

hiBus 服务器收到执行特定过程的请求后，首先做如下检查：

- 解析调用请求；存在无效数据时，比如无效的过程名称，未提供参数，或者参数不符合要求等，返回 400、405 或者 406 状态码。
- 指定的过程是否存在；若不存在，立即返回 404。
- 调用者是否拥有执行该过程的权限；若没有权限或者未提供要求的身份验证信息，则分别返回 403、401 状态码。

如果以上检查均无问题，则转发过程调用请求到过程所在的应用行者，然后向调用者返回 202 状态码。

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

注意，由于事件发生器所在的行者连接对应了其所在主机、应用以及行者，故而在产生的事件数据包中无需指定这些信息。

#### 收到事件

当服务器收到一个新的事件后，将把该事件转发给所有的订阅者。事件订阅者将从服务器获得该事件数据。收到的事件数据包通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<hased_event_identifier>",
    "timeDiff": 0.1234,
    "fromHost": "<host_name>",
    "fromApp": "<app_name>",
    "fromRunner": "<runner_name>",
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
- `fromRunner` 表示发生该事件的行者名称。
- `bubbleName` 是事件泡泡名称。
- `bubbleData` 包含真正的事件泡泡数据。

#### 乒乓心跳

服务器或者客户端应通过乒乓心跳数据包来判断对方是否仍然可用。

```json
{
    "packetType": "ping",
    "pingId": "<hased_ping_identifier>",
}
```

任何一方收到 `ping` 数据包之后，应立即发送 `pong` 数据包：

```json
{
    "packetType": "pong",
    "pingId": "<the_ping_identifier_correspond_to_this_pong>",
}
```

一般而言，在 Internet 连接上使用乒乓心跳可以有效检测连接的意外丢失，但对通过 Unix Domain Socekt 建立的连接是不需要执行乒乓心跳测试的。

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
    "fromRunner": "builtin",
    "timeConsumed": 0.5432,
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
    "fromRunner": "builtin",
    "timeConsumed": 0.5432,
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
    "fromRunner": "builtin",
    "timeConsumed": 0.5432,
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
   + `eventName`：事件名称，含完整的主机名、应用名、行者名以及泡泡名。
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
    "fromRunner": "builtin",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": [
        "@localhost/cn.fmsoft.hybridos.foo/first",
        "@localhost/cn.fmsoft.hybridos.bar/second",
    ],
}
```

#### 回声

该过程主要用于测试

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/echo`
- 参数：
   + `words`：一个非空字符串。
- 返回值：成功时返回传入的非空字符串。
- 常见错误状态码：
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<hased_result_identifier>",
    "callId": "<hased_call_identifier>",
    "fromHost": "localhost",
    "fromApp": "cn.fmsoft.hybridos.hibus",
    "fromRunner": "builtin",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retValue": "I am still live",
}
```

## 架构及关键模块

hiBus 服务器使用 C/C++ 语言开发，由服务器程序、命令行程序和供客户端使用的函数库（以及头文件）三部分组成。

### 架构及服务器模块构成

下图描述了 iBus 系统的架构以及服务器的主要模块构成。

```
    -----------       -----------        -----------
   |    App 1  |     |    App 2  |      |    App 3  |
   |-----------      |-----------       |-----------
   |  Runner 1 |     |  Runner 1 |      |  Runner 1 |
   |  Runner 2 |     |  Runner 2 |      |  Runner 2 |
   |  ...      |     |  ...      |      |  ...      |
   |  Runner N |     |  Runner N |      |  Runner N |          ...
    -----------       -----------        -----------
         |                 |                 |                  |
         -------------------------------------------------------
                                   |
                                   | WebSocket connection
                                   |
 --------------------------------------------------------------------
|                    Web Socket Connection Manager                   |
 --------------------------------------------------------------------
|                     Connection/Endpoint Manager                    |
 --------------------------------------------------------------------
|      Registered Event Manager    |   Registered Procedure Manager  |
|                           Event Subscriber Manager                 |
 --------------------------------------------------------------------
|           Event Dispatcher       |        Call Dispatcher          |     hiBus Server
 --------------------------------------------------------------------
|                           The Builtin Endpoint                     |
 --------------------------------------------------------------------
|                                Helpers                             |
|   ------------------------------------------------------------     |
|   |       JSON Processing        |    Security Processing     |    |          
 --------------------------------------------------------------------
|                 Unix Domain Socket Connection Manager              |
 --------------------------------------------------------------------
                                   |
                                   | Unix Domain Socket connection
                                   |
         -----------------------------------------------------------
         |                              |                           |
    --------------------       --------------------                ...
   | hiBus Command Line |     |      App 5         |
   |--------------------      |--------------------
   | Event handlers     |     |      Runner 1      |
   | Result handlers    |     |      Runner 2      |
   |                    |     |        ...         |
    --------------------       --------------------
```

如上图所示，App 通过 Web Socket 或者 Unix Domain Socket 连接到 hiBus 服务器，每个应用可包含若干行者，可用于过程处理，产生事件，或者处理事件，发起过程调用。

其中，hiBus 命令行（command line）作为 `cn.fmsoft.hybridos.hibus` 应用的外挂行者实现，可用于系统管理员查看或者开发者调试使用。

hiBus 服务器主要包含如下软件模块：

- Web Socket/Unix Domain Socket 连接管理器：该模块用于监听套接字端口，接受连接请求，检测连接是否已断开等。
- 连接/端点管理器（Connection/Endpoint Manager）：该模块验证应用身份，记录连接和端点的映射关系。
- 已注册事件管理器（Registered Event Manager）：该模块处理事件的注册及撤销请求，维护已注册事件列表。
- 已注册过程管理器（Registered Procedure Manager）：该模块处理过程的注册及撤销请求，维护已注册过程列表。
- 事件订阅者管理器（Event Subscriber Manager）：该模块管理所有的事件订阅者清单信息，处理事件的订阅及取消订阅请求。
- 事件分发器（Event Dispatcher）：该模块将端点产生的事件分发给订阅者。
- 调用分发器（Call Dispatcher）：该模块将端点发起的过程调用分发给对应的端点进行处理。
- 内置端点（The Builtin Endpoint）：该模块实现 hiBus 的内置过程。
- 辅助模块（Helpers）：辅助模块中主要包括 JSON 的处理模块以及安全性处理模块。前者主要用于解析 JSON 格式的数据包并将其转换成内部结构，或者反之。后者主要提供基于非对称个加密算法的签名验证功能以及通配符处理功能等。

### 命令行

hiBus 的命令行工具，将被编译为独立的程序，该程序以端点名 `@localhost/cn.fmsoft.hybridos.hibus/cmdline` 连接到服务器，视为 hiBus 本身的 `cmdline` 行者，以独立进程方式运行。

使用该命令行工具时，可通过 hiBus 内置过程来查询已注册过程、事件、特定事件的订阅者信息，亦可订阅特定事件，或者调用某个特定的过程。

### 客户端接口

客户端接口编译为独立函数库（libhibus.so），提供 C 语言接口，以方便使用不同编程语言开发的客户端使用这些接口注册/撤销过程、注册/撤销/订阅事件等。

所有的客户端接口均是线程安全的，这样，我们可以用某个线程来实现某个行者。比如，我们可以在主线程中以 `event` 行者名连接到服务器，在主线程中只处理事件，而所有调用远程过程的功能则在另一个线程中实现，其行者名为 `call`。如此设计，可帮助构造符合数据驱动需求的良好应用架构。

#### 全局类型

```c
struct _hibus_conn;
typedef struct _hibus_conn hibus_conn;

struct _hibus_json;
typedef struct json_object hibus_json;
```

`hibus_conn` 是一个用来表示 hiBus 连接的匿名数据结构。

`hibus_json` 是一个用来表示 JSON 对象的数据结构。

#### 连接管理

使用如下接口之一连接到 hiBus 服务器：

```c
int hibus_connect_via_unix_socket (const char* path_to_socket, const char* runner_name, hibus_conn** conn);
int hibus_connect_via_web_socket (const char* host_name, int port, const char* runner_name, hibus_conn** conn);
```

上面的两个函数，分别使用 Unix Domain Socket 或者 Web Socket 连接到 hiBus 服务器上。函数的返回值为套接字文件描述符（fd）。`fd >= 0` 时表明连接成功，此时会通过 `conn` 参数返回一个匿名的 `hibus_conn` 结构指针。`fd < 0` 时表明连接失败，其绝对值标识错误编码。

若连接成功，其后所有的接口均使用通过 `conn` 返回的结构指针来标识该连接。

使用如下接口关闭到 hiBus 服务器的连接：

```c
int hibus_disconnect (hibus_conn* conn);
```

使用如下接口从 `hibus_conn` 结构中获得相关信息（主机名、应用名、行者名、套接字文件描述符）：

```c
const char* hibus_conn_host_name (hibus_conn* conn);
const char* hibus_conn_app_name (hibus_conn* conn);
const char* hibus_conn_runner_name (hibus_conn* conn);
int hibus_conn_socket_fd (hibus_conn* conn);
```

#### 辅助函数

行者可使用如下辅助函数来解析端点名称：

```c
#define LEN_HOST_NAME       127
#define LEN_APP_NAME        127
#define LEN_MODULE_NAME     64
#define LEN_METHOD_NAME     64
#define LEN_BUBBLE_NAME     64

int hibus_get_host_name (const char* endpoint, char* buff);
int hibus_get_app_name (const char* endpoint, char* buff);
int hibus_get_runner_name (const char* endpoint, char* buff);

char* hibus_get_host_name_alloc (const char* endpoint);
char* hibus_get_app_name_alloc (const char* endpoint);
char* hibus_get_runner_name_alloc (const char* endpoint);
```

行者可使用如下辅助函数来组装一个端点名称：

```c
int hibus_assembly_endpoint (const char* host_name, const char* app_name,
        const char* runner_name, char* buff);

char* hibus_assembly_endpoint_alloc (const char* host_name, const char* app_name,
        const char* runner_name);
```

#### 过程管理

行者可以使用如下的接口注册或撤销过程：

```c
typedef hibus_json* (*hibus_method_handler)(hibus_conn* conn,
        const char* from_endpoint, const char* method_name,
        const hibus_json* method_param);

int hibus_register_procedure (hibus_conn* conn, const char* method_name,
        hibus_method_handler method_handler);
int hibus_revoke_procedure (hibus_conn* conn, const char* method_name);
```

`hibus_register_procedure` 函数用于注册一个过程。当该行者收到调用该过程的请求时，将自动调用对应的回调函数 `method_cb`，并传递方法名、调用者端点以及过程参数。

`hibus_revoke_procedure` 函数用于撤销一个过程。

#### 事件管理

行者可以使用如下的接口注册或撤销事件：

```c
int hibus_register_event (hibus_conn* conn, const char* bubble_name,
        const char* to_host, const char* to_app);
int hibus_revoke_event (hibus_conn* conn, const char* bubble_name);
int hibus_fire_event (hibus_conn* conn,
        const char* bubble_name, const hibus_json* bubble_data);
```

`hibus_register_event` 函数用于注册一个事件。

`hibus_revoke_event` 函数用于撤销一个事件。

`hibus_fire_event` 用于产生一个事件。

#### 订阅事件

模块可以使用如下的接口订阅或者取消订阅一个事件：

```c
typedef void (*hibus_event_handler)(hibus_conn* conn,
        const char* from_endpoint, const char* bubble_name,
        const hibus_json* bubble_data);

int hibus_subscribe_event (hibus_conn* conn,
        const char* endpoint, const char* bubble_name,
        hibus_event_handler event_handler);

int hibus_unsubscribe_event (hibus_conn* conn,
        const char* endpoint, const char* bubble_name);
```

`hibus_subscribe_event` 函数用于订阅一个事件。当指定的事件来临时，`event_handler` 指定的事件回调函数将被调用。

`hibus_unsubscribe_event` 函数用于取消对一个事件的订阅。

`hibus_wait_for_event` 函数用于等待事件。

#### 调用过程

行者可使用如下接口调用过程：

```c
typedef void (*hibus_result_handler)(hibus_conn* conn,
        const char* from_endpoint, const char* method_name,
        int ret_code, const hibus_json* ret_value);

int hibus_call_procedure (hibus_conn* conn, const char* endpoint, const char* method_name,
        const hibus_json* method_praram, time_t ret_time_expected, hibus_result_handler result_handler);

int hibus_call_procedure_and_wait (hibus_conn* conn, const char* endpoint, const char* method_name,
        const hibus_json* method_praram, time_t ret_time_expected, hibus_json** ret_value);
```

`hibus_call_procedure` 提供了异步调用远程过程的接口：发起调用后会立即返回，然后在回调函数（`result_handler`）中等待结果或错误。此时，需要配合后面讲到的 `hibus_wait_for_packet` 函数使用。

`hibus_call_procedure_and_wait` 提供了同步调用远程过程的接口：发起调用后将等待结果或错误然后返回。注意，在等待返回值的过程中，可能会收到事件，此时，该函数会调用相应的事件处理器。

### 等待数据包及事件循环

```c
int hibus_wait_for_packet (hibus_conn* conn, struct timeval *timeout);
```

该函数检查服务器发过来的数据，并调用相应的事件处理器或者结果处理器。通常在一个循环中使用，如：

```c
    struct timeval timeout = { 0, 100000 }; // 100ms

    while (1) {

        ...

        hibus_wait_for_packet (conn, &timeout);

        ...
    }
```

## 其他

### 简单的应用管理

在 HybridOS 系统中，应用包管理类似 Android 系统：

- 每个应用被安装在独立的目录中，且每个应用对应一个唯一的用户账号，通常使用和应用名称一样的用户账号名称。
- 每个应用所在目录中包含有一对非对称加密算法使用的公钥和私钥。在安装该应用时，对应的公钥将被复制到系统目录（如 `/usr/apps/cn.fmsoft.hybridos.hibus/`）中，可用于验证签名。
- 检查应用是否安装好，只要检查系统目录中是否有对应的应用即可。
- 对具有需要超级用户权限的应用，将其对应的用户名加入到 `admin` 组中。

在没有应用管理器的情况下，使用系统管理员账号（root）完成应用目录的创建、用户名的创建以及公钥的复制。

### 跨设备连接的思考

注：当前版本仅处理本机上的连接。

跨设备（主机）连接有两种思路：

1. hiBus 服务器使用 Web Socket 在非回环地址上监听连接请求，另一个主机上的应用通过 Web Socket 直接连接到该服务器。问题：如何对另一台主机上的应用进行身份验证？
1. 在不同的 hiBus 服务器实例之间建立中继服务，所有针对另一个主机上的请求，由 hiBus 服务器之间通过中继完成，从而实现跨主机的远程过程调用或者事件订阅。此种设计下，身份验证只在 hiBus 服务器之间进行，各主机上的应用身份验证，由本机处理。这样的话，本机 Web Socket 的事件和过程调用端口均只在本地回环地址上提供服务。比如，当过程调用的目标主机非本机时，hiBus 服务器可将该请求转发给目标主机所在的 hiBus 服务器，然后将结果转发给调用者。

