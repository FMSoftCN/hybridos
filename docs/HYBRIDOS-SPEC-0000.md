# HybridOS Architecture

HybridOS Specification 0000<br/>
Author: Vincent Wei<br/>
Category: System Architecture<br/>
Date: November 2018<br/>
Status: Proposal

Copyright Notice

  Copyright (C) 2018 Beijing FMSoft Technologies. All Rights Reserved.

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
    * ...
1. HybridOS services such as
    * WebSocket server
    * HTTP Responder (Python-based)
    * MQTT Broker (Python-based)
    * ...

#### HybridOS WebSocket Server

HybridOS WebSocket Server is an general-purpose implementation of WebSocket server. 
The other process can connect to the server via Unix Socket, and exchange the data
with the remote clients. 

Indeed, HybridOS WebSocket server acts as a broker between the remote clients
and the real local service providers.

Currently, HybridOS WebSocket Server can 

  * act as a remote display of HybridOS app. The user can interact with a HybridOS app
    in a HTML5 browser, but the HybridOS app is running in the device actually.
  * act as a H.264 streaming server.
  * act as a text-based command responder.

You (the developer) can write an app or a service to connect to the
HybridOS WebSocket Server via UNIX socket and act as the real service provider.

In this way, you do not need to write a WebSocket server to handle the details
of the WebSocket protocol, you only need to write your own application logic
to provide the services.

The HybridOS HTTP Responder also runs in such manner.

#### HybridOS HTTP Responder

#### HybridOS MQTT Broker

### Libraries

In addition to the standard glibc, HybridOS integrates the following third-party
libraries:

1. Common libraries, such as LibXML2, libZ, LibPNG, LibJPEG, ...
1. SQLite
1. FreeType
1. MiniGUI
1. CURL3
1. V8 Engine
1. HFCL (HybridOS Foundation Class Library)
1. ...

## Cloud Side

### MQTT Broker

### Identity Authentication

## Client Side

### Plugins for HTML5 Web Browser

### Native Container

