# 漫谈 HVML，它的由来和未来

魏永明

HVML 是笔者在开发合璧操作系统的过程中提出的一种新型的高级编程语言。如小说《考鼎记》中所介绍，这语言有个昵称叫“呼噜猫”。

### HVML 的由来

随着互联网技术和应用的发展，围绕 HTML/CSS/JavaScript 发展出来的 Web 前端开发技术发展迅猛，甚至可以用“一日千里”来形容。五年前，基于 jQuery 和 Bootstrap 的前端框架大行其道，而从 2019 年开始，基于虚拟 DOM 技术的框架又受到前端开发者的青睐，比如著名的 React.js（<https://reactjs.org/>）、Vue.js（<https://cn.vuejs.org>）等。值得注意的是，微信小程序、快应用等，也不约而同使用了这种虚拟 DOM 技术来构建应用框架。

所谓“虚拟 DOM” 是指前端应用程序通过 JavaScript 来创建和维护一个虚拟的文档对象树，应用脚本并不直接操作真实的 DOM 树。在虚拟 DOM 树中，通过一些特别的属性实现了基于变量的一些流程控制，如条件、循环等。虚拟 DOM 技术提供如下一些好处：

1. 由于脚本并不使用脚本程序直接操作真实的 DOM 树，故而一方面通过现有的框架简化了前端开发的复杂性，另一方面通过优化对真实 DOM 树的操作而减少了由于动态修改页面内容而对 DOM 树的频繁操作，从而提高了页面的渲染效率和用户体验。
1. 通过虚拟 DOM 技术，程序对某个数据的修改，可以直接反应到该数据绑定的页面内容上，开发者无需主动或直接地调用相关接口来操作 DOM 树。这种技术提供了所谓的“响应式”编程，极大降低了开发者的工作量。

以 React.js、Vue.js 为代表的前端框架取得了巨大成功。看起来还是互联网公司钱多人多，所以可以肆无忌惮地不停地造各种各样的轮子。

然而，嵌入式或者物联网设备上的应用开发者却无法享受这些技术进步带来的红利。要开发嵌入式或者物联网设备上的应用，绝大多数情况下还是要使用 C/C++ 这种编程语言。虽说现在也有一些物联网操作系统尝试在系统中引入 JavaScript 或者 Python 语言的支持，但仅仅一个编程语言是不够的。比如博大精深的 Web 前端技术中，JavaScript 的确扮演了重要的角色，但 Web 前端技术赖以运转的基础却是 DOM 以及 CSS：DOM 提供了一种描述文档或者界面中的元素及其属性、内容的数据结构，而 CSS 描述了元素的布局、样式以及动画效果等渲染特性。一句话，那精妙的 Web 页面效果，不是通过 JavaScript 描述的，而是由 DOM 和 CSS 决定的。

所以，我们若想要在非浏览器环境中利用 Web 前端技术带来的便利来开发 GUI 应用，最该引入的首先应该是 DOM 和 CSS，而不仅仅是对一个脚本语言的支持。

除了嵌入式或者物联网应用的开发者之外，使用非 JavaScript 语言的其他开发者，比如使用 Python 做人工智能、大数据分析的开发者，也无法轻松使用 Web 前端技术带来的好处。若要用，非要折腾一番不可，比如要做个计算结果的可视化，要么使用 Python 的各种 GUI 绑定，要么绕一圈喂数据给浏览器，而且还要多学一门脚本语言。期间辛苦，也是罄竹难书。

笔者开发 MiniGUI 二十多年，知道使用 C/C++ 如何开发界面，多写几行 C/C++ 代码也能做出很多精妙的界面来。但有那么几年，笔者搞了几次 Web 前端开发，被 Web 前端技术开发界面的精妙之处所折服。后来再用 MiniGUI 开发界面，笔者心里也是百般地不情愿。要不是为了钱，都 2020 年了，谁愿意用 C/C++ 开发 GUI？居然还有人要做“国产”的 C++ GUI 系统替代 Qt。呸！那都是二十年前的技术了。要搞国产替代，你先往前看看行吗？

但是，要想在嵌入式系统或者物联网设备中支持完整的 Web 前端技术，所需要的存储尺寸和运行内存都比较大，要获得好的效果，还要有 GPU 的支持。所以在合璧操作系统（HybridOS）的开发中，我们通过一些技术上的小突破来改造 Web 前端技术，使开发者可以用更少的 JavaScript 代码来完成更多的工作。但在 HybridOS 的开发过程中，我们意识到对 Web 前端技术的修修补补只能是权宜之计——我们需要一种新的开发框架，尤其是编程语言来打破 Web 前端技术现有的框框和限制。

于是就有了 HVML。如果总结一下的话，HVML 的诞生有其历史使命：

1. 通过完备、自恰、高度抽象的新编程语言来进一步归纳和总结 React.js、Vue.js 等围绕虚拟 DOM 技术所做一些技术尝试。
1. 打破 Web 前端技术和 JavaScript 的耦合，让其他编程语言，比如 Python、Lua、C/C++ 等，也可以直接使用 Web 前端技术带来的便利。
1. 为传统的 GUI 开发，包括设计工具、开发框架等带来新的变革。
1. 为云环境中的物联网应用开发带来新的可能。

这就是 HVML 的由来。未来，HVML 将成为 HybridOS 的 App 开发首选编程语言。

## HVML 概貌

下面用一个简单的例子来描述 HVML 的基本长相。有关 HVML 的详细规范，感兴趣的读者可点击文末的原文链接。

下面的示例 HVML 代码生成的 HTML 页面，将在屏幕上展示三组信息：

1. 在页面顶端显示的系统状态栏，用于展示当前时间、WiFi 信号强度、电池电量信息等。这些信息将通过监听来自系统的状态事件动态更新。
1. 在页面中间位置展示用户列表，每个用户项包括用户名称、头像等信息。这些信息来自 JSON 表达的一个字典数组。当使用者点击了某个用户头像后，该 HVML 代码将装载一个模态对话框，其中展示该用户的更多信息。
1. 在页面底部展示一个搜索引擎连接。具体的搜索引擎根据系统所在的语言地区（locale）信息确定。

```html
<!DOCTYPE hvml>
<hvml target="html" script="python" lang="en">
    <head>
        <init as="users">
            [
                { "id": "1", "avatar": "/img/avatars/1.png", "name": "Tom", "region": "en_US" },
                { "id": "2", "avatar": "/img/avatars/2.png", "name": "Jerry", "region": "zh_CN" }
            ]
        </init>

        <init as="_TIMERS" uniquely by="id">
            [
                { "id" : "foo", "interval" : 500,  "active" : "yes" },
                { "id" : "bar", "interval" : 1000, "active" : "no" },
            ]
        </init>

        <listen on="hibus://system/status" as="systemStatus" />
    </head>

    <body>
        <archetype id="user-item">
            <li class="user-item" id="user-$?.id" data-value="$?.id" data-region="$?.region">
                <img class="avatar" src="$?.avatar" data-value="$?.id" />
                <span>$?.name</span>
            </li>
        </archetype>

        <archedata id="item-user">
            {
                "id": "$?.attr.data-value", "avatar": "$?.content[0].attr.src",
                "name": "$?.children[1].textContent", "region": "$?.attr.data-region"
            },
        </archedata>

        <header id="theStatusBar">
            <img class="mobile-status" src="" />
            <span class="mobile-operator"></span>
            <img class="wifi-status" src="" />
            <span class="local-time">12:00</span>
            <img class="battery-status" />>
        </header>

        <ul class="user-list">
            <iterate on="$users" with="#user-item" to="append" by="CLASS: IUser">
                <nodata>
                    <img src="wait.png" />
                </nodata>
                <except on="StopIteration">
                    <p>Bad user data!</p>
                </except>
            </iterate>
        </ul>

        <archetype id="footer-cn">
            <p><a href="http://www.baidu.com">Baidu</a></p>
        </archetype>

        <archetype id="footer-tw">
            <p><a href="http://www.bing.com">Bing</a></p>
        </archetype>

        <archetype id="footer-def">
            <p><a href="http://www.google.com">Google</a></p>
        </archetype>

        <footer id="the-footer">
            <test on="$_SYSTEM.locale" in='the-footer'>
                <match for="~zh_CN" to="displace" with="#footer-cn" exclusively>
                </match>
                <match for="~zh_TW" to="displace" with="#footer-tw" exclusively>
                </match>
                <match for="*" to="displace" with="#footer-def">
                </match>
                <error on="nodata">
                    <p>You forget to define the global variable!</p>
                </error>
                <except on="KeyError">
                    <p>Bad global data!</p>
                </except>
                <except on="IdentifierError">
                    <p>Bad archetype data!</p>
                </except>
            </test>
        </footer>

        <observe on="$_TIMERS" for="foo" to="update" in="#the-header" >
            <update on="> span.local-time" textContent="$_SYSTEM.time('%H:%m')" />
        </observe>

        <observe on="$systemStatus" for="battery" to="test">
            <test on="$?.level" in="#the-header">
                <match for="100" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-full.png" />
                </match>
                <match for=">90" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-90.png" />
                </match>
                <match for=">70" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-70.png" />
                </match>
                <match for=">50" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-50.png" />
                </match>
                <match for=">30" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-30.png" />
                </match>
                <match for=">10" to="update" exclusively>
                    <update on="img.mobile-status" attr.src="/battery-level-10.png" />
                </match>
                <match for="*" to="update">
                    <update on="img.mobile-status" attr.src="/battery-level-low.png" />
                </match>
            </test>
        </observe>

        <observe on=".avatar" for="clicked" to="load">
            <load on="user.hvml" with="{'id': $@.attr['data-value']}" as="_modal" />
        </observe>
    </body>
</hvml>
```

熟悉 HTML 读者一定会觉得上面的代码很眼熟。是的，和 HTML 类似，HVML 使用标签（tag）；但和 HTML 不同的是，HVML 是动态的，表述的是程序，而 HTML 是静态的，表述的是文档。

我们对照上述示例，介绍 HVML 的一些特点。

首先是数据驱动编程。通过基于数据的迭代、插入、更新、清除等操作，开发者不需要编写程序或者只要少量编写程序即可动态生成最终的 XML/HTML 文档。比如下面的上面示例代码中的 `iterate` 标签，就在`$users` 变量代表的数据（在 `header` 中使用 `init` 标签定义）上做迭代，然后在最终文档的 `ul` 元素中插入了若干 `li` 元素。而 `li` 元素的属性（包括子元素）由一个 `archetype` 标签定义，其中使用 `$？` 来指代被迭代的 `$users` 中的一个数据单元。

在上面的示例代码中，我们使用了系统内置变量 `$_TIMERS` 来定义定时器，每个定时器有一个全局的标识符，间隔时间以及是否激活的标志。如果要激活一个定时器，我们只需要使用 HVML 的 `update` 或 `set` 标签来修改对应的键值，而不需要调用某个特定的编程接口。这是数据驱动编程的另一个用法——我们不需要为定时器或者其他的类似模块的操作提供额外的 API。

另外，在上面的示例代码中，我们通过 `observe` 标签观察新的数据或文档本身的变化以及用户交互事件，可实现 XML/HTML 文档或数据的动态更新。比如在最后一个 `observe` 标签中，通过监听用户头像上的点击事件来装载一个新的 `user.hvml` 文件，以模态对话框的形式展示对应用户的详细信息。

我们将这种编程方式称为数据驱动的编程（data-driven programming）。

其次是界面、交互和数据之间的耦合。通过 HVML 引入的编程模型和方法，用于表述界面的 XML/HTML 文档内容可完全由 HVML 生成和动态调整，这避免了在程序代码中直接操作文档的数据结构（即文档对象树，或简称 DOM 树），而程序只需要关注数据本身的产生和处理即可。这样，就实现了界面和数据的解耦。比如：
   - HVML 可在文档片段模板或者数据模板中定义数据和 DOM 元素之间的映射关系（如示例代码中的 `archetype` 或 `archedata` 标签），而无需编写额外的代码完成数据到 DOM 元素属性、内容等的赋值操作。
   - HVML 将错误和异常的展现和程序代码分离了开来，程序只要产生适当的错误或者异常（如示例代码中的 `error` 和 `except` 标签），而对错误或者异常的处理则直接在 HVML 中定义，这不仅仅将程序和界面隔离了开来，同时还提高了代码的可维护性。

再次是数据的 JSON 表达。HVML 对文档和数据的操作提供了一致接口。HVML 要求所有外部数据均使用 JSON 格式表述，JSON 格式是一种人机共读的数据表述形式，可在数值、字符串、数组、字典等基本数据单元的基础上表述复杂对象。由于 HTML/XML 文档的片段可表述为 JSON 格式的数据，因此，HVML 亦可用于操作使用 JSON 表述的数据。在 HVML 中，我们甚至提供了对动态 JSON 对象的支持，我们可以使用外部脚本程序来实现自己的动态 JSON 对象，且可以在这些对象上执行类似函数一样的功能。

最后，HVML 通过动作标签（通常都是一些英文的动词，如 `init`、`set`、`update`、`iterate` 等）以及与之配合的介词或副词属性来定义动作标签所依赖的数据、目标操作位置以及执行条件来完成特定的文档操作。这和常见的编程语言有很大不同，HVML 的描述方式更加贴近自然语言，从而可以大幅降低学习门槛。

限于篇幅，我们不打算在本文中详细介绍 HVML，读者对 HVML 有个感性认识就可以了。有兴趣了解详细规范的读者，可以参阅文末的原文链接。不过要耐心点哦，定义一个完备、自恰的编程语言不是一件容易的事儿，所以要读，就要找个大段的事件，耐心点儿仔细地阅读。

## HVML 如何改变传统的 GUI 开发

我们假设有一个 GUI 系统，使用 XML 来描述界面上的构件（widget）。现在，我们要使用这个 GUI 系统开发一个简单的文件打开对话框，大致的界面需求如下：

1. 有一个列表框（Listbox），其中列出了当前路径下的目录及文件（统称为目录项）。用户可使用鼠标或者键盘切换列表框中的当前选中项，并产生一个选中项改变的事件。
1. 在该列表框的顶部，有一个文本标签（Label），其中显示了当前的路径。
1. 当用户点击列表框下面的“Open"按钮（Button）时，若列表框的当前选中项是目录，则进入这个目录，修改用于显示当前路径的文本标签内容，并使用新路径下的目录项填充列表框，若当前选中项是一个文件，则返回选中的这个文件。

对上述界面和交互需求，我们通常可以使用如下的 XML 文件描述：

```xml
<ui>
    <label id="path">
        /home
    </label>

    <listbox id="entries">
        <item class="dir">..</item>
        <item class="dir">vincent</item>
        <item class="dir">david</item>
        <item class="file">README.txt</item>
    </listbox>

    <button id="open">
        Open
    </button>
</ui>
```

注意：为简单起见，我们没有引入有关构件布局的描述信息。

为满足以上的交互处理需求，我们使用 HVML 来描述这个界面的动态生成以及交互过程：

```html
<!DOCTYPE hvml>
<hvml target="xml" script="python">
    <head>
        <init as="fileInfo">
            {
                "curr_path": "/home/", 
                "selected_type": "dir",
                "selected_name": "..",
            }
        </init>
    </head>

    <body>
        <label id="path">
            $fileInfo.curr_path
        </label>

        <archetype id="dir-entry">
            <item class="$?.type">$?.name</item>
        </archetype>

        <define as="fillDirEntries">
            <choose on="$?" to="iterate" by="CLASS: CDirEntries">
                <iterate on="$?" to="append" in="#entries" with="#dir-entry" by="RANGE: 0">
                </iterate>
            </choose>
        </define>

        <listbox id="entries">
            <call on="$fillDirEntries" with="$fileInfo.curr_path" />
        </listbox>

        <button id="open">
            Open
        </button>

        <observe on="$entries" for="selected-item-changed">
            <update on="$fileInfo" key.selected_type=$?.type key.selected_name=$?.name />
        </observe>

        <observe on="$open" for="click">
            <test on="$fileInfo.selected_type">
                <match for="dir" to="empty call update update" exclusively>
                    <init as="new_path">
                        "$fileInfo.curr_path{$2.name}/"
                    </init>

                    <empty on="#entries" />
                    <call on="$fillDirEntries" with="$new_path" />
                    <update on="$fileInfo" key.curr_path=$new_path />
                    <update on="#path" textContent="$new_path" />
                </match>
                <match for="file" exclusively>
                    <back to="_caller" with="$fileInfo">
                </match>
            </test>
        </observe>
    </body>
</hvml>
```

下面对上述 HVML 代码的重点部分做一些说明。

首先，该代码使用了一个全局的 `$fileInfo` 变量来记录当前路径（初始为 `/home/`）以及当前选中目录项的类型（初始为 `dir`）和名称（初始为 `..`）。当用户在列表框中选择新的目录项时，会观察到 `selected-item-changed` 事件，并更新 `$fileInfo` 中的 `selected_type` 和 `selected_name` 键值。该事件的 `payload` 键值示例如下：

```json
    {
        "type": "dir",
        "name": "david",
    }
```

其次，该代码使用了 `choose` 元素以及一个外部执行器（`CLASS: CDirEntries`）来获得当前路径中的所有目录项。返回的结果数据大致为：

```json
    [
        { "type": "dir", "name": "david" },
        { "type": "dir", "name": "vincent" },
        { "type": "file", "name": "README.txt" },
    ]
```

在上述结果之上，使用 `iterate` 元素填充列表框。

最后，当用户点击 `Open` 按钮时，上述代码会观察到 `clicked` 事件。在处理该事件时，通过检查 `$fileInfo.selected_type` 来完成相应的工作：

- 如果当前选中的目录项类型是目录，则切换到该目录。此时，会首先清空列表框，然后再使用新路径下的目录项填充列表框。
- 如果当前选中的目录项类型是文件，则使用 `back` 标签返回上个页面，同时返回 `fileInfo` 数据。

在上述代码中，外部选择器 `CDirEntries` 的实现非常简单，就是列出给定路径下的目录项，并按照要求返回一个字典数组。使用 Python 实现时非常简单，所以这里略去不谈。

如果我们使用 HybridOS 中提到的直接执行本地系统命令的扩展图式（lcmd），我们甚至都不需要编写任何代码，而只需要使用 `request`：

```html
        <init as="lcmdParams">
            { "cmdLine": "ls $fileInfo.curr_path" }
        <init>

        <requset on="lcmd:///bin/ls" with="$lcmdParams">
            <iterate on="$?" to="append" in="#entries" with="#dir-entry" by="RANGE: 0">
            </iterate>
        </request>
```

如此，开发者不需要做编写任何程序，即可实现一个简单的文件浏览和打开对话框。

## HVML 的未来

## 总结

本文所描述的 HVML，是一种通用、完备、优雅的数据驱动动态标记语言。其主要优点可总结如下：

1. 通过为数不多的动作标签定义了数据驱动的 HTML/XML 文档生成规则，避免使用基于流程控制的传统编程方法，开启了一种新的低代码编程模式。
1. 通过动作标签的介词属性和副词属性，规定了执行动作所需要的数据和动作类型以及规则，便于开发者理解和掌握，从而降低了学习门槛。
1. 为除了 JavaScript 脚本语言之外的其他脚本语言（或编程语言），提供了利用 Web 技术（HTML、CSS、HTTP、WebSocket 等）开发应用程序的框架和设施。
1. 通过丰富的内建执行器，通过诸如 KEY、RANGE、TRAVEL、SQL 等语句在元素和数据上执行迭代、过滤、排序、规约等操作，使开发者可以专心于业务逻辑的实现，而非具体的算法。
1. 通过外部执行器，为复杂数据的处理提供了使用外部脚本或者模块实现相应功能的方法，提供了可扩展性。
1. 通过绑定外部程序模块，提供了可扩展、灵活的动态 JSON 对象实现方法，结合本文定义的 JSON 求值表达式，可用于满足各种基于函数调用的计算需求。
1. 解决了构建在现有 Web 技术之上的虚拟 DOM 技术存在的打补丁式解决方案引入的问题，比如代码的可读性降低，结构不清晰等问题。

[Beijing FMSoft Technologies Co., Ltd.]: https://www.fmsoft.cn
[FMSoft Technologies]: https://www.fmsoft.cn
[HybridOS Official Site]: https://hybrid.fmsoft.cn

[MiniGUI]: http:/www.minigui.com
[WebKit]: https://webkit.org
[HTML 5.3]: https://www.w3.org/TR/html53/
[DOM Specification]: https://dom.spec.whatwg.org/
[WebIDL Specification]: https://heycam.github.io/webidl/
[CSS 2.2]: https://www.w3.org/TR/CSS22/
[CSS Box Model Module Level 3]: https://www.w3.org/TR/css-box-3/

[HybridOS Architecture]: HybridOS-Architecture
[HybridOS Code and Development Convention]: HybridOS-Code-and-Development-Convention

