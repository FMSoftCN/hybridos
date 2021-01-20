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

（待定）


