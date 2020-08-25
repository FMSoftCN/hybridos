HybridOS Specification No. 00  
Topic: HybridOS Architecture  
Author: Vincent Wei  
Category: System Architecture  
Date: November 2018  
Status: Proposal

*Copyright Notice*

Copyright (C) 2018, 2019 [FMSoft Technologies]  
All Rights Reserved.

## Introduction

As we know, HybridOS tries to provide the developers with more possibilities
than just a traditional operating system for a stand-alone hardware environment.

HybridOS not only runs on smart IoT devices to support application development
on devices, but also provides programming interfaces for the cloud and the clients.
It tries to give the developers a new complete software stack and protocol stack
from devices to the cloud and the clients.

In this document, we describe the architecture and the key components of HybridOS,
and try to show you the whole picture of HybridOS and the key features.

## Device Side

### System components

In this document, we call a software which runs as a process as a `component`.
Generally, most of components are daemons.

HybridOS integrates the following components:

1. Linux or other POSIX-compliant kernel.
1. Basic runtime utilities such as
    * Busybox
1. General system services such as
    * procd
    * sntpd
    * udevd
    * hiBus (the HybridOS variant of uBus)
    * hiAppmand (the HybridOS App Management Daemon)
    * hiLogged (the HybridOS logging service)
    * hiSecurityd (the HybridOS security service)
    * hiNetifd (the HybridOS variant of Network Interface Daemon)
    * hiXinetd (the HybridOS variant of xinetd)
    * ...
1. HybridOS servers such as
    * WebSocket Server
    * HTTP Server
    * CoAP Server
    * Streaming Server
    * MQTT Broker
    * ...

### Daemons

The daemons in HybridOS are traditional ones like a Linux distribution.
Of course, they are compact ones. If you are familiar with OpenWRT, you
will find that some daemons are almost same ones of OpenWRT.

However, HybridOS integrates some specialized daemons for IoT applications:

  * The HybridOS logging service. This service provide a general-purpose
    logging infrastructure for IoT applications. Unlike a cloud server,
    this service stores log in a loop-buffer, and can feed the log via
    HTTP, CoAP, or MQTT, so a remote client can get the system log easily.
  * The HybridOS security service. This service acts a key role in HybridOS.
    This service provides the central access control mechanisms for other
    daemons and/or servers.

### Servers

IoT devices, in most cases, operate more like a server. For example, an IoT device
may run a HTTP server. The user can visit the device via standard HTTP by using
a web browser or an client app.

Therefore, HybridOS integrates common networking servers on device side. However,
HybridOS provides a different architecture for the implementation of the servers.
Any HybridOS app or service can act as a real service provider of the
server. On the other hand, a web server such as Apache/Nginx, which runs in the
cloud, does not provide this mechanism for developers.

Generally, you (the developer) can write an app or a service to connect to the
HybridOS Servers via a specific Unix socket and act as the real service provider.
The server handles the protocol details and passes the requests from a client
to a specific app or service, which has registered as a service provider at a
specific endpoint (a namespace actually). If the app or the service returns the
responses to the server, the server passes them to the remote client.

In this way, you do not need to write or run a complex HTTP server or a WebSocket
server to handle the details of the protocols, you only need to write a service
provider in your app or a service, register your endpoint to the server, and
return the responses for the requests.

Further, we can provide an abstract interface for various application protocols,
such as HTTP, CoAP, MQTT, so that a HybridOS app or service can only implement
a request provider once for different protocols.

We call a component which provides a specific services as a `service provider` or
a `request responder`.

However, unlike Apache or Nginx, these HybridOS servers are not designed for
large concurrency. But, security is our number one priority when designing
these servers.

Therefore, we introduce an universal security service to check the validity
and compliance for each connect request.

#### HybridOS WebSocket Server

HybridOS WebSocket Server is an general-purpose implementation of WebSocket server.
Your app or service can connect to the server via Unix Socket, and exchange the data
with the remote clients.

The HybridOS WebSocket Server can

* act as a remote display of HybridOS app. The user can interact with
  a HybridOS app in a HTML5 browser on client device, but the HybridOS app
  is running in the device actually.
* act as a simple H.264 streaming server.
* act as a text-based command responder.

#### HybridOS HTTP Server

HybridOS HTTP Server is a simple implementation of HTTP 2.0. Your app or service can
register as a request responder at a specific endpoint. All requests sent to
the endpoint will be passed by the server to your app or service.

The HybridOS HTTP Server can

* provide a webpage for the system settings, like what a network router does.
* provide a webpage for a specific app/service, which shows the current
  system status.

#### HybridOS CoAP Server

CoAP, the Constrained Application Protocol, is a specialized web transfer protocol
for use with constrained nodes and constrained networks in the Internet of Things.

Although the IoT device which can run HybridOS is not a constrained node, but
the other devices connected to the device powered by HybridOS are constrained nodes.
Therefore, HybridOS provides a CoAP server which can serves other nodes.

Again, the HybridOS CoAP Server provides the same mechanism as HybridOS HTTP Server.
You can implement the request responder of CoAP in your app or service, while
the server hides the details of the implementation of the protocol and the security.

#### HybridOS Streaming Server

HybridOS Streaming Server can encode a H.264/265 media stream in RTMP and push
the stream to the cloud. The server can also act as a streaming server to feed the
stream in RTMP or raw H.264/H.265 frames to a client which directly connects to
the device.

#### HybridOS MQTT Broker

HybridOS MQTT Broker is a simple implementation of MQTT broker and client.

For apps and services on the device, this server acts as a broker, so an app
or a service can connect to the server and provide subscription services for
other apps or services.

On the other hand, the broker can act as a client to the remote MQTT broker
which runs on cloud. In this way, client apps which runs on desktop computers,
smart phones, or tablets can exchange data via MQTT with an app or a service
running in the device, or a constrained node which connected to the device.

### System Software Diagram

The following chart shows the software diagram of HybridOS on device-side:

     -----------------------------------------------
    |                  HybridOS Apps                |
     -----------------------------------------------
    |                HybridOS Desktop               |
     -----------------------------------------------
    |      hiWebKit with a replacable JS engine     |____
     -----------------------------------------------     |
    |  MiniGUI, SQLite,       | Node.js runtime env.|____| (Node.js optinal)
    |  Cairo, EGL, OpenGLES2, |---------------------|    |
    |  FreeType, HarfBuzz,    | Python runtime env. |____| (Python is optinal)
    |  LibPNG, LibJPEG,       |---------------------|    |
    |  GStreamer, ...         |  C/C++ runtime env. |____|
     -----------------------------------------------     | hiBus
    |          HybridOS daemons/servers             |____|
     -----------------------------------------------
    |          LibZ, CURL, LibGcrypt, ...           | (infrastructure)
    |        HAL, C/C++ standard libraries          | (HAL: Hardware abstract layer)
     -----------------------------------------------
    |               Kernel/Drivers                  |
     -----------------------------------------------

As shown in the diagram, HybridOS provides two API sets:

1. Use [The WebKit Derivative for HybridOS](`hiWebKt` for short), to write
a HybridOS app in HVML/CSS/JavaScript, either the activities or the services.

1. When your device has a low hardware configuration, you can use a JavaScript
engine with low footprint. If your device has a high hardware configuration,
you use V8 or SquirrelFish JavaScript engine.

In addition to the standard C/C++ libraries, HybridOS integrates many
mature open-source libraries as the infrastructure, for example,
MiniGUI, FreeType, HarfBuzz, SQLite, V8 JS engine, and so on.

The following components act key roles in HybridOS for device-side:

1. [MiniGUI], provide a light-weight windowing and graphics user interface
system for HybridOS.

1. [hiWebKit], a WebKit derivative, provides support for
[HybridOS View Markup Language], as well as the popular Web standards
and specifications such as HTML 5.x and CSS 3. It also provides
support for customizing the view type defined by HVML.

1. [HybridOS Desktop] acts as the window/activity manager for HybridOS,
it provides the internal interfaces to create and manage
windows, activities, intents, services for your HybridOS apps.

1. [hiBus] provides a secure data bus mechanism for communication between
different modules of the system. One HybridOS app can send commands to
the specified server via hiBus, the server returns the execution result,
or HybridOS can listen for certain types of events, and the underlying
server or daemon sends events to the app via hiBus. Note that the request
responder in a HybridOS app sends the response data via Unix Domain socket
or other IPC mechanisms of underlying kernel instead of hiBus. This is mainly
due to the performance considerations.

## Client Side

On the client side, HybridOS provides the developers with some convenience,
making it easy to run the HybridOS app on various client platforms.

The key is [hiWebKit]. You know that [WebKit] can run on many platforms,
including macOS, Windows, iOS, Linux+GTK, and so on. As a derivative of
WebKit, hiWebKit inherits the feature. Therefore, HybridOS apps can
run on many client sides.

In other words, it is possible to write an app by using the universal
HybridOS API for IoT devices and various clients at the same time.

HybridOS app engine (`HAE` for short) is a wrapper of HFCL and hiWebKit,
which is a user agent of HVML (HybridOS View Markup Language) and
HTML 5 as well.

The developer can directly run a HybridOS app in HAE, just like visiting
a webpage in a web browser. Or you can wrap the app with HAE SDK as a
standalone app for a specific client platform.

## Cloud Side

We plan to address some of the common problems of IoT in the cloud
computing environment by providing some new cloud services.

  * Device certification and registration. This service can judge whether
    a specific device is genuine or not, and provides the registration
    and active statistics of the devices.
  * Online firmware upgrade. This service provides CDN distribution,
    download, and upgrade of the firmware for your device.
  * Online upgrade of an app or service. This service provides the
    online upgrade of a HybridOS app or service. It is somehow
    similar to the app store of smartphone.
  * MQTT service. This service providing a ready-to-use MQTT server.
  * Identity authentication service. This service provides
    a security implementation to bind a HybridOS device with a client app.
    It can also verify that an access request from an client to a certain
    device is validity.

In fact, some cloud services listed above are already very common on
smartphones. But they are basically provided by the smartphone
manufacturers and are not open. By using the services provided by HybridOS,
developers can easily implement these features for their IoT applications
without having to develop and maintain them themselves.

The HybridOS Security Service which runs on device side works closely with
the cloud services above to ensure the security of your IoT application.

The identity authentication mechanisms of HybridOS will learn some ideas
or concept from blockchain technology, in order to in order to avoid
hijacking device by cracker.

In the future, we will provide serverless computing on the cloud side.

[Beijing FMSoft Technologies Co., Ltd.]: https://www.fmsoft.cn
[FMSoft Technologies]: https://www.fmsoft.cn
[HybridOS Official Site]: https://hybridos.fmsoft.cn

[MiniGUI]: http:/www.minigui.com
[WebKit]: https://webkit.org
[HTML 5.3]: https://www.w3.org/TR/html53/
[DOM Specification]: https://dom.spec.whatwg.org/
[WebIDL Specification]: https://heycam.github.io/webidl/
[CSS 2.2]: https://www.w3.org/TR/CSS22/
[CSS Box Model Module Level 3]: https://www.w3.org/TR/css-box-3/

[HybridOS Architecture]: HybridOS-Architecture
[HybridOS Code and Development Convention]: HybridOS-Code-and-Development-Convention

