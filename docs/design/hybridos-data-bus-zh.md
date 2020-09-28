# 合璧操作系统设备端数据总线概要设计

魏永明

本文阐述合璧操作系统设备端数据总线（hiBus）的设计。

## 基本框架

下图给出了单一应用场景下的 HybridOS 设备侧系统架构图：

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

在 HybridOS 中，始终贯彻着一个重要的设计思想：数据驱动。而不论是单一应用场景还是多应用场景，
hiBus 将成为 HybridOS 连接 GUI 和底层功能模块的纽带。

hiBus 的基本思想来自于 OpenWRT 的 uBus，比如通过 JSON 格式传递数据。但相比 uBus，hiBus 具有如下重要的改进：

1. 提供两种类型的底层连接通道：本地的 Unix Domain Socket 和 Web Socket，以方便 Web App 通过 Web Socket 连接到 hiBus 上。
1. 分离事件和远程过程调用，使用不同的套接字，以解决模块同时提供事件和远程过程调用服务时出现的问题。
1. 提供基本的安全机制，以决定某个应用或者某个远程节点能否订阅（subscribe）特定的事件，以及能否调用（call）某个特定的方法。
1. 考虑到在未来，hiBus 可通过 Web Socket 向局域网中的其他 IoT 设备节点提供服务。因此，需要在事件订阅以及调用远程过程时使用包含主机或设备名称的统一资源标识符（URI）。

我们保留如下特别的主机名称、应用名称：

- `localhost`：指通过 Unix Domain Socket 或本机 IP 地址（127.0.0.1）连接到 hiBus 的本机模块。
- `cn.fmsoft.hybridos.hibus`：保留的应用名称，指 hiBus 本身，注册或注销事件或方法时，向该应用发送指定的过程调用。该应用也可提供一些一般性的系统操作命令以及事件。

不同的软件模块可扮演不同的角色或同时扮演多个角色：

1. 事件发生器（event generator）：通过套接字接口连接到 hiBus 服务器，然后向服务器注册应用名称以及事件名称。
1. 方法处理器（method handler）：通过套接字接口连接到 hiBus 服务器，然后向服务器注册应用名称以及方法名称。
1. 事件订阅者（event subscriber）：通过套接字接口连接到 hiBus 服务器，然后向服务器订阅指定的事件，之后将收到该事件发生器上新发生的事件。
1. 方法调用者（method caller）：通过套接字接口连接到 hiBus 服务器，然后向服务器发送过程调用请求，之后同步等待或者异步等待处理结果。

## 协议及接口

### 套接字接口

1. Unix Doman Socket 端口：
   - `/var/run/hibus.event`：事件
   - `/var/run/hibus.method`：过程调用
1. Web Socket 端口（HybridOS 保留 7000 ~ 7999 端口）：
   - `7700`：事件
   - `7701`：过程调用

### 协议

#### 调用指定过程

1) 发送过程调用请求

发送执行特定过程（或方法）的请求时，向过程调用端口写入如下 JSON 数据：

```json
{
    "requestId": "<hased_request_identifier>",
    "procedure": "localhost/<app_name>/<method_name>",
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
   - `requestId` 是由请求方提供的一个字符串，用于标识请求。
   - `procedure` 是过程名称，包括主机名、应用名称和方法名称。
   - `expectedTime` 是期望的执行时间，毫秒为单位，为 0 表示不限。
   - `authenInfo` 是可选的身份验证信息（如果该过程需要额外的身份验证的话）。
   - `parameter` 是该过程的执行参数。

2) 获取过程调用结果

hiBus 服务器会返回结果标识符、请求标识符、状态码以及可能的结果：

```json
{
    "resultId": "<hased_result_identifier>",
    "requestId": "<hased_request_identifier>",
    "retCode": 200,
    "result": {
        ...
    }
}
```

其中 `retCode` 取 HTTP 状态码子集，可取如下值：

   - 200 Ok：表示过程正常执行方法并返回了结果。
   - 202 Accepted：表示已提交方法及参数给过程，但该执行器可能耗时较长，所以暂时没有返回结果。
   - 400 Bad Request：表示 JSON 格式解析错误。
   - 401 Unauthorized：表示该方法需要验证用户身份，但未提供。
   - 403 Forbidden：表示没有权限调用该过程。
   - 404 Not Found：表示请求的过程无效。
   - 405 Method Not Allowed：表示请求参数不被允许。
   - 406 Not Acceptable：表示请求参数不被接受。
   - 500 Internal Server Error：表示服务器内部错误。
   - 501 Not Implemented：表示未实现。
   - 502 Bad Gateway：表示执行该调用的过程发生异常。
   - 503 Service Unavailable：表示服务不可用。
   - 504 Gateway Timeout：表示执行该调用的过程超时。
   - 507 Insufficient Storage：表示遇到内存或存储不足的问题。

只有 `retCode` 为 200 时，返回的数据中才包含有结果数据（`result`）。当 `retCode` 为 202 时，发起请求的一侧应根据 `resultId` 来监视后续的结果数据，从而获得最终的执行结果。

3) hiBus 内置过程

- `cn.fmsoft.hybridos.hibus/registerProcedure`：注册过程。
- `cn.fmsoft.hybridos.hibus/revokeProcedure`：撤销过程。
- `cn.fmsoft.hybridos.hibus/registerEvent`：注册事件。
- `cn.fmsoft.hybridos.hibus/revokeEvent`：撤销事件。
- `cn.fmsoft.hybridos.hibus/makeObservation`：建立事件观察。
- `cn.fmsoft.hybridos.hibus/cancelObservation`：取消事件观察。

4) 接收事件

建立事件观察之后，从事件端口读入数据即可获得相应的事件数据（JSON）。事件数据通常具有如下格式：

```json
{
    "eventId": "<hased_event_identifier>",
    "source": "localhost/<app_name>/<event_name>",
    "payload": {
        ...
    }
}
```

### C 语言接口

## 安全性

## 演进

