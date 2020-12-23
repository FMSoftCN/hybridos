# 合璧操作系统应用管理器概要设计

【主题】合璧操作系统应用管理器概要设计  
【摘要】本文描述了合璧操作系统的应用管理架构、安全性设计以及访问控制策略
【作者】魏永明  
【日期】2020 年 12 月  
【状态】草案  

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

本文涉及到的飞漫软件或其合作伙伴的注册商标或商标，有关其详细列表，请查阅文档末尾。

**目录**

- [基本框架及术语](#基本框架及术语)
- [协议及接口](#协议及接口)
   + [简单的应用管理](#简单的应用管理)
   + [端点权限管理](#端点权限管理)
- [附：商标声明](#附商标声明)


## 基本框架及术语


## 协议及接口


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

## 附：商标声明

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

