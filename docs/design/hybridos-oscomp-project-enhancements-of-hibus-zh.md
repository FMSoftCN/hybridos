# OSComp Project: Enhancements of hiBus

## 题目

合璧操作系统设备端数据总线 hiBus 的增强

## 项目描述

hiBus 是合璧操作系统提出的一个设备端数据总线技术，通过 hiBus 本机或者远程主机上的程序可以发起远程过程调用或者订阅特定事件。

hiBus 为客户端提供了 Unix Domain Socket 和 WebSocket 两种连接方式。

目前，hiBus 已完成 1.0 版本的开发。现有如下增强需求：

1. 在客户端函数库中实现 WebSocket 的支持。hiBus 服务器已实现对 WebSocket 的支持，可通过支持 WebSocket 的浏览器直接连入 hiBus 服务器。但客户端函数库中尚未完成对 WebSocket 的支持，希望完成相关实现。
1. 增强 hiBus 服务器，使之可以动态装载一个内建行者，并开发一个 hiBus 内建行者，提供类似 BusyBox 的命令执行能力，从而可通过 hiBus 由本机或者远程的行者执行命令，并获得命令的输出结果。比如通过执行 `ls *.c` 获得当前路径下的所有 C 文件列表。

当前项目相关资料：

- 仓库：<https://github.com/FMSoftCN/hibus>
- hiBus 协议：<https://github.com/FMSoftCN/hybridos/blob/master/docs/design/hybridos-data-bus-zh.md>

## 所属赛道

2021全国大学生操作系统比赛的“OS功能设计”赛道

## 参赛要求

- 以小组为单位参赛，最多三人一个小组，且小组成员是来自同一所高校的本科生（2021年春季学期或之后本科毕业的大一~大四的学生）
- 如学生参加了多个项目，参赛学生选择一个自己参加的项目参与评奖
- 请遵循“2021全国大学生操作系统比赛”的章程和技术方案要求

## 项目导师

魏永明：
- GitHub 账号：<https://github.com/VincentWei>
- 邮箱：<hybridos@fmsoft.cn>

## 难度

中等

## 许可证

- GPLv3 <https://www.gnu.org/licenses/gpl-3.0.html>

## 预期目标

### 客户端函数可支持 WebSocket

在 `libhibus.so` 函数库中，使用 C 代码增加对 WebSocket 的支持。针对远程主机连接，需要考虑如下主机名称相关的事宜：

1. 如何定义 hiBus 服务器所在主机的主机名。
1. 枚举已连接到 hiBus 的客户端主机，实现为内置行者的一个方法。

### 定义 hiBus 服务器端内置行者的接口

以操作函数指针的形式，定义 hiBus 服务器端动态装载内置行者的接口，该接口可用于注册一个不同于 `builtin` 的内置行者，并在 hiBus 中注册该行者对应的方法以及泡泡。

### 将 BusyBox 的功能重构为 `busybox` 内置行者的方法

该行者应以动态库的形式实现，并在 hiBus 启动时装载。该行者可以以远程过程的方式执行 busybox 提供的命令行功能。注意如下要点：

1. 设计对应的远程过程调用和/或事件的接口。
1. 以客户端行者为单位，维护一个上下文信息，主要用来维护当前路径等 Shell 常见的环境变量。
1. 所有的命令，在 hiBus 子进程中执行。子进程应切换 euid（有效用户标识符）到调用者的用户标识符；对远程主机上的行者，使用 `guest` 用户作为其 euid。
1. 命令行支持支持重定向和管道。
1. 所有命令的标准输出和标准错误，在没有被重定向的前提下，作为该过程的返回值返回给调用者。


