# 合璧操作系统设备端数据总线概要设计

【主题】合璧操作系统设备端数据总线概要设计  
【概要】本文阐述合璧操作系统设备端数据总线（hiBus）1.0 版本的设计  
【作者】魏永明
【日期】2020 年 11 月  
【状态】定稿  

*版权声明*

版权所有 &copy; 2020 北京飞漫软件技术有限公司 
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

*目录*

- [基本框架及术语](#基本框架及术语)
- [协议及接口](#协议及接口)
   + [套接字接口](#套接字接口)
   + [协议](#协议)
      * [连接](#连接)
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
      * [列出已连接端点](#列出已连接端点)
      * [列出已注册过程](#列出已注册过程)
      * [列出已注册事件](#列出已注册事件)
      * [列出事件的订阅者](#列出事件的订阅者)
      * [回声](#回声)
   + [hiBus 内置事件](#hibus-内置事件)
      * [新行者事件](#新行者事件)
      * [行者断开事件](#行者断开事件)
      * [丢失事件发生器事件](#丢失事件发生器事件)
      * [撤销泡泡事件](#撤销泡泡事件)
- [架构及关键模块](#架构及关键模块)
   + [架构及服务器模块构成](#架构及服务器模块构成)
   + [命令行](#命令行)
   + [客户端接口](#客户端接口)
      * [全局类型](#全局类型)
      * [连接管理](#连接管理)
      * [数据包读写函数](#数据包读写函数)
      * [辅助函数](#辅助函数)
      * [过程管理](#过程管理)
      * [事件管理](#事件管理)
      * [订阅事件](#订阅事件)
      * [调用过程](#调用过程)
      * [等待并分发数据包](#等待并分发数据包)
- [其他](#其他)
   + [简单的应用管理](#简单的应用管理)
   + [端点权限管理](#端点权限管理)
   + [跨设备连接的思考](#跨设备连接的思考)
- [附：商标声明](#附商标声明)

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

1. 提供两种类型的底层连接通道：本地的 Unix Domain Socket（简称 UnixSocket）和 WebSocket，以方便使用不同编程语言开发的模块都可以方便地连接到 hiBus 上。
1. 提供基本的安全机制，以决定某个应用或者某个远程节点能否订阅（subscribe）特定的事件，以及能否调用（call）某个特定的过程（procedure）。
1. 考虑到在未来，hiBus 可通过 WebSocket 向局域网中的其他 IoT 设备节点提供服务。因此，需要在事件订阅以及调用远程过程时包含主机名称信息。
1. 重新设计的通讯协议，可避免出现同一应用扮演不同角色时出现死锁情形。

术语：

1. 主机（host）：指合璧操作系统设备侧赖以运行的一个计算机主机。
1. 应用（app）：合璧操作系统中，一个应用由多个不同的行者组成，这些行者可能使用不同的编程语言开发，在系统中以进程或线程的形式运行。
1. 行者（runner）：行者用来区分一个应用的不同并行运行组件。在合璧操作系统中，一个应用可以有多个行者，通常以独立的进程或者线程形式存在。比如专门用于处理底层设备控制的行者，专门用于处理人机交互的行者。
1. 事件（event）/泡泡（bubble）：事件名称中包含了主机名称、应用名称、行者名称以及泡泡名称。
1. 过程（procedure）/方法（method）：过程名称中包含了主机名称、应用名称、行者名称以及方法名称。

在 hiBus 中，一个应用的不同行者都可以作为客户端连接到 hiBus 服务器上；单个应用的不同连接对应一个唯一的行者名称，每个行者都可以扮演如下单个角色或多个角色：

1. 事件发生器（event generator）：注册事件后，发生新的事件后向服务器写入事件数据。
1. 过程处理器（procedure handler）：注册过程后，等待调用请求、处理，然后发送处理结果。
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
                            | UnixSocket connection
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
- `<runner_name>`、`<method_name>` 以及 `<bubble_name>` 符合常见编程语言的变量名规范，但不区分大小写。长度不超过 63 字节。按照惯例，我们一般使用全大写字母来表示泡泡名。

我们保留如下特别的主机名称和应用名称：

- `localhost`：指本机。
- `cn.fmsoft.hybridos.ibus`：保留的应用名称，指 hiBus 本身，注册或注销事件或过程时，向该应用发送指定的过程调用。该应用也可提供一些一般性的系统操作命令以及事件。

## 协议及接口

本协议的设计考虑到了跨设备，多行者的情形，但当前的 hiBus 服务器版本仅处理本机情形。

### 套接字接口

1. UnixSocket 端口：
   - `/var/run/hibus.sock`：通讯端口，用于过程调用、发送和接收事件。
1. WebSocket 端口（HybridOS 保留 7000 ~ 7999 端口）：
   - `7700`：通讯端口，用于过程调用、发送和接收事件。
   - `7701`：保留；或可用于中继功能。

理论上，当 hiBus 服务器运行在 WebSocket 端口且绑定到非本机回环（loopback）地址上时，有能力为其他设备节点提供事件及过程调用服务。

### 协议

#### 连接

当客户端通过 WebSocket 连接到服务器时，WebSocket 规范规定了初始的连接处理。但通过 UnixSocket 连接时，服务器可能在达到资源上限或者其他错误情形下拒绝客户端的连接请求，此时，服务器向客户端发送如下数据包，然后断开连接：

```json
{
    "packetType": "error",
    "protocolName": "HIBUS",
    "protocolVersion": 90,
    "causedBy": [ "event" | "call" | "result" ],
    "causedId": "<event_id> | <call_id> | <result_id> ",
    "retCode": 503,
    "retMsg": "Service Unavailable",
}
```

其中，
- `protocolName`：包含协议名称，当前取 `HIBUS`。
- `protocolVersion`：包含协议版本号，正整数。
- `causedBy`: 包含导致该错误的数据包类型，可取 `event`、`call`、`result` 等。该字段可选，若缺失，表示不可恢复的一般性错误，这种情况下，服务器通常会断开连接。
- `causedId`：包含导致该错误的数据包标识符。该字段仅在 `causedBy` 字段存在时出现。
- `retCode`：包含错误码，一般性错误时，可能的取值有（来自 HTTP 状态码）：
    + 503（Service Unavailable）：达到连接上限。
    + 503（Service Unavailable）：达到连接上限。
    + 505（Internal Server Error）：内部错误。
- `retMsg`：包含简单的信息。

#### 身份识别

当客户端（某个应用的某个行者）连接到上述套接字端口之后，服务器将首先发送一个挑战码（Challenge Code）给客户端，客户端发送身份信息给服务器，服务器据此确定客户端代表的应用名称。

客户端的身份信息主要包括主机名称、应用名称、行者名称，以及用于证明应用名称正确性的验证信息。

当前版本，UnixSocket 和 WebSocket 均运行在本机上，不对外提供服务，因此，主机名称的确定策略非常简单：

1. 当使用 UnixSocket 端口连接服务器时，主机名称将被始终确定为 `localhost`。
1. 当使用 WebSocket 端口连接服务器时，客户端的主机名称按如下规则确定：
   - IPv4 的回环地址（`127.0.0.1`，或 `127.0.0.0` 到 `127.255.255.255`），均被映射为 `localhost`。
   - IPv6 的回环地址（`0:0:0:0:0:0:0:1`，或 `::1`），被映射为 `localhost`。
   - 对其他 IP 地址，由客户端在进行身份验证时指定；在发生冲突的情况下，服务器添加后缀名（编号）后返回给客户端。未来，应通过名称服务完成反向解析。

应用名称的验证，需要客户端提供如下信息：

1. 应用名称本身。
1. 使用应用私钥对服务器发给客户端的挑战码进行签名。

服务器发送给客户端的挑战码使用如下数据包格式：

```json
{
    "packetType": "auth",
    "protocolName": "HIBUS",
    "protocolVersion": 90,
    "challengeCode": "..."
}
```

客户端发送给服务器端如下的身份验证数据包：

```json
{
    "packetType": "auth",
    "protocolName": "HIBUS",
    "protocolVersion": 90,
    "hostName": "localhost",
    "appName": "cn.fmsoft.hybridos.inetd",
    "runnerName": "self",
    "signature": "...",
    "encodedIn": [ "base64" | "hex" ]
}
```

服务器端将根据客户端发送的身份验证数据包，根据指定应用安装到系统中时提供的公钥执行验签操作，若通过，则给客户端发送如下的验证通过数据包：

```json
{
    "packetType": "authPassed",
    "serverHostName": "localhost",
    "reassignedHostName": "localhost",
}
```

其中，
- `serverHostName`：返回服务器的正规主机名称。
- `reassignedHostName`：用于返回重新分配的主机名称，或者根据名称反向解析服务获得的正规主机名。

如果验证未通过，服务器将发送如下验证失败数据包，然后断开连接：

```json
{
    "packetType": "authFailed",
    "retCode": 409,
    "retMsg": "Conflict",
}
```

其中，
- `retCode`：用于验证失败的错误码，可能的取值有（来自 HTTP 状态码）：
    + 400（Bad Request）：数据包格式错误。
    + 401（Unauthorized）：验签失败。
    + 404（Not Found）：系统中未安装指定应用。
    + 406（Not Acceptable）：无效的主机名、应用名或行者名。
    + 409（Conflict）：行者名冲突。
    + 426（Upgrade Required）：客户端版本过低，需要升级。
- `retMsg`：包含简单的返回信息。

在身份验证失败的情况下，服务器将立即断开连接。
在身份验证未通过之前，客户端发送到服务器所有非用于身份验证的数据包，都将被忽略并致使服务器立即断开连接。

注：在单一应用场景下，由于不安装第三方应用，所以应用的身份验签可以做简化处理；而在多应用场景下，需要一个额外的应用管理器配合身份验证。

#### 过程调用请求

发起执行特定过程（procedure）的请求时，向 hiBus 服务器写入如下 JSON 数据：

```json
{
    "packetType": "call",
    "callId": "<unique_call_identifier>",
    "toEndpoint": "@<host_name>/<app_name>/<runner_name>",
    "toMethod": "<method_name>",
    "expectedTime": 30000,
    "authenInfo": {
        ...
    },
    "parameter": "...",
}
```

其中，
- `packetType` 表示数据包类型，这里用 `call`，表示过程调用。
- `callId` 是调用方提供的一个唯一性字符串，用于在调用方标识特定的调用请求。
- `toEndpoint` 是目标端点名称，包括主机名、应用名称、行者名称。
- `toMethod` 是要调用的方法名称。
- `expectedTime` 是期望的执行时间，毫秒为单位，为 0 表示不限（内部上限可在运行时配置，一般取 30s）。
- `authenInfo` 是可选的用户身份验证信息（如果该过程需要额外的用户身份验证的话）。当前版本暂不考虑。
- `parameter` 是该过程的执行参数。注意，执行参数以及返回值使用 JSON 表达，但转为字符串传递。由方法处理器和调用者负责解析。

服务器在处理 `call` 数据包时，若遇错误，则会返回 `error` 数据包给调用者：

```json
{
    "packetType": "error",
    "protocolName": "HIBUS",
    "protocolVersion": 90,
    "causedBy": "call",
    "causedId": "<unique_call_identifier>",
    "retCode": 503,
    "retMsg": "Service Unavailable",
}
```

其中，
- `causedBy` 为 `call`；
- `causedId` 为 `call` 数据包中传来的调用标识符。
- `retCode` 的可能取值有：
   - 400 Bad Request：表示 JSON 格式解析错误。
   - 401 Unauthorized：表示需要验证用户身份，但未提供。
   - 403 Forbidden：表示没有权限调用该过程。
   - 404 Not Found：表示请求的过程无效。
   - 405 Method Not Allowed：表示不允许调用者调用该方法。
   - 406 Not Acceptable：表示请求参数不被接受。
   - 409 Confilct：表示冲突；比如重复注册过程或事件时。
   - 423 Locked：表示被锁定；比如要撤销的过程尚未结束某个执行。
   - 500 Internal Server Error：表示服务器内部错误。
   - 501 Not Implemented：表示未实现。
   - 502 Bad Gateway：表示执行该调用的过程发生异常。
   - 503 Service Unavailable：表示服务不可用。
   - 504 Gateway Timeout：表示执行该调用的过程超时。
   - 507 Insufficient Storage：表示遇到内存或存储不足的问题。

#### 过程调用结果

hiBus 服务器会首先将过程调用请求转发给过程端点，根据过程处理器的处理结果返回结果标识符、请求标识符、状态码以及可能的结果给调用者。相应的数据包格式如下：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@<host_name>/<app_name>/<runner_name>",
    "fromMethod": "<method_name>"
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": "...",
}
```

其中，
- `packetType` 表示数据包类型，这里用 `result`，表示过程调用结果。
- `resultId` 是 hiBus 服务器为每个过程调用分配的一个全局唯一字符串，用于跟踪特定的调用。
- `callId` 是调用方发起过程调用时提供的标识符。
- `fromEndpoint` 表示处理该调用端点名称，包括主机名称、应用名称及行者名称。仅在 `retCode` 为 200 时存在该字段。
- `fromMethod` 表示处理该调用的方法名称。仅在 `retCode` 为 200 时存在该字段。
- `timeConsumed` 是方法处理器实际消耗的事件；单位秒，浮点数。仅在 `retCode` 为 200 时存在该字段。
- `timeDiff` 是自收到原始请求到返回该结果的时间差；单位秒，浮点数。
- `retCode` 取 HTTP 状态码子集，可取如下值：
   - 200 Ok：表示过程正常执行并返回了结果。
   - 202 Accepted：表示已提交参数给过程，但该过程可能耗时较长，所以暂时没有返回结果。
- `retMsg`：对 `retCode` 的可读简短描述。
- `retValue` 包含过程调用的返回值。只有 `retCode` 为 200 时，返回的数据中才包含有结果数据。注意，执行参数以及返回值使用 JSON 表达，但转为字符串传递。由方法处理器和调用者负责解析。

在正常情况下，调用者会首先收到 202 状态码，然后在未来的某个时间点收到 200 状态码。只有状态码为 200 时，才表示过程执行完毕并包含有返回值（`retValue`）信息。

通常，当 `retCode` 为 202 时，调用方应根据 `resultId` 来监视后续从服务器返回的数据包，从而获得最终的执行状态或结果。

注意，过程处理器返回给服务器的结果数据包格式，和上述数据包类似，但缺少 `fromEndpoint` 以及 `timeDiff` 等字段，但包含 `toEndpoint` 和 `timeConsumed` 等字段，表示过程处理器执行该调用消耗的时间。

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromMethod": "<method_name>",
    "timeConsumed": 1.2345,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": "...",
}
```

服务器在成功转发 `result` 数据包后，会返回 `resultSent` 数据包给过程处理器：

```json
{
    "packetType": "resultSent",
    "resultId": "<unique_event_identifier>",
    "timeDiff": <time_consumed_to_send_event>,
}
```

其中，
- `resultId`：标识结果的一个全局唯一字符串。
- `timeDiff`：自收到结果数据包到执行转发所经过的时间，浮点数，单位秒。

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
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@<host_name>/<app_name>/<runner_name>",
    "toMethod": "<method_name>",
    "timeDiff": 0.5432,
    "authenInfo": {
        ...
    },
    "parameter": "...",
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
    "eventId": "<unique_event_identifier>",
    "bubbleName": "<bubble_name>",
    "bubbleData": "...",
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId`：事件发生器为每个事件分配的一个全局唯一字符串。
- `bubbleName`：事件泡泡名称。
- `bubbleData` 包含真正的事件泡泡数据。注意，泡泡数据可使用 JSON 表达，但转为字符串传递，由事件发生器和接收器负责解析。

注意，由于事件发生器所在的行者连接对应了其所在主机、应用以及行者，故而在产生的事件数据包中无需指定这些信息。

服务器在成功处理 `event` 数据包后，会返回 `eventSent` 数据包给事件发生器：

```json
{
    "packetType": "eventSent",
    "eventId": "<unique_event_identifier>",
    "nrSucceeded": <number_of_succeeded_subscribers>,
    "nrFailed": <number_of_failed_subscribers>,
    "timeDiff": <time_consumed_to_send_event>,
    "timeConsumed": <time_consumed_to_send_event>
}
```

其中，
- `eventId`：标识事件的一个全局唯一字符串。
- `nrSucceeded`：转发成功的订阅者数量。
- `nrFailed`：转发失败的订阅者数量。
- `timeDiff`：自收到事件数据包到开始转发事件所耗时间，浮点数，单位秒。
- `timeConsumed`：转发事件所耗时间，浮点数，单位秒。

服务器在处理 `event` 数据包时，若遇错误，则会返回 `error` 数据包给事件发生器：

```json
{
    "packetType": "error",
    "protocolName": "HIBUS",
    "protocolVersion": 90,
    "causedBy": "event",
    "causedId": "<unique_event_identifier>",
    "retCode": 503,
    "retMsg": "Service Unavailable",
}
```

其中，
- `causedBy` 为 `event`；
- `causedId` 为 `event` 数据包中传来的事件标识符。
- `retCode` 的可能取值有：
   - 400 Bad Request：表示 `event` 数据包格式错误。
   - 404 Not Found：表示无效的事件泡泡名。
   - 500 Internal Server Error：表示服务器内部错误。
   - 501 Not Implemented：表示未实现。
   - 502 Bad Gateway：表示执行该调用的过程发生异常。
   - 503 Service Unavailable：表示服务不可用。
   - 504 Gateway Timeout：表示执行该调用的过程超时。
   - 507 Insufficient Storage：表示遇到内存或存储不足的问题。

#### 收到事件

当服务器收到一个新的事件后，将把该事件转发给所有的订阅者。事件订阅者将从服务器获得该事件数据。收到的事件数据包通常具有如下格式：

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "timeDiff": 0.1234,
    "fromEndpoint": "@<host_name>/<app_name>/<runner_name>",
    "fromBubble": "<bubble_name>",
    "bubbleData": {
        ...
    }
}
```

其中，
- `packetType` 表示数据包类型，这里用 `event`，表示这是一个事件。
- `eventId` 是事件发生器为每个事件分配的一个全局唯一字符串。
- `timeDiff` 表示服务器收到该事件到转发该事件的时间差；单位秒，浮点数。
- `fromEndpoint` 表示发生该事件的端点名称，包括主机名称、应用名称和行者名称。
- `fromBubble` 是事件的泡泡名称。
- `bubbleData` 包含真正的事件泡泡数据。注意，泡泡数据可使用 JSON 表达，但转为字符串传递，由事件发生器和接收器负责解析。

#### 乒乓心跳

一般而言，在套接字连接上使用乒乓心跳可以有效检测连接的意外丢失。WebSocket 规范规定了心跳的处理机制，和 WebSocket 类似，在 UnixSocket 的连接中，服务器和客户端之间，也将通过类似 WebSocket 的机制来处理乒乓心跳。这种处理由底层的通讯协议进行，上层无须关系。

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
    "callId": "<unique_call_identifier>",
    "toEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "toMethod": "registerProcedure",
    "expectedTime": 30000,
    "authenInfo": null,
    "parameter": "{ \"methodName\": \"foo\", \"forHost\": \"localhost\", \"forApp\": \"*\" }"
}
```

成功情形下的结果数据包：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "registerProcedure",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": ""
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
   + `endpointName`：事件所属的行者名称，含主机名、应用名以及行者名。
   + `bubbleName`：要订阅的泡泡名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否订阅成功，可能的值有：
   + `404`：表示未找到指定的事件。
   + `403`：表示事件发生器不允许调用方订阅该事件。
   + `200`：表示成功。

#### 取消事件订阅

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/unsubscribeEvent`
- 参数：
   + `endpointName`：事件所属的行者名称，含主机名、应用名以及行者名。
   + `bubbleName`：要订阅的泡泡名。
- 返回值：无。客户端依据结果的 `retCode` 判断是否取消成功，可能的值有：
   + `404`：表示调用方未订阅指定的事件；或者该事件已经被撤销。
   + `200`：表示成功。

#### 列出已连接端点

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listEndpoints`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可调用的过程清单。
- 常见错误状态码：403（Forbidden）。
- 仅允许 `cn.fmsoft.hybridos.hibus` 应用调用。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "listEndpoints",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": [
        {
            "endpointName": "localhost/cn.fmsoft.hybridos.hibus/builtin",
            "livingSeconds": 50,
            "methods": ["foo", "bar"],
            "bubbles": ["FOO", "BAR"],
            "memUsed": 4526,
            "peakMemUsed": 4526
        },
        {
            "endpointName": "localhost/cn.fmsoft.hybridos.hibus/cmdline",
            "livingSeconds": 20,
            "methods": ["foo", "bar"],
            "bubbles": ["FOO", "BAR"],
            "memUsed": 4526,
            "peakMemUsed": 4526
        },
    ],
}
```

注意：`retValue` 始终为 JSON 格式的字符串。

#### 列出已注册过程

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listProcedures`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可调用的过程清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "listProcedures",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": [
        "localhost/cn.fmsoft.hybridos.inetd/getHotSpots",
        "localhost/cn.fmsoft.hybridos.inetd/connectToHotSpot",
    ],
}
```

注意：`retValue` 始终为 JSON 格式的字符串。

#### 列出已注册事件

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listEvents`
- 参数：无。
- 返回值：成功时返回已注册的，且调用方可订阅的事件清单。
- 常见错误状态码：无

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "listEvents",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": [
        "@localhost/cn.fmsoft.hybridos.inetd/NETWORKCHANGED",
        "@localhost/cn.fmsoft.hybridos.inetd/SIGNALCHANGED",
    ],
}
```

注意：`retValue` 始终为 JSON 格式的字符串。

#### 列出事件的订阅者

- 过程名称：`@localhost/cn.fmsoft.hybridos.hibus/builtin/listEventSubscribers`
- 参数：
   + `endpointName`：事件所属的行者名称，含主机名、应用名以及行者名。
   + `bubbleName`：要订阅的泡泡名。
- 返回值：成功时返回指定事件的订阅者的主机名及应用名清单。
- 常见错误状态码：
   + `404`：表示未找到指定的事件。
   + `403`：表示调用方无权查询指定事件的订阅者。
   + `200`：表示成功。

下面是一个示例结果：

```json
{
    "packetType": "result",
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "listEventSubscribers",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": [
        "@localhost/cn.fmsoft.hybridos.foo/first",
        "@localhost/cn.fmsoft.hybridos.bar/second",
    ],
}
```

注意：`retValue` 始终为 JSON 格式的字符串。

#### 回声

该过程主要用于测试。

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
    "resultId": "<unique_result_identifier>",
    "callId": "<unique_call_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromMethod": "echo",
    "timeConsumed": 0.5432,
    "timeDiff": 0.1234,
    "retCode": 200,
    "retMsg": "Ok",
    "retValue": "I am still live",
}
```

### hiBus 内置事件

hiBus 服务器通过 `builtin` 行者产生内置事件。

#### 新行者事件

当一个新的行者成功连入 hibus 服务器时，产生 `NEWENDPOINT` 事件：

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromBubble": "NEWENDPOINT",
    "bubbleData": {
        "endpointType": [ "web" | "unix" ],
        "endpointName": "<the_endpoint_name>",
        "peerInfo": [ <PID_if_type_is_unix> | "IP_address_if_type_is_web"],
        "totalEndpoints": <the_number_of_total_endpoints>
    }
}
```

其中：
- `endpointType` 取 `web` 或者 `unix`，对应于 WebSocket 连接或者 UnixSocket 连接。
- `endpointName` 是包含主机名、应用名以及行者名的端点名称。
- `peerInfo` 是行者信息；对 WebSocket 连接，为 IP 地址（字符串）；对 UnixSocket 连接，为 PID（整数）。
- `totalEndpoints` 是整数，表示当前端点数量。

注意：
- `bubbleData` 将以 JSON 字符串的形式传递，避免在服务器端做额外的解析。
- 该事件仅允许本机中的 `cn.fmsoft.hybridos.*` 应用订阅。

#### 行者断开事件

当一个行者因为丢失连接或者长时间无响应而移除时，产生 `BROKENENDPOINT` 事件：

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromBubble": "BROKENENDPOINT",
    "bubbleData": {
        "endpointType": [ "web" | "unix" ],
        "endpointName": "<the_endpoint_name>",
        "brokenReason": [ "lostConnection" | "notResponding" ],
        "totalEndpoints": <the_number_of_total_endpoints>
    }
}
```

其中：
- `endpointType` 取 `web` 或者 `unix`，对应于 WebSocket 连接或者 UnixSocket 连接。
- `endpointName` 是包含主机名、应用名以及行者名的端点名称。
- `borkenReason` 是行者的断开原因，取 `lostConnection` 和 `notResponding` 两个值之一。
- `totalEndpoints` 是整数，表示当前端点数量。

注意：
- `bubbleData` 将以 JSON 字符串的形式传递，避免在服务器端做额外的解析。
- 该事件仅允许本机中的 `cn.fmsoft.hybridos.*` 应用订阅。

#### 丢失事件发生器事件

当某个行者订阅了某个事件，但产生该事件的行者意外断开时，将向订阅者发送 `LOSTEVENTGENERATOR` 事件：

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromBubble": "LOSTEVENTGENERATOR",
    "bubbleData": {
        "endpointName": "<the_endpoint_name>",
    }
}
```

其中 `bubbleData` 中的各参数说明如下：
- `endpointName` 是包含主机名、应用名以及行者名的端点名称。

注：不可订阅。

#### 撤销泡泡事件

当某个行者订阅了某个事件，但事件发生器撤销该事件时，将向订阅者发送 `LOSTBUBBLE` 事件：

```json
{
    "packetType": "event",
    "eventId": "<unique_event_identifier>",
    "fromEndpoint": "@localhost/cn.fmsoft.hybridos.hibus/builtin",
    "fromBubble": "LOSTBUBBLE",
    "bubbleData": {
        "endpointName": "<the_endpoint_name>",
        "bubbleName": "<the_bubble_name>",
    }
}
```

其中 `bubbleData` 中的各参数说明如下：
- `endpointName` 是包含主机名、应用名以及行者名的端点名称。
- `bubbleName` 泡泡名称。

注：不可订阅。

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
|                    WebSocket Connection Manager                    |
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
|                     UnixSocket Connection Manager                  |
 --------------------------------------------------------------------
                                   |
                                   | UnixSocket connection
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

如上图所示，App 通过 WebSocket 或者 UnixSocket 连接到 hiBus 服务器，每个应用可包含若干行者，可用于过程处理，产生事件，或者处理事件，发起过程调用。

其中，hiBus 命令行（command line）作为 `cn.fmsoft.hybridos.hibus` 应用的外挂行者实现，可用于系统管理员查看或者开发者调试使用。

hiBus 服务器主要包含如下软件模块：

- WebSocket/UnixSocket 连接管理器：该模块用于监听套接字端口，接受连接请求，检测连接是否已断开等。
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
int hibus_connect_via_unix_socket (const char* path_to_socket,
        const char* app_name, const char* runner_name, hibus_conn** conn);
int hibus_connect_via_web_socket (const char* host_name, int port,
        const char* app_name, const char* runner_name, hibus_conn** conn);
```

上面的两个函数，分别使用 UnixSocket 或者 WebSocket 连接到 hiBus 服务器上。函数的返回值为套接字文件描述符（fd）。`fd >= 0` 时表明连接成功，此时会通过 `conn` 参数返回一个匿名的 `hibus_conn` 结构指针。`fd < 0` 时表明连接失败，其绝对值标识错误编码。

若连接成功，其后所有的接口均使用通过 `conn` 返回的结构指针来标识该连接。

使用如下接口关闭到 hiBus 服务器的连接：

```c
int hibus_disconnect (hibus_conn* conn);
```

使用如下接口从 `hibus_conn` 结构中获得相关信息（主机名、应用名、行者名、套接字文件描述符、套接字类型等）：

```c
const char* hibus_conn_srv_host_name (hibus_conn* conn);
const char* hibus_conn_own_host_name (hibus_conn* conn);
const char* hibus_conn_app_name (hibus_conn* conn);
const char* hibus_conn_runner_name (hibus_conn* conn);
int hibus_conn_socket_fd (hibus_conn* conn);
int hibus_conn_socket_type (hibus_conn* conn);
```

#### 数据包读写函数

客户端可直接使用如下函数读取一个数据包：

```c
#define MAX_LEN_PAYLOAD     4096
int hibus_read_packet (hibus_conn* conn, void* packet_buf, unsigned int *packet_len);
void* hibus_read_packet_alloc (hibus_conn* conn, unsigned int *packet_len);

int hibus_send_text_packet (hibus_conn* conn, const char* text, unsigned int txt_len);
```

和 WebSocket 类似，在数据包长度超过 4096 字节时，通过 UnixSocket 发出的数据包也会被分成较小的数据帧（frame）来传输。每个数据帧的负载（payload）大小被限定为 4096 字节，这些函数将自动处理数据包的分片发送或者读取。也会自动处理乒乓心跳数据帧。

注意，通常客户端不需要直接调用这几个底层的读写数据包函数。这些函数供 Python、JavaScript 等编程语言实现本地绑定功能时使用。另外，这些函数全部使用阻塞读写模式，故而在调用这些函数，尤其是读取函数之前，应通过 `select` 系统调用判断对应的文件描述符上是否存在相应的可读取数据。

#### 辅助函数

行者可使用如下辅助函数来解析端点名称：

```c
#define LEN_HOST_NAME       127
#define LEN_APP_NAME        127
#define LEN_RUNNER_NAME     64
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
int hibus_assemble_endpoint (const char* host_name, const char* app_name,
        const char* runner_name, char* buff);

char* hibus_assemble_endpoint_alloc (const char* host_name, const char* app_name,
        const char* runner_name);
```

#### 过程管理

行者可以使用如下的接口注册或撤销过程：

```c
typedef char* (*hibus_method_handler)(hibus_conn* conn,
        const char* from_endpoint, const char* method_name,
        const char* method_param);

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
        const char* bubble_name, const char* bubble_data);
```

`hibus_register_event` 函数用于注册一个事件。

`hibus_revoke_event` 函数用于撤销一个事件。

`hibus_fire_event` 用于产生一个事件。

#### 订阅事件

模块可以使用如下的接口订阅或者取消订阅一个事件：

```c
typedef void (*hibus_event_handler)(hibus_conn* conn,
        const char* from_endpoint, const char* bubble_name,
        const char* bubble_data);

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
        int ret_code, const char* ret_value);

int hibus_call_procedure (hibus_conn* conn, const char* endpoint, const char* method_name,
        const char* method_praram, time_t ret_time_expected, hibus_result_handler result_handler);

int hibus_call_procedure_and_wait (hibus_conn* conn, const char* endpoint, const char* method_name,
        const char* method_praram, time_t ret_time_expected, char** ret_value);
```

`hibus_call_procedure` 提供了异步调用远程过程的接口：发起调用后会立即返回，然后在回调函数（`result_handler`）中等待结果或错误。此时，需要配合后面讲到的 `hibus_wait_for_packet` 函数使用。

`hibus_call_procedure_and_wait` 提供了同步调用远程过程的接口：发起调用后将等待结果或错误然后返回。注意，在等待返回值的过程中，可能会收到事件，此时，该函数会调用相应的事件处理器。

#### 等待并分发数据包

```c
int hibus_wait_and_dispatch_packet (hibus_conn* conn, struct timeval *timeout);
```

该函数检查服务器发过来的数据，并调用相应的事件处理器或者结果处理器。通常在一个循环中使用，如：

```c
    struct timeval timeout = { 0, 100000 }; // 100ms

    while (1) {

        ...

        hibus_wait_and_dispatch_packet (conn, &timeout);

        ...
    }
```

## 其他

### 简单的应用管理

在 HybridOS 系统中，应用包管理类似 Android 系统：

- 每个应用被安装在独立的目录中，且每个应用对应一个唯一的用户账号，通常使用和应用名称一样的用户账号名称（需要将应用名中的 `.` 字符调整为 `-` 字符）。
- 每个应用所在目录中包含有一对非对称加密算法使用的私钥。在安装该应用时，对应的公钥应被复制到系统目录（如 `/etc/public-keys/`）中，可用于验证签名。
- 检查应用是否安装好，只要检查系统目录中是否有对应的应用即可。
- 对具有需要超级用户权限的应用，将其对应的用户名加入到 `admin` 组中。

在没有应用管理器的情况下，使用系统管理员账号（root）完成应用目录的创建、用户名的创建以及公钥的复制。

### 端点权限管理

一个应用是否可以调用某个特定的过程或者订阅某个特定的事件，可以通过 `forHost` 和 `forApp` 两个参数来指定。

可使用如下的语法来使用通配符（`*` 和 `?`）指定多个主机，亦可用逗号（`,`）指定多个匹配项，称为模式（pattern）列表。如：

```
    cn.fmsoft.hybridos.*, cn.fmsoft.app.*, com.example.a??
```

另外，还支持如下特定的模式指定方法：

1. 使用 `$` 前缀时，视作变量，将使用上下文信息替代相应的变量。目前支持两个变量：
   - `$self`：表示该过程或事件的创建者所在主机名。
   - `$owner`：表示创建该过程或者事件的应用名。
1. 使用 `!` 前缀时，表示排除给定的匹配模式。

如下面的模式列表，用于匹配创建当前方法或者事件的应用，以及 hibus 应用本身：

```
    $owner, cn.fmsoft.hybridos.hibus
```

如下模式列表，用于匹配所有不以 `cn.fmsoft.hybridos.` 打头的应用：

```
    !cn.fmsoft.hybridos.*, *
```

### 跨设备连接的思考

注：当前版本仅处理本机上的连接。

跨设备（主机）连接有两种思路：

1. hiBus 服务器使用 WebSocket 在非回环地址上监听连接请求，另一个主机上的应用通过 WebSocket 直接连接到该服务器。问题：如何对另一台主机上的应用进行身份验证？
1. 在不同的 hiBus 服务器实例之间建立中继服务，所有针对另一个主机上的请求，由 hiBus 服务器之间通过中继完成，从而实现跨主机的远程过程调用或者事件订阅。此种设计下，身份验证只在 hiBus 服务器之间进行，各主机上的应用身份验证，由本机处理。这样的话，本机 WebSocket 的事件和过程调用端口均只在本地回环地址上提供服务。比如，当过程调用的目标主机非本机时，hiBus 服务器可将该请求转发给目标主机所在的 hiBus 服务器，然后将结果转发给调用者。

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

