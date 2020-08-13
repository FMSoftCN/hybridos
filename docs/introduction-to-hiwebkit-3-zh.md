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

为实现本特性的目标，我们为 HTML 新增一个属性，名为 `databind`。我们使用这个属性定义该元素的属性或者内容和其他元素之间的绑定关系。如下所示：

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

本特性要求：

- 所有函数使用字符串为参数；
- 使用单引号表示字面立即值，如 'We are literal words'；
- 对需要整数或浮点数作为参数时，以符合 C 语言标准的字符串形式表达，如 '55'（十进制）、'0B10'（二进制）、'071'（八进制）、'0xFF'（十六进制）、'55.356E10'（浮点数）等；
- 所有函数返回字符串，当参数非法时，返回空字符串（''）。

所有函数支持以上要求后，可实现函数的嵌套使用，并可实现高效的解析处理。如下面的表达式：

```
UPPER(STRCAT('a', 'b'))
```

将返回字符串`AB`。

对具体的用户代理实现，本特性建议至少支持如下字符串函数：

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

上面 HTML 代码中的 `view` 元素是通过一种复杂控件支持机制实现的秒表控件，其通过指针展示当前时间，效果见下图。当用户启动这个秒表控件后，我们希望通过其底部的文本展示秒表当前的时间，此时，我们就可以通过本特性提供的绑定机制高效实现这一功能。而如果没有这种绑定机制，我们需要编写一个脚本程序，这个脚本程序启动一个周期性的定时器（一秒或更短），在定时器中读取 `view` 元素的 `value` 属性值，然后再设置到 `p` 元素上。

![秒表当前值的绑定更新](figure-1.png)

在上面这一应用场景中，本特性提供的绑定关系将发挥更大的作用。实验发现，当 `view` 元素的 `value` 属性以百分秒（如`01:01.55`）的精度更新时，如果使用脚本同步更新 `p` 元素的内容，最高可达到的每秒更新次数为 20 次，而使用本特性提供的绑定关系，则可达到每秒 50 次的更新次数。

### HTML 内容的本地动态生成技术

本特性提供的方法，本质上通过让用户代理中的 HTML 解析器完成更多工作而避免使用大量脚本程序，从而提升了 HTML 页面的渲染效率，尤其是动态页面的渲染效率。

#### 1) 基本原理

在 HTML 5.3 规范中引入了一个新的元素类型：`template`。我们可以利用这个标签定义一个虚拟的元素，然后利用脚本来克隆（clone）这一虚拟元素，并将其插入到已有的 DOM 树中。有关 `template` 元素的更多信息，可参阅：

<https://www.w3.org/TR/html53/semantics-scripting.html#the-template-element>

本特性提供的方法，其目标在一定程度上和 HTML 5 规范引入的 `template` 元素类似，但无须执行脚本即可完成虚拟元素的克隆。为此，本特性引入了如下几个新的 HTML 元素：

- `archedata`：该元素在 HTML 文档的头部定义 JSON 格式的数据，JSON 数据可嵌入到 HTML 文档中，亦可加载外部内容获得。
- `archetype`：类似 `template` 的虚拟元素，用来定义一个 HTML 模板，通常为一段 HTML 片段。
- `iteration` 和 `displace` 元素：`iteration` 元素用来定义一个在数组型 JSON 数据上的迭代；`displace` 元素定义一个基于模板的置换。HTML 解析器可基于已有的 JSON 数据自动完成迭代操作，重复克隆 `archetype` 元素定义的模板，置换其中的属性或内容，并插入到 DOM 树中。
- `switch` 和 `case` 元素：这两个元素用来定义一个条件置换：在符合某个条件的情况下，克隆 `archetype` 元素定义的模板，置换其中的属性或者内容并插入到 DOM 树中。

如下是一个使用上述元素的 HTML 文档内容：

```html
<!DOCTYPE html>
<html>
    <head>
        <archedata name="users">
            [
                { "id": "1", "avatar": "/img/avatars/1.png", "name": "Tom", "region": "en_US" },
                { "id": "2", "avatar": "/img/avatars/2.png", "name": "Jerry", "region": "zh_CN" }
            ]
        </archedata>
        <archedata name="global">
            { "locale" : "zh_CN" }
        </archedata>
    </head>

    <body>

        <archetype id="footer-cn">
            <p><a href="http://www.baidu.com">Baidu</a></p>
        </archetype>

        <archetype id="footer-tw">
            <p><a href="http://www.bing.com">Bing</a></p>
        </archetype>

        <archetype id="footer-def">
            <p><a href="http://www.google.com">Google</a></p>
        </archetype>

        <archetype id="user-item">
            <li class="user-item" datamap="attr.id: $@.id; attr.alt: $@.name">
                <img datamap="attr.src: $@.avatar" />
                <span datamap="textContent: $@.name"></span>
            </li>
        </archetype>

        <archetype id="no-user">
            <li>
                <img src="def-avatar.png" />
                <p>Sign Up for the first user!</p>
            </li>
        </archetype>

        <header>
            <h1>User List</h1>
        </header>

        <main>
            <ul>
                <iteration data="$users">
                    <displace archetype="user-item" />
                </iteration>
            </ul>
        </main>

        <footer class="footer">
            <switch>
                <case archetype="footer-cn" if="MATCH($global.locale, 'zh_CN')" />
                <case archetype="footer-tw" if="MATCH($global.locale, '^zh', 'reg')" />
                <case archetype="footer-def" />
            </switch>
        </footer>
    </body>
</html>
```

上述 HTML 文档在 `head` 元素中使用 `archedata` 元素定义了一个全局的 JSON 数据：

```html
        <archedata name="users" once>
            [
                { "id": "1", "avatar": "/img/avatars/1.png", "name": "Tom", "region": "en_US" },
                { "id": "2", "avatar": "/img/avatars/2.png", "name": "Jerry", "region": "zh_CN" }
            ]
        </archedata>
```

在后续的 HTML 文档中，我们可以使用 `$users` 这个变量来引用这个 JSON 对象。注意 `archedata` 元素的 `name` 属性指定了该 JSON 对象的名称。

在上述 HTML 文档中，我们使用 `archetype` 元素定义了多个 HTML 片段模板，其中有一个模板如下所示：

```html
        <archetype id="user-item">
            <li class="user-item" datamap="attr.id: $@.id; attr.alt: $@.name">
                <img datamap="attr.src: $@.avatar" />
                <span datamap="textContent: $@.name"></span>
            </li>
        </archetype>
```

我们使用 `archetype` 元素的 `id` 属性给这个元素一个标识符，以便可以在后续代码中引用这个模板。`archetype` 子元素定义了一段 HTML 文档片段（fragement），其中使用了标准的 HTML 元素 `li`、`img`、`span` 等。

注意在 `li` 等标准 HTML 元素中，我们引入了一个新的 `datamap` 属性，该属性以下面的方式定义了一个或多个数据映射关系。以 `li` 元素为例：

```
attr.id: $@.id; attr.alt: $@.name
```

上面的 `datamap` 属性值定义了两个数据映射关系，用 `;`（分号）隔开。分别表示：

1. 将 `$@.id` 的值赋值给 `li` 元素的 `id` 属性。
1. 将 `$@.name` 的值赋值给 `li` 元素的 `alt` 属性。

类似地，`img` 元素的 `src` 属性将使用 `$@.avatar` 的值；而 `span` 元素的文本内容（用关键词 `textContent` 表示）将被 `$@.name` 的值替代。

其中，`$@` 是本特性中用来指代当前上下文的一个特殊变量。为了说明其作用，我们看引用这个模板的代码：

```html
                <iteration data="$users">
                    <displace with="user-item" />
                </iteration>
```

上述代码中的 `iteration` 元素使用 `data` 属性定义了在 `$users` JSON 对象上的一个迭代，并在其中使用 `displace` 元素的 `with` 属性引用了模板 `user-item`。

在用户代理在 `$users` JSON 数据上执行迭代时（该 JSON 数据是一个数组，其中的每个单元是一个字典数据结构），每次迭代会有一个迭代上下文数据，也就是该 JSON 数据中的每个单元，我们用 `$@` 这一特别变量名称来指代这个上下文数据。因此，

1. 在第一次迭代中，`$@` 的值是 `{ "id": "1", "avatar": "/img/avatars/1.png", "name": "Tom", "region": "en_US" }`，所以 `$@.id` 的值是 `1`，而 `$@.name` 的值是 `Tom`。
1. 在第二次迭代中，`$@` 的值是 `{ "id": "2", "avatar": "/img/avatars/2.png", "name": "Jerry", "region": "zh_CN" }`，所以 `$@.id` 的值是 `2`，而 `$@.name` 的值是 `Jerry`。

于是，迭代执行完成后的实际生成的 HTML 文档片段将为：

```html
        <main>
            <ul>
                <li class="user-item" id="1" alt="Tom">
                    <img src="/img/avatars/1.png" />
                    <span>Tom</span>
                </li>
                <li class="user-item" id="2" alt="Jerry">
                    <img src="/img/avatars/2.png" />
                    <span>Jerry</span>
                </li>
            </ul>
        </main>
```

类似地，`switch` 和 `case` 则定义了一个条件置换：

```html
            <switch>
                <case with="footer-cn" if="MATCH($global.locale, 'zh_CN')" />
                <case with="footer-tw" if="MATCH($global.locale, '^zh', 'reg')" />
                <case with="footer-def" />
            </switch>
```

`case` 元素使用 `if` 属性定义了一个匹配条件，使用 `with` 属性指定了要置换的模板。HTML 解析器在解析 `switch` 元素时，将依次判断 `case` 子元素中的匹配条件，若匹配则执行 `with` 属性定义的置换，并不再继续判断其后的 `case` 子元素。

上述代码引用了名为 `global` 的全局 JSON 对象，并使用 `$global.locale` 的值作为判断条件，判断当前地区是中国大陆（`zh_CN`）还是其他中文地区，如台湾（`zh_TW`）、香港（`zh_HK`）等。其中 `if` 使用了本特性定义的字符串匹配函数 `MATCH`，用来判断两个字符串是否匹配。

由于 `head` 中定义的 `$global.locale` 的值为 `zh_CN`，故上述条件置换的结果为：

```html
<p><a href="http://www.baidu.com">Baidu</a></p>
```

当HTML解析器完成解析并作用了迭代置换以及条件置换之后，上述 HTML 文档最终的内容如下：

```html
        <header>
            <h1>User List</h1>
        </header>

        <main>
            <ul>
                <li class="user-item" id="1" alt="Tom">
                    <img src="/img/avatars/1.png" />
                    <span>Tom</span>
                </li>
                <li class="user-item" id="2" alt="Jerry">
                    <img src="/img/avatars/2.png" />
                    <span>Jerry</span>
                </li>
            </ul>
        </main>

        <footer class="footer">
            <p><a href="http://www.baidu.com">Baidu</a></p>
        </footer>
```

#### 2) 定义数据映射

整体上，定义数据映射的方法和本特性权利人提交的另一项特性《一种HTML元素属性或内容的绑定机制》中定义的数据绑定类似，但稍有不同。

```html
        <archetype id="user-item">
            <li class="user-item" datamap="attr.id: $@.id; attr.alt: $@.name">
                <img datamap="attr.src: $@.avatar" />
                <span datamap="textContent: $@.name"></span>
            </li>
        </archetype>
```

如上所述，从 JSON 数据置换到模板元素属性、内容等的关系，通过 `archetype` 元素上的 `datamap` 属性定义，该属性的值是一个或多个赋值表达式：

```
    <lvalue>: <rvalue expression>[; <lvalue>: <rvalue expression>]
```

该表达式使用 `:`（冒号）运算符，其左值（lvalue）指定模板元素的哪个属性或者内容将被置换，可取值：

- `textContent`：表示右值（rvalue）表达式的值将被用作目标元素的文本内容。该取值亦可用 `innerText` 表示。
- `innerHTML`：表示右值表达式的值将被用作目标元素的内部 HTML 内容。
- `attr.<attribute-name>`：表示右值表达式的值将被用作目标元素的特定属性（attribute）的值，如 `attr.value`、`attr.name`、`attr.id` 等。
- `style.<style-name>`：表示右值表达式的值将被用作目标元素的特定 CSS 属性（property）的值，如 `style.color`、`style.font`、`style.width` 等。

在右值表达式中，我们使用 `$` 前缀来指代一个变量，如前面示例中使用的 `$global`、`$users`、`$@` 等。`$@` 这类名称特殊的变量称为内置（built-in）变量，可在迭代中使用。除了 `$@` 之外，本特性还定义了其他内置变量，罗列如下：

- `$@`：当前迭代上下文中使用的数据。
- `$&`：当前迭代的索引值，比如第一次迭代，该变量的值为 0，第二次迭代，该变量的值为 1，以此类推。
- `$#`：被迭代的数据的单元数量。
- `$<N>`：被迭代数组数据的第 `N` 个单元数据。
- `$$`：当前迭代上下文数据的 JSON 字符串表达，比如用 `['string0', 'string1', 'string2', 'string3']` 表示一个字符串数组。
- `$!`：一个空的数组对象。

另外，如《一种HTML元素属性或内容的绑定机制》所述，我们还可以在右值表达式中引用已有 HTML 元素的属性值或者内容：使用 CSS 选择器来选择 HTML 文档中的其他元素，然后使用类似左值的表达式来选择该元素的文本内容、HTML 内容、HTML 属性（attribute）或者 CSS 属性（property）值。如：

```
(#theColor).attr.value
```

在定义数据映射关系的右值表达式中，我们还可以使用《一种HTML元素属性或内容的绑定机制》所描述的字符串函数。详情可参阅该特性的技术文档。

比如，下面的代码就引用了 `archedata` 数据对象中数据、已有元素的数据以及内置的字符串函数：

```html
        <input type="hidden" id="theSiteName" name="site-name" value="" />

        <archetype id="user-item">
            <li class="user-item" datamap="attr.id: STRCAT('user-', $@.id); attr.alt: STRCAT((#theSiteName).attr.value, $@.name)">
                <img datamap="attr.src: $@.avatar" />
                <span datamap="textContent: $@.name"></span>
            </li>
        </archetype>
```

#### 3) 迭代置换

如前所述，当我们需要根据 `archedata` 元素指定的 JSON 数组对象迭代克隆 `archetype` 模板元素时，使用 `iteration` 和 `displace` 元素。

在 `iteration` 元素中，我们使用 `data` 属性指定要迭代的 JSON 数据对象名称，同时使用如下属性指定一些迭代行为：

- `datarange`：使用该属性指定迭代范围，语法为：`[start], stop[, step]`，分别指定起始索引值、结束索引值以及步长值。不指定该属性时，按照自然顺序依次迭代。
- `sortbykey`：指定排序键。当数组元素为字典时，可以通过这个属性指定按照某个特定的键值进行排序，然后再进行迭代。

如下面的代码，使用 `$users` 数据对象，从索引值 0 迭代到索引值 10：

```html
    <iteration data="$users" datarange="0, 10">
        <displace data="$@" with="user-item" />
    </iteration>
```

而下面的代码，仅迭代 `$users` 数组对象中的偶数索引值对应的单元：

```
    datarange="0, $#, 2"
```

下面的代码反序迭代 `$users` 数组对象：

```
    datarange="$#, 0, -1"
```

下面的代码根据指定的数组单元键值进行排序，然后再进行迭代：

```
    sortbykey="name ASC"
```

其中的 `ASC` 或者 `DESC` 后缀可用于指定排序规则，分别为升序、降序，默认为升序。

另外，在 `displace` 元素中，我们可以使用 `data` 属性指定当前迭代上下文数据中的某个子对象，比如：

```html
        <displace data="$@.info" with="user-item" />
```

`data` 属性的值默认为 `$@`，表示当前上下文数据。

#### 4) 条件置换

我们在上面的示例中已经看到了条件置换的基本用法。条件置换还可以在迭代置换中使用：

```html
    <iteration data="$users" datarange="0, 10">
        <switch>
            <case with="footer-cn" if="MATCH($global.locale, $@.locale)" />
        </switch>
    </iteration>
```

以上代码最终将只显示 `$users` 中，`locale` 键值匹配 `$global.locale` 的用户。

在 `if` 属性中，我们可以使用预先定义的逻辑操作函数，用于实现复杂的逻辑运算，和字符串操作函数类似，这些函数可嵌套使用。另外有如下约定：

- 当字符串作为逻辑值使用时，空字符串（''）表示 FALSE，其他字符串表示 TRUE。
- 当逻辑值作为字符串使用时，FALSE 对应空字符串（''），TRUE 对应包含一个空格的字符串（' '）。

本特性要求必须实现的逻辑操作函数如下：

- `NOT`，对参数逻辑值做取反操作，其原型为：

```
NOT(boolean)
```

- `AND`，逻辑与操作，要求至少两个参数，其原型为：

```
AND(boolean1, boolean2, ...)
```

- `OR`，逻辑或操作，要求至少两个参数，其原型为：

```
OR(boolean1, boolean2, ...)
```

- `XOR`，逻辑异或操作，其原型为：

```
XOR(boolean1, boolean2)
```

同时应至少实现如下字符串匹配函数：

- `MATCH`，判断两个字符串是否匹配，可选严格匹配，不区分大小写匹配或者执行正则表达式匹配，其原型为：

```
    MATCH($string1, $string2[, 'case'|'reg')
```

其中第三个参数可选：

- 没有第三个参数时，不区分大小写匹配字符串。
- 第三个字符串为 'case' 时，区分大小写匹配字符串。
- 第三个字符串为 'reg' 时，匹配正则表达式。

有了以上的逻辑操作函数之后，我们可以构造比较复杂的逻辑条件，如：

```html
    <iteration data="$users" datarange="0, 10">
        <switch>
            <case with="footer-cn" if="OR(MATCH($global.locale, $@.locale), MATCH(LEFT($@.name, '1'), 'J'))" />
        </switch>
    </iteration>
```

以上的 `if` 条件在用户的 `$@.locale` 值匹配 `$global.locale`，或者 `$@.name` 的第一个字母匹配 'J' 时成立。

#### 5) 从外部装载数据

显然，通过使用 `archedata` 和 `archetype` 元素，我们可以实现文档和数据的隔离或者解藕。此种情况下，我们可以将 HTML 文档看成一个大的模板，最终的 HTML 文档根据 `archedata` 定义的数据在本地由 HTML 解析器动态生成。

尽管上面的例子给出的数据仍然包括在 HTML 文档中，但我们可以通过简单的扩展让 HTML 解析器从外部装载数据：

```html
<!DOCTYPE html>
<html lang="en">
    <head>
        <archedata name="users"
                href="https://company.com/get-user-list?start=0&count=10" />
        ...
    </head>

    <body>
        ...
    </body>
</html>
```

在 `head` 元素的 `archedata` 子元素中，我们使用 `href` 属性来指定需要从外部装载的 `archedata` 的 URL。于是，我们将彻底隔离生成文档使用的数据和文档本身：

1. 服务器端用来提供数据的接口，不需要关心数据的具体呈现方式，而只需要使用抽象的 JSON 格式返回这些数据即可。
1. 原本由服务器端动态生成的 HTML 文档，现在交由用户代理的 HTML 解析器生成。

这一设计将大大简化服务器端的代码设计和开发工作量，并降低服务器端的负载。

但随之而来另外一个问题： 由于 `archedata` 数据通常由用户代理在后台异步加载，在数据尚未准备就绪的情况下，页面内容如何呈现？

为了解决这一问题，在数据未准备好，或者在引用数据出现错误时，使用 `iteration` 元素的 `nodata` 子元素的内容进行渲染，或者使用 `displace` 或者 `case` 元素的子元素进行渲染，如下所示：

```html
    <iteration data="$users">
        <displace with="user-item">
            <p>Bad data!</p>
        </displace>

        <switch>
            <case with="footer-cn" if="MATCH($global.locale, 'zh_CN')">
                <p>Bad locale data!</p>
            </case>
            <case with="footer-tw" if="MATCH($global.locale, '^zh', 'reg')">
                <p>Bad locale data!</p>
            </case>
            <case with="footer-def">
            </case>
        </switch>

        <nodata>
            <p>Please wait for data!</p>
        </nodata>
    </iteration>
```

在 `$users` 数据尚未准备好时，以上内容相当于如下 HTML 片段：

```html
        <p>Please wait for data!</p>
```

在 `$users` 已准备好（假定迭代 2 次），但 `$global` 未准备好时，以上内容相当于如下 HTML 片段：

```html
        <p>Bad locale data!</p>
        <p>Bad locale data!</p>
```

我们将这些元素称为备用元素（alternative elements）。一旦这些数据就绪，HTML 解析器将重新解析 `iteration` 等元素，完成迭代并置换模板元素的工作，删除备用元素并将置换后的元素插入 DOM 树中。

这种处理方法带来另一个好处：

- 浏览器展示页面内容发起两次内容请求，一次获取静态的 HTML 文档内容，可以非常快，渲染速度也很快，并可在本地长期缓存，而另外一次是获取真正数据的请求，相对会慢一些。
- 基于此，浏览器在展现一个 HTML 文档内容时，可通过静态 HTML 文档中的备用元素展示等待状态的页面（这将非常快），等数据就绪后，再展示完整页面，这将带来用户体验的提升。

#### 6) 本特性的优点

1. 通过引入新的虚拟元素和数据映射技术，使得用户代理对 DOM 树的动态调整无须脚本介入就可实现，通过降低脚本的使用量来提高复杂网页的整体渲染性能：
   - 由 HTML 解析器基于 JSON 对象数据实现的迭代、置换等方法，相比基于虚拟DOM等严重依赖脚本的实现，可大幅降低系统内存的使用，并提高处理速度。
   - 通过简单高效的函数式数据映射定义技术，在占用内存小、运行速度快的基础上提供了可编程能力，为数据映射的实现带来了灵活性。
1. 将通常由服务器后台实现的 HTML 文档的动态生成工作，转移到了用户代理一侧，可简化后台服务器的设计和开发工作量，降低了服务器的负载，减少了数据传输量。
   - 使用本特性提供的方法之后，后台服务器将不需要考虑数据的具体表述形式（比如使用 HTML 还是 XML），而只需要提供 JSON 格式的原始数据即可。
1. 通过将 HTML 文档进行模板化改造，解除或削弱了数据和 HTML 文档结构之间的耦合，同时改善用户体验。
   - 浏览器最终展示的内容由两部分组成，一个是作为模板的 HTML 文档，另外一个是用来迭代、置换模板的数据。因此，浏览器在渲染一个页面时，将发起两次内容请求，一次获取静态的 HTML 模板文档内容，体积小，所以可以非常快，且可在本地长期缓存；而另外一次是获取真正数据的请求，相对会慢一些。
   - 基于此，浏览器在展现一个 HTML 文档内容时，可通过静态 HTML 文档中的备用元素展示等待状态的页面（这将非常快），等数据就绪后，再展示完整页面，这将带来用户体验的提升。

---

更多合璧操作系统的技术突破，请关注后续文章。

