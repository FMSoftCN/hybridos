# 合璧操作系统的大胆技术突破（3）

魏永明

本文是《合璧操作系统的大胆技术突破》系列文章的第三篇。主要阐述我们在 hiWebKit 中新增的数据绑定和 HTML 内容的本地生成技术。有关本系列的其他文章，可点击上面的专辑链接查阅。

## 问题和需求

熟悉浏览器的人知道，WebKit 是苹果 Safari 浏览器使用的引擎，谷歌 Chrome 浏览器使用的引擎也发端于此。作为合璧操作系统核心组件的 hiWebKit，是在苹果维护的最新 WebKit 引擎基础上做了诸多小的突破。本文介绍的小的突破，其目的是为了支持在物联网或者嵌入式系统中运行合璧操作系统的应用时，可以少编写一些 JavaScript 脚本程序，而让用户代理（user agent，指浏览器引擎）自动完成一些工作。这符合低代码（low code）趋势。

比如，我们能否在 HTML 页面的不同元素之间建立一种关联关系？当某个元素的值发生变化的时候，可以自动更新另外一个元素的值？

再比如，我们能否让用户代理根据外部数据自动创建新的页面内容？

而在当前，要实现上述两个功能，我们都需要编写 JavaScript 脚本代码才能完成相应的工作。

## hiWebKit 的解决方案

在 hiWebKit 中，我们引入两种新的特性，使之在一些情况下，我们可以避免编写 JavaScript 脚本，而让用户代理自动完成上述工作：

1. 通过定义 HTML 元素之间的绑定关系，由用户代理自动完成被绑定元素之间的属性或者内容同步。
1. 通过数据映射技术，使得用户代理对 DOM 树的动态调整无须脚本介入就可实现。

以下分别介绍这两种新的特性。

### HTML 元素间属性或内容的绑定机制

#### 1) 简单示例

为实现本发明的目标，我们为 HTML 新增一个属性，名为 `databind`。我们使用这个属性定义该元素的属性或者内容和其他元素之间的绑定关系。如下所示：

```html
<main>
    <select size="4" name="noodle" value="兰州拉面" id="noodle">
        <option value="兰州拉面">兰州拉面</option>
        <option value="重庆小面">重庆小面</option>
        <option value="上海阳春面">上海阳春面</option>
        <option value="武汉热干面">武汉热干面</option>
    </select>
    <p>您当前的选择是：<span databind="textContent: (#noodle).attr.value"></span></p>
</main>
```

上面的例子中，我们使用 `textContent: #noodle.attr.value` 这一语句定义了两个不同 HTML 元素之间的数据绑定关系：

- `span` 元素的内容将依赖于 `id` 属性为 `noodle` 这个元素（即 `select` 元素）的 `value` 属性。我们将 `span` 元素称为目标元素，`select` 元素称为被绑定元素。
- `span` 元素的初始内容相当于：

```html
    <span>兰州拉面</span>
```

- 当 `select` 元素的 `value` 属性值因为用户选择而发生变化时，`span` 元素的内容将自动发生变化。比如，用户通过 `select` 元素选择了 `重庆小面` 这一条目，则 `span` 元素的内容相当于：

```html
    <span>重庆小面</span>
```

这一同步改动将由用户代理自动实现，不需要运行任何外部脚本程序。

#### 2) 定义绑定关系

如上所述，绑定关系通过特定元素的 `databind` 属性定义，该属性的值是一个赋值表达式：

```
    <lvalue>: <rvalue expression>
```

该表达式使用 `:`（冒号）运算符，其左值（lvalue）指定该元素的哪个属性或者内容受其他元素影响，可取值：

- `textContent`：表示右值（rvalue）将被用作目标元素的文本内容。该取值亦可用 `innerText` 表示。
- `innerHTML`：表示右值表达式的值将被用作目标元素的内部 HTML 内容。
- `attr.<attribute-name>`：表示右值表达式的值将被用作目标元素的特定属性（attribute）的值，如 `attr.value`、`attr.name`、`attr.id` 等。
- `style.<style-name>`：表示右值表达式的值将被用作目标元素的特定 CSS 属性（property）的值，如 `style.color`、`style.font`、`style.width` 等。

在最简单情况下，我们可以使用 CSS 选择器来选择 HTML 文档中的其他元素作为被绑定的元素，然后使用类似左值的表达式来选择该元素的文本内容、HTML 内容、HTML 属性（attribute）或者 CSS 属性（property）值作为右值（rvalue）。如：

```
    style.color: (#theColor).attr.value
```

上述表达式使用 `id` 为 `theColor` 的元素之 `value` 属性值，将用于目标元素的 `color` 风格属性值。

其中，CSS 选择器的规范可见文档：

<https://www.w3.org/TR/selectors-3/>

当 CSS 选择器返回多个 HTML 元素时，可使用下标获得指定的元素：

```
    (.pastoral)[0]
```

上述表达式返回 `class` 属性为 `pastoral` 的第一个元素。

如果我们要在 `databind` 属性中指定多个绑定关系，可以使用 `;`（分号）分割多个绑定关系，如：

```
    style.color: (#theColor).attr.value; attr.class: (#theColor).attr.value
```

#### 3) 字符串操作函数

我们可以在右值表达式中使用预先定义的字符串函数，用来实现基本的字符串操作，以便提供一定的可编程能力。

本发明要求：

- 所有函数使用字符串为参数；
- 使用单引号表示字面立即值，如 'We are literal words'；
- 对需要整数或浮点数作为参数时，以符合 C 语言标准的字符串形式表达，如 '55'（十进制）、'0B10'（二进制）、'071'（八进制）、'0xFF'（十六进制）、'55.356E10'（浮点数）等；
- 所有函数返回字符串，当参数非法时，返回空字符串（''）。

所有函数支持以上要求后，可实现函数的嵌套使用，并可实现高效的解析处理。如下面的表达式：

```
UPPER(STRCAT('a', 'b'))
```

将返回字符串`AB`。

对具体的用户代理实现，本发明建议至少支持如下字符串函数：

- `INTSTR`，将给定字符串按照要求的进制进行转换，其原型为：

```
INTSTR(string[, scale])

INTSTR('255 asdfk', '16') returns '0x55';
```

- `STRCAT`，用于连接两个或多个字符串，其原型为：

```
STRCAT(string1, string2, ...)
```

- `LOWER`，用于转换字符串中所有字符为小写字母，其原型为：

```
LOWER(string)
```

- `UPPER`，用于转换字符串中所有字符为大写字母，其原型为：

```
UPPER(string)
```

- `SUBSTR`，用于获取子字符串，其原型为：

```
SUBSTR(string, position[, length])
```

- `LENGTH`，用于获取字符串长度（返回值仍为字符串），其原型为：

```
LENGTH(string)
```

- `LEFT`，返回指定长度左侧子字符串，其原型为：

```
LEFT(string, length)
```

- `RIGHT`，返回指定长度右侧子字符串，其原型为：

```
RIGHT(string, length)
```

- `LTRIM`，去掉字符串左侧的指定前缀子字符串（默认为空格），其原型为：

```
LTRIM(string[, prefix])
```

- `RTRIM`，去掉字符串右侧的指定后缀子字符串（默认为空格），其原型为：

```
RTRIM(string[, postfix])
```

- `LSTRIP`，返回从开头到第一个分割字符串（默认为空格）之间的子字符串，其原型为：

```
LSTRIP(string[, delimiter])
```

- `RSTRIP`，返回从结尾倒数第一个分割字符串（默认为空格）之间的子字符串，其原型为：

```
RSTRIP(string[, delimiter])
```

- `REPEAT`，重复字符串，其原型为：

```
REPEAT(string, count)
```

- `REVERSE`，反转字符串，其原型为：

```
REVERSE(string)
```

- `REPLACE`，查找并替换子字符串，其原型为：

```
REPLACE(string, from, to)
```

- `GETSTR`，返回字符串数组（使用 JSON 表达式，如 `["兰州拉面", "重庆小面", "上海阳春面", "武汉热干面" ]`）中特定位置（索引值）的字符串，其原型为：

```
GETSTR(array, index)
```

- `MAPSTR`，在字典结构（使用 JSON 表达式，如 `{ "la": "兰州拉面", "xiao": "重庆小面", "yang": "上海阳春面", "re": "武汉热干面" }`）中查找特定值，其原型为：

```
MAPSTR(dict, key)
```

- `JSONIZE`，利用多个字符串构造特定的 JSON 表达式字符串，其原型为：

```
JSONIZE(<'array'|'dict'>, string1, string2, string3, string4, ...)
```

#### 4) 用法示例

根据以上描述，我们可以非常方便地实现各种绑定关系，并通过函数操作实现某些期望的结果。比如，

```html
    <h3 id="theFirstHead" class="info">Head 1</h3>
    <h3 id="theSecondHead" class="warning">Head 2</h3>
    <p databind="attr.class: STRCAT((h3)[0].attr.class, '-', (h3)[1].attr.class);
                innerText: UPPER(STRCAT((h3)[0].innerText, '; ', (h3)[1].innerText))">Paragraph</p>
```

以上代码中的 databind 属性定义了两个绑定关系：

1. 第一个绑定关系使用第一个 `h3` 元素的 `class` 属性值以及第二个 `h3` 元素的 `class` 属性值，中间使用 `-` 字符连接后赋值给了当前元素的 `class` 属性。
1. 第二个绑定关系使用第一个 `h3` 元素的 `innerText` 内容值以及第二个 `h3` 元素的 `innerText` 内容值，中间使用 `; ` 字符串连接后赋值给了当前元素的 `innerText` 内容。

作用以上绑定关系表达式后，相当于如下代码：

```html
<h3 class="info">Head 1</h3>
<h3 class="warning">Head 2</h3>
<p class="info-warning">HEAD 1; HEAD 2</p>
```

#### 5) 本特性的优点

除了提供一种简单可行的方法实现元素属性或内容间的同步之外，本特性还可以提供明显的性能改善。

考虑如下秒表的实现：

```html
    <view id="theStopWatch" type="stopwatch" design="analog"
            class="CLASS" id="IDENTIFIER"
            name="NAME" value="0.00" state=""
            width="600" height="400">
        <param name="action" value="start" />
        <param name="faces" value="second, minute, hour, digits" />
        <param name="secondscale" value="60" />
        <param name="minutescale" value="60" />
        <param name="hourscale" value="24" />
        <param src="dida.mp3" name="ticktack" value="loop, always" />
    </view>

    <p databind="textContent: (#theStopWatch).attr.value"></p>
```

上面 HTML 代码中的 `view` 元素是通过一种复杂控件支持机制实现的秒表控件，其通过指针展示当前时间，效果见下图。当用户启动这个秒表控件后，我们希望通过其底部的文本展示秒表当前的时间，此时，我们就可以通过本发明提供的绑定机制高效实现这一功能。而如果没有这种绑定机制，我们需要编写一个脚本程序，这个脚本程序启动一个周期性的定时器（一秒或更短），在定时器中读取 `view` 元素的 `value` 属性值，然后再设置到 `p` 元素上。

![秒表当前值的绑定更新](figure-1.png)

在上面这一应用场景中，本发明提供的绑定关系将发挥更大的作用。实验发现，当 `view` 元素的 `value` 属性以百分秒（如`01:01.55`）的精度更新时，如果使用脚本同步更新 `p` 元素的内容，最高可达到的每秒更新次数为 20 次，而使用本发明提供的绑定关系，则可达到每秒 50 次的更新次数。

### HTML 内容的本地动态生成技术

---

更多合璧操作系统的技术突破，请关注后续文章。

