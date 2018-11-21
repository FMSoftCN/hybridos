# HybridOS Architecture

HybridOS Specification 0000  
Author: Vincent Wei  
Category: System Architecture  
Date: November 2018  
Status: Proposal

*Copyright Notice*

Copyright (C) 2018 Beijing FMSoft Technologies.  
All Rights Reserved.

## Introduction

As we know, HybridOS tries to provide the developers with more possibilities
than just a traditional operating system for a stand-alone hardware environment.

HybridOS not only runs on smart IoT devices to support application development
on devices, it also provides programming interfaces for the cloud and the clients,
providing developers with a new complete software stack and protocol stack 
from devices to the cloud and the clients.

We will describe the architecture and the key components of HybridOS in this document.

## Device Side

### System Components

In this document, we call a software which runs as a process as a `component`. Generally,
most of components are daemons.

HybridOS integrates the following components:

1. Linux kernel.
1. Basic runtime utilities such as
    * Busybox
1. General system services such as
    * procd
    * hBUS
    * ntpd
    * netifd (Network Interface Daemon)
    * Security service
    * ...
1. HybridOS services such as
    * WebSocket Server
    * HTTP Server
    * CoAP Server
    * Streaming Server
    * MQTT Broker
    * ...

### Services

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

  * act as a remote display of HybridOS app. The user can interact with a HybridOS app
    in a HTML5 browser, but the HybridOS app is running in the device actually.
  * act as a simple H.264 streaming server.
  * act as a text-based command responder.

#### HybridOS HTTP Server

HybridOS HTTP Server is a simple implementation of HTTP 2.0. Your app or service can 
can register as a request responder at a specific endpoint. All requests sent to
the endpoint will be passed by the server to your app or service. 

The HybridOS HTTP Server can

  * provide a webpage for the system settings, like what a network router does.
  * provide a webpage for a specific app/service, which shows the current work status.

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

### Libraries

In addition to the standard glibc, HybridOS integrates the following third-party
libraries:

1. Common libraries, such as LibXML2, libZ, LibPNG, LibJPEG, ...
1. SQLite
1. FreeType
1. MiniGUI
1. CURL
1. V8 Engine
1. HFCL (HybridOS Foundation Class Library)
1. Python 3 runtime environment
1. ...

## Client Side

### Native Container

### JS Library for HTML5 Web Browser

## Cloud Side

### Distributed MQTT Broker

### Identity Authentication


[HybridOS Architecture]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0000.md
[HybridOS App Framework]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0001.md
[HybridOS Foundation Class Library]: https://github.com/VincentWei/hybridos/blob/master/docs/HYBRIDOS-SPEC-0002.md
