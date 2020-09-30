# HybridOS Device Side

This directory contains the building scripts, the source of daemons and
servers, and the samples for device side of HybridOS.

```
    `
    |-- build/          # The building scripts.
    |-- daemons/        # The source for HybridOS daemons/servers.
    |-- samples/        # The source for HybridOS device-side samples.
    |-- thirdparty/     # The source for the third party librairies and tools.
```

## Building

We recommend that you use a latest Linux distribution with long term support,
for example, Ubuntu Linux 18.04 LTS.

Note that:

1. You need to reserve at least 30GB of disk space for downloading
   the source code and building the entire system.
1. Depending on your network connection speed (especially the speed of
   accessing GitHub) and the hardware configuration of your computer,
   it may take 2 hours or more to download the source code and build
   the system. **Please be patient.**

Building steps:

1) Clone this repository to your local system:

```
$ cd </path/to/your/working/directory/>
$ git clone https://github.com/FMSoftCN/hybridos.git
```

2) Install dependencies

```
$ cd hybridos/device-side/
$ build/hbd/install-dependencies
```

3) Build the system

```
$ build/Scripts/build
```

4) Build hiview renderer

```
$ git clone https://github.com/FMSoftCN/hiviewrenderer.git
$ cd watchface
$ make && sudo make install
$ cd ..

$ cd stopwatch
$ make && sudo make install
```


5) Run the executables

```
# Run mginit
$ build/Scripts/run-mginit

```

## Copying

Copyright (C) 2018 \~ 2020 [Beijing FMSoft Technologies Co., Ltd.]

* For device side and client side, HybridOS uses GPLv3 or LGPLv3.
* For server side, HybridOS uses AGPLv3.
* For documents, GPLv3 applies.

Note that HybridOS integrates many mature open source software, such as SQLite,
FreeType, V8 JavaScript engine, and so on. For the copyright owners and licenses
for these software, please refer to the README or LICENSE files contained in
the source trees.

[Beijing FMSoft Technologies Co., Ltd.]: https://www.fmsoft.cn
[FMSoft Technologies]: https://www.fmsoft.cn
[HybridOS Official Site]: https://hybridos.fmsoft.cn

[HybridOS Architecture]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-Architecture
[HybridOS App Framework]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-App-Framework
[HybridOS View Markup Language]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-View-Markup-Language
[HybridOS Foundation Class Library]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-Foundation-Class-Library
[HybridOS Security Design]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-Security-Design
[HybridOS Device Simulation Environment]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-Device-Simulation-Environment
[HybridOS Code and Development Convention]: https://github.com/FMSoftCN/hybridos/wiki/HybridOS-Code-and-Development-Convention

