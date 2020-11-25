##### 零、讨论情境

为讨论方便，假设以下情境

```c
 --------------------------------------------------------------------------------- 
|                                 App: Interface GUI                              |
|--------------------------------------------------------------------------------- 
|          runner: battery check            |     runner: phone monitor           |
|Event subscribers   Procedure callers      |Event subscribers   Procedure callers|
 ---------------------------------------------------------------------------------
  |                    |                    |                  |
  |                    |                    |                  |
 ---------------------------------------------------------------------------------   hiBus
  |                    |                    |                  |
  | quantity msg       | shutdown func      | call in msg      | hung up func
 ---------------------------------------------------------------------------------
|Event bubbles      Procedure handlers      |Event bubbles      Procedure handlers|
|                battry daemon              |             GSM daemon              |
 ---------------------------------------------------------------------------------
```

假设系统中运行着三个用户进程：

1. GUI：用户界面进程。其中包含了两个runner：
   1. battery check：
      1. Event subscribers：订阅电池电量事件，要求1s一次，同时根据电量值，显示电池电量；
      2. Procedure callers：当电池电量小于某值时，调用电池守护进程的关机功能；
   2. phone monitor：
      1. Event subscribers：订阅来电事件，要求10ms一次。如果事件产生，从事件中获取来电号码并显示是否接听；
      2. Procedure callers：当用户选择拒绝，调用GSM守护进程的挂断功能。
2. battry daemon：power监控守护进程，仅有一个runner：
   1. power manager：
      1. Event bubbles：1s产生一次电量事件；
      2. Procedure handlers：关机功能。
3. GSM daemon：GSM守护进程，仅有一个runner：
   1. GSM manager：
      1. Event bubbles：来电事件，仅在有来电时发送；
      2. Procedure handlers：挂断功能。



现在理解：

1. 关于事件：

   1. 任何进程都可以产生任何事件。事件的eventId，由系统统一编码。不在统一编码中的事件将被hiBus认为非法；
   2. 产生的事件，无需特别操作，直接扔到hiBus总线上。也就是说电池电量、来电事件一旦发生直接送往hiBus，只要它们合法；
   3. 事件的接收（订阅），是要告之hiBus的。hiBus只将订阅的事件发往订阅的runner。

2. 关于过程调用：

   1. 任何进程都可以向hiBus申请过程调用，无需特别的操作。申请的调用是否被提供，是否合法，由hiBus判断；
   2. 过程调用的提供者，是需要向hiBus注册过程调用的。只有注册过的过程调用才为合法。因此关机、挂断两个功能是需要注册的；
   3. hashed_call_identifier如何确定？

3. 关于hiBus连接：

   GUI进程的hiBus连接有两种模型：

   1. 两个runner各自拥有各自的连接，也即有两个hiBus对象，hiBus的最小粒度为runner；
   2. 两个runner只有一个连接，也即有一个hiBus对象，hiBus的最小粒度为App。

下面的文字，基于下面的几个理解：

1. 产生的事件，无需特别操作，直接扔到hiBus总线上。事件的接收（订阅），是要告之hiBus的；

2. 任何进程都可以向hiBus申请过程调用，无需特别的操作。过程调用的提供者，是需要向hiBus注册过程调用的；

3. hiBus的最小粒度为runner。

   

##### 一、hiBus软件结构

如下图：

```c
 ---------------------------------------------------------------------------------
|                           User JS code (Java Script)                            |
 --------------------------------------------------------------------------------- 
|                        JS hiBus Framework (Java Script)                         |
 ---------------------------------------------------------------------------------
|                          JS hiBus Connection (C Code)                           |
 --------------------------------------------------------------------------------- 
|                   HybridOS servers and user daemons (hiBus daemon)              |
 ---------------------------------------------------------------------------------
|                          Python runtime environment (optional)                  |
|                             LibZ, CURL, LibGcrypt, ...                          |
|                               C/C++ runtime environment                         |
 ---------------------------------------------------------------------------------
|                               Linux Kernel/Drivers                              |
 ---------------------------------------------------------------------------------

```

其中：

- JS hiBus Framework，是一段封装好的JS代码，对外提供接口，供用户JS代码调用。其负责：
  - 根据用户传递的参数，建立到hiBus daemon的连接；
  - 保存用户注册的事件、方法的回调函数；
  - 提供对hiBus的写接口，并完成对写入数据的封装；
  - 以轮询方式定时检测与接收hiBus daemon发回的数据；
  - 解析数据，然后调用事件、方法对应的回调函数；
  - 将方法回调的结果，再次发送到hiBus daemon。
- JS hiBus Connection，完成与hiBus daemon通信，用c代码完成，其负责：
  - 建立到hiBus daemon的连接，完成连接、身份识别；
  - 完成对hiBus的写操作；
  - 完成对hiBus的读操作。

##### 二、JS hiBus Framework

###### 2.1 主要数据结构及注册过程

```c
                                                     Event List
                        |-------------------------------------------------------------------| 
     register-event     | id0 | host name | app name | runner name | bubble name | function |
 ---------------------->|-------------------------------------------------------------------|
                        | id1 | host name | app name | runner name | bubble name | function |  
                        |-------------------------------------------------------------------|
                        ZZ|                              .......                              |
    unregister-event    |-------------------------------------------------------------------|
 <----------------------| idn | host name | app name | runner name | bubble name | function |
                        |-------------------------------------------------------------------|
     
                                                       Method List
                        |-------------------------------------------------------------------|
     register-method    | id0 | host name | app name | runner name | method name | function |
 ---------------------->|-------------------------------------------------------------------|
                        | id1 | host name | app name | runner name | method name | function |  
                        |-------------------------------------------------------------------|
                        |                              .......                              |
   unregister-method    |-------------------------------------------------------------------|
 <----------------------| idn | host name | app name | runner name | method name | function |
                        |-------------------------------------------------------------------|                                                        
```

其中：

- register-event：注册一个接收事件，以及该事件的回调函数：
  - 如果host name、app name、runner name、bubble name、function与列表中的某节点一致，则认为该函数已经注册；
  - 如果host name、app name、runner name、bubble name、function至少有一项与列表中所有节点不同，则认为是一个新节点，插到列表结尾；
  - 如未达到列表最大长度，该函数返回idn；
  - 如已达到列表最大长度，该函数返回-1，表示插入失败；
  - 新插入的节点id从0开始，顺序计数；
  - id为-1，表示该节点为空；
  - 调用hiBus_event_subscribe()，订阅一个事件。

- unregister-event：删除一个接收事件：
  - 当参数为host name、app name、runner name、bubble name、function时，所有项都相同，视为所要删除的节点，将该节点的id设置为-1；
  - 当参数为id，则直接删除该id的节点，将该节点的id设置为-1；
  - id不存在，或者为-1，视为删除失败；
  - 调用hiBus_event_remove()，删除一个事件的订阅；
  - 该函数返回被删除节点的id，或者-1；

- register-method：注册一个回调过程，以及该实际执行的回调函数。具体规则同register-event函数；
- unregister-method：删除一个回调过程，具体规则同unregister-event。

###### 2.2 启动过程

Start_hiBus(host_name, app_name, runner_name, type, Interval);

1. 判断是否存在hiBus对象，如果存在，删除之（同一窗口页面跳转时，删除上一个页面的痕迹），删除Event List和Method List，跳转到步骤2；
2. 使用host_name, app_name, runner_name，创建hiBus对象，建立和hiBus daemon的连接，如连接失败，返回false，成功跳转到步骤3；
3. 如果type为TIMER_MODE，以Interval为参数，调用setInterval建立定时器，返回true；
4. 如果type为USER_INVOKE，返回true。

在hiBus的使用过程中，存在两种模式：

- TIMER_MODE模式：使用js定时器，自动定时获取hiBus的输入数据，并传递给hiBus处理函数；

- USER_INVOKE模式：用户根据自己的需求，在任何时刻都可以调用hiBus处理函数，框架本身不维护timer事件。

即使在TIMER_MODE，用户为了某些实时目的，也可以在代码中，直接调用hiBus处理函数。因此hiBus处理函数名称保留为：

```c
hiBus_Read_Handler();
```

###### 2.3 hiBus_Read_Handler过程

hiBus_Read_Handler();

1. 如果hiBus对象未定义，直接返回。如果有值，则跳转到步骤2；
2. 调用hiBus.read()方法，读取hiBus传递过来的数据。如果数据长度为0，则直接返回。否则则跳转到步骤3；
3. 根据hiBus协议，对Json格式的请求逐一解析：
   1. 如果是Event请求，则在Event List中搜索。搜索到后执行回调函数。如果未搜索到，则跳到下一个请求；
   2. 如果是Method请求，则在Method List中搜索。搜索到后执行回调函数，如果未搜索到，则跳到下一个请求。执行回调函数后，获得函数返回值，调用hiBus.write()方法，写入hiBus；
4. 执行完全部请求，函数返回，该函数没有返回值。

###### 2.4 hiBus写过程

hiBus_Write();

此步骤没有想好，对数据的封装是调用者处理，还是在此处处理，或者二者都处理。

###### 2.5 事件的订阅与取消

hiBus_event_subscribe();

hiBus_event_remove();

讨论后再写。

###### 2.6 过程的注册与取消

hiBus_procedure_register();

hiBus_procedure_unregister();

讨论后再写。

##### 三、hiBus connection

hiBus connection既为hiBus对象的c语言实现。

###### 3.1 创建hiBus对象

var hibus = new hiBus(host_name, app_name, runner_name);

其步骤为：

​	1、连接服务器

​		参数：host_name, app_name, runner_name, param；

​		param: 符合协议的json字符串，这个根据hiBus提供接口而定；

​		接收：hiBus 服务器返回的挑战码。

​	2、身份识别

​		对服务器发来的挑战码进行数字签名，并返回签名。

​		问题：签名算法？

​	3、获得连接

​		获得服务器返回值，判断是否连接成功。如果成功，返回hiBus对象指针，如果失败，返回NULL；



###### 3.2 hiBus写操作

视hiBus API而定。

###### 3.3 hiBus读操作

视hiBus API而定。





问题：

1、心跳的处理

2、多个hiBus对象如何使用同一个框架js代码；

3、同一个APP如果包含若干个页面，每个页面都是重新加载和解析的。当切换的时候，不同页面属于同一个runner，但是页面变了，hiBus如何处理。



