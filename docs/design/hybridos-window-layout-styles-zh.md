# 合璧操作系统设备端窗口布局样式规范

【主题】合璧操作系统设备端窗口布局样式规范  
【概要】本文阐述合璧操作系统设备端窗口的布局样式  
【版本】1.0  
【作者】魏永明  
【日期】2021 年 01 月  
【状态】初稿  

**版权声明**

版权所有 &copy; 2020 北京飞漫软件技术有限公司  
保留所有权利

此文档不受合璧操作系统相关软件开源许可证的管辖。

飞漫软件公开此文档的目标，用于向开发者解释合璧操作系统的设计原理或者相关规范。在未获得飞漫软件书面许可之前，任何人不得复制或者分发本文档的全部或部分内容，或利用本文档描绘的技术思路申请专利、撰写学术论文等。

本文涉及到的飞漫软件或其合作伙伴的注册商标或商标，有关其详细列表，请查阅文档末尾。

**目录**


## 基本概念及术语

为了支持应用行者在多个窗口中运行，合璧操作系统（HybridOS）的应用管理框架提出了使用 CSS 子集来定义多个窗口的布局、大小和位置。本文描述相关规范。相关术语归纳如下：

- 布局（Layout）：我们将屏幕看成一个容器，布局定义了窗口在屏幕中的布局模型，比如 CSS 最初定义的块布局以及后来引入的弹性盒子、网格布局等。
- 盒子（Box）：我们将窗口看成在屏幕中排列的矩形盒子，盒子样式定义了窗口的边距、间距、大小、位置等样式。

整体上，合璧操作系统设备端窗口布局的样式规范使用 CSS 2.1 定义的基本布局和定位样式，加上后来引入的弹性盒子布局模型以及网格布局模型。

## 使用

在合璧操作系统中，当交互行者以窗口形式运行时，可以在 manifest 文件中使用 `windowLayoutStyles` 和 `windowBoxStyles` 设置窗口的布局和盒子样式。如：

```json
{
  "windowLayoutStyles": "display: block;"
  "runners": [
    {
      "name": "wifi",
      "type": "hijs",
      "entry": "hijs/wifi/index.bc",
      "runas": "window autostart",
      "windowBoxStyles": "postion: relative; width: 100%; height: 50%;"
    },
    {
      "name": "bluetooth",
      "type": "hijs",
      "entry": "hijs/bluetooth/index.bc",
      "runas": "window autostart"
      "windowBoxStyles": "postion: relative; width: 100%; height: 50%;"
    },
  ]
}

为简化使用，我们对如下 CSS 相关的内容做取舍：

- 简单的选择器，使用如下规则：
   + 我们将行者类型当做标签类型使用，所以可使用 `hijs` 作为选择器。
   + 我们将行者名称当做标识符使用，所以可使用 `#wifi`、`#bluetooth` 选择器。
   + 我们可为每个行者定义一个类（class）,故而可使用 `.header` 这类选择器。
   + 对于屏幕（即容器），我们定义隐含的标识符 `#screen`。
- 不使用 `em` 等以字符高度、宽度确定的逻辑长度单位；仅支持百分比和绝对单位，如像素单位（px）。
- 不支持伪类、伪元素等。
- 不支持有关字体、文本、颜色等的属性。
- 可选择支持媒体查询。

在 manifest 文件中，我们可以使用上述两个关键词定义布局样式和盒子样式，这时候无需使用大括号。
但为了方便使用，我们可以用独立的 CSS 文件来定义窗口布局，其中可使用以上所述的选择器。

```css
@media landscape #screen {
    display: block;
    width: 50%;
    height: 100%;
}

@media portrait #screen {
    display: block;
    width: 100%;
    height: 50%;
}

#wifi {
    position: relative;
    width: 100%;
    height: 25%;
}

hijs {
    position: relative;
    width: 100%;
    height: 50%;
}
```

假如以上文件保存在 `assets/default.css` 文件中，则我们可如下修改应用的 manifest 文件：

```json
{
  "windowCSSFile": "assets/default.css",
  "windowLayoutStyles": "display: block;"
  "runners": [
    {
      "name": "wifi",
      "type": "hijs",
      "class": "header",
      "entry": "hijs/wifi/index.bc",
      "runas": "window autostart",
      "windowBoxStyles": "postion: relative; width: 100%; height: 50%;"
    },
    {
      "name": "bluetooth",
      "type": "hijs",
      "class": "header",
      "entry": "hijs/bluetooth/index.bc",
      "runas": "window autostart"
      "windowBoxStyles": "postion: relative; width: 100%; height: 50%;"
    },
  ]
}

我们在 manifest 文件中仍然可以直接使用 `windowLayoutStyles` 和 `windowBoxStyles` 两个关键词来定义样式。此种情况下，CSS 级联规则生效。

## 布局模型

在当前规范中，我们支持如下几种 CSS 定义的布局：`block`、`inline-block`、`flex`、`inline-flex`、`grid`。通常通过 `display` 属性定义，如：

我们不打算支持如下布局模型：

- 表格相关布局，如 table、inline-table、table-row-group、table-header-group 等。
- 列表，item-list。
- 其他专用于文本排版的布局，如 ruby。

参阅如下规范文档：

- [CSS 2.1 Specification]<https://www.w3.org/TR/CSS21/>
- [CSS Flexible Box Layout Module Level 1]<https://www.w3.org/TR/css-flexbox-1/>
- [CSS Grid Layout Module Level 1]<https://www.w3.org/TR/css-grid-1/>

## 盒子模型

参阅如下规范文档：

- [CSS 2.1 Specification: Box model]<https://www.w3.org/TR/CSS21/box.html>

## 未来可支持的样式

在未来，可支持窗口显示、关闭时的动画。

## 附：参考链接

- [CSS Display Module Level 3]<https://www.w3.org/TR/css-display-3/>
- [CSS Positioned Layout Module Level 3]<https://www.w3.org/TR/css-position-3/>
- [CSS Box Module Level 4]<https://www.w3.org/TR/css-display-4/>

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

