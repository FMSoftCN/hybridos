# 合璧操作系统的大胆技术突破（2）

魏永明

本文是《合璧操作系统的大胆技术突破》系列文章的第二篇。主要阐述我们在 hiWebKit 中新增的直接获取本地命令行执行结果以及获得本地或者远程数据库查询结果的特性。有关本系列的其他文章，可点击上面的专辑链接查阅。

## 问题和需求

熟悉浏览器的人知道，WebKit 是苹果 Safari 浏览器使用的引擎，谷歌 Chrome 浏览器使用的引擎也发端于此。作为合璧操作系统核心组件的 hiWebKit，是在苹果维护的最新 WebKit 引擎基础上做了诸多小的突破，其目的是为了支持在物联网或者嵌入式系统中运行合璧操作系统时，可以非常方便地整合本地的操作系统基础功能。

长期以来，人们在尝试着使用 Web 技术来构建可在本地运行的 App。常用的方案有传统的插件技术和集成 Node.js 这两类。

### 传统插件技术

我们使用 HTML 的 `object` 或 `embed` 元素，可在网页中集成由其他软件组件提供的功能。其典型用法就是 Java Applet，如下所示：

```html
  <object type="application/x-java-applet">
    <param name="code" value="MyJavaClass">
    <p>You do not have Java available, or it is disabled.</p>
  </object>
```

在浏览器中，对 `object` 或 `embed` 元素的支持通常通过插件实现。比如，当用户代理集成了 Java 支持插件后，上述 HTML 代码将创建一个 `object` 元素，并在其中展示 Java Applet 的内容，用户甚至可以和 Java Applet 执行交互。

然而，这种方案存在如下不足：

- 和 `canvas` 的实现类似，HTML 文档中的其他元素和 `object` 元素之间存在数据交互鸿沟。根据 HTML 规范的描述，HTML 页面中的其他模块（如脚本语言）只能通过修改 `param` 元素的属性来向 `object` 元素中的对象传递一些参数，但 `object` 元素所代表的对象之状态改变，却无法反馈给脚本或者其他 HTML 元素，比如集成到表单当中。
- 通过 `object` 或者 `embed` 元素实现的对象，主要用来支持复杂外部内容的渲染或者用户交互，比如 Java Applet、Abode Flash 等，和 HTML 相比，基本上属于另外一套自包含（self-contained）的技术。比如插件对象要使用字体来渲染文本，插件实现代码中就需要包含有字体的支持代码，而无法使用用户代理本身已经提供的字体支持功能。

目前，常用的浏览器插件机制使用 NPAPI（Netscape Plugin API，网景插件编程接口）。Google Chrome 在其官方网站上如是评价 NPAPI（见网页：<https://developer.chrome.com/extensions/npapi>）：

> NPAPI is a really big hammer that should only be used when no other approach will work...

大意就是 NPAPI 真是个大锤子，应该只在没有其他办法的情况下使用。因此，Google Chrome 提供了重新定义的浏览器扩展接口。Google Chrome 的扩展机制提供的是 JavaScript 接口，插件主要通过 `chrome` 对象来实现对应的扩展功能。因此，Chrome 的扩展机制和基于画布和脚本的机制有着相同或者类似的缺陷和不足，且其主要实现目标并不是为了支持复杂控件，而是为了扩展浏览器的功能。比如，通过分析 HTML 文档中的某些特别元素，过滤页面中的广告内容。

### JavaScript 本地绑定

我们也可以通过扩展 JavaScript 本地绑定对象的方法，为 Web App 提供更多可以访问本地系统数据或功能的能力。比如著名的 PhoneGap（<https://www.phonegap.com/>），为移动端的 Web App 提供了大量的 JavaScript 本地绑定对象，从而可以让开发者在 Web App 中使用它提供的接口来操作本地系统对象。如：

- 图库
- 联系人
- 摄像头（拍摄照片或视频）
- 电池状态
- 加速度传感器
- 指南针
- ...

微信为运行在微信内嵌浏览器中的网页也提供了类似 PhoneGap 的 JavaScript 本地绑定对象，称为 JS-SDK(<https://developers.weixin.qq.com/doc/offiaccount/OA_Web_Apps/JS-SDK.html>)。除了提供类似 PhoneGap 的通用功能之外，还提供了微信支付、扫一扫等功能。

### 集成 Node.js

在桌面操作系统上，当我们利用网页前端技术实现 Web App 时，为了最大程度上获得对系统中各种资源的访问能力，可通过集成一个运行在本地的 HTTP 服务器（如 Node.js）来实现此类功能。如开源的跨操作系统桌面 Web App 的支持框架 Electron（<https://electronjs.org/>）就是一个典型。

### 上述方案存在的问题

无论使用传统插件、JavaScript 本地绑定还是使用 Electron 这种方案，存在如下缺陷或不足：

1. 无法和已有的 HTML 文档元素之间形成有机的整合，均需要额外的代码来处理。如传统插件机制。
1. 系统架构复杂，不利于 Web App 在内存受限系统（如嵌入式、物联网设备）中的应用。如 Electron。
1. JavaScript 是解释性编程语言，执行效率相对较低，且占用内存大。如 JavaScript 本地绑定。
1. 需要编写额外的脚本程序，这加大了开发人员的工作量，不符合低代码开发趋势。

## hiWebKit 的解决方案

在 hiWebKit 中，我们通过引入新的类似 HTTP 这样的 URL 图式（schema）来获得对本地资源的访问。为此，我们提供了两种新的图式：

1. 用于访问 SQL 数据库的扩展图式
1. 用于执行本地系统命令的扩展图式

以下分别介绍这两种新的图式。

### 用于访问 SQL 数据库的扩展图式

假定我们在本地有一个 SQL 数据库，可通过如下 URL 在其上执行 SQL 查询：

```
lsql:///data/testdb/db?sqlQuery=SELECT%20a%2C%20b%2C%20sum(c)%20FROM%20tab1%20GROUP%20BY%20a%3B
```

上述 URL 指定了如下信息：

1. `lsql` 是合璧引入的新的 URL 图式（schema），用于指定本 URL 将直接在本地的 SQL 数据库上执行 SQL 查询；
1. `/data/testdb` 指定了本地 SQL 数据库文件的路径；
1. `db` 指定了其后要执行的 SQL 语句的目标数据库名称；
1. `sqlQuery` 参数的值是一条经过 URI 编码（对 URL 中特殊字符，如空格、逗号、分号等做转义处理）后的 SQL 语句，解码后的内容为：

```
SELECT a, b, sum(c) FROM tab1 GROUP BY a;
```

或者我们也可以如此描述：上述 URL 发起了一个 `lsql` 请求（或 LSQL 请求），其目标资源为本地数据库文件 `/data/testdb` 中的 `db` 数据库，要执行的 SQL 查询语句为 `SELECT a, b, sum(c) FROM tab1 GROUP BY a;`。

假定我们在远程服务器（`foo.bar.com`）上运行有一个 SQL 数据库（如 MySQL、PostgreSQL、Oracle、DB2、SQL Server 等），则可通过如下 URL 在其上执行 SQL 查询：

```
rsql://username:password@foo.bar.com:3306/mysql/db?sqlQuery=SELECT%20a%2C%20b%2C%20sum(c)%20FROM%20tab1%20GROUP%20BY%20a%3B
```

上述 URL 指定了如下信息：

1. `rsql` 是合璧引入的新的 URL 图式（schema），用于指定本 URL 将直接在远程 SQL 数据库上执行 SQL 查询；
1. `username` 是用户账号名称；
1. `password` 是用户账号对应的密码；
1. `foo.bar.com` 是运行 SQL 数据库的主机名称；
1. `3306` 是 SQL 数据库提供 SQL 查询服务的端口号；
1. `/mysql/db` 指定了远程 SQL 数据库的类型以及其后要执行的 SQL 语句的目标数据库名称；
1. `sqlQuery` 参数的值是一条经过 URI 编码（对 URL 中特殊字符，如空格、逗号、分号等做转义处理）后的 SQL 语句，解码后的内容同上。

或者我们也可以如此描述：上述 URL 发起了一个 `rsql` 请求（或者 RSQL 请求），其目标资源为远程主机 `foo.bar.com` 上的数据库 `db`（数据库类型为 `mysql`），要执行的 SQL 查询语句为 `SELECT a, b, sum(c) FROM tab1 GROUP BY a;`。该远程主机在端口 `3306` 上处理 `rsql` 请求，连接使用的用户账号名称和密码分别为：`username`、`password`。

另外，如 `https` 是 `http` 的加密传输协议一样，`rsqls` 是 `rsql` 的加密图式（`lsql` 图式不需要支持加密传输）。

其次，除了使用 GET 方法通过 URL 的查询（query）部分来传递参数之外，`lsql` 等图式也支持使用 POST 方法传递参数。

注意，当 `sqlQuery` 参数缺失时，`lsql` 等请求将不执行任何 SQL 查询，但会检查指定的数据库是否存在。

针对以上新的 URL 图式（`lsql`、`rsql`、`rsqls`），用户代理应按照新的协议进行处理，并将结果转换成 JSON 格式（MIME 类型为 `application/json`）。所有的 SQL 查询结果均应包括如下 JSON 字段信息：

- `statusCode`: 整数值，类似 HTTP 状态码，如 200 表示成功执行，404 表示不存在的目标数据库。
- `errorMsg`: 字符串或 NULL，表示 SQL 查询语句的执行错误信息，NULL 时表示没有错误。
- `rowsAffected`：受影响的记录行数；当执行更新（Update）、删除（Delete）等操作时，将通过该字段返回受影响的记录行数。在执行检索（Retrieve）操作时，该值通常为 0。
- `rows`：用于返回结果集；当执行检索（Retrieve）时，通过该字段返回结果集，没有结果集时取 NULL，结果集使用数组来表示所有的行，每一行用字典数据结构（默认）或者数组表示。

假定我们执行如下 SQL 语句在一个 SQLite 数据库文件中创建了一个用于记录书本信息的 `Books` 表，并在其中插入了三本书的标题、作者以及 ISBN 编号信息：

```sql
CREATE TABLE Books(Id integer PRIMARY KEY, Title text, Author text, Isbn text default 'not available');
INSERT INTO Books VALUES(1, 'War and Peace', 'Leo Tolstoy', '978-0345472403');
INSERT INTO Books VALUES(2, 'The Brothers Karamazov', 'Fyodor Dostoyevsky', '978-0486437910');
INSERT INTO Books VALUES(3, 'Crime and Punishment', 'Fyodor Dostoyevsky', '978-1840224306');
```

然后使用如下的 URL：

```
lsql:///data/testdb/db?sqlQuery=SELECT%20*%20FROM%20Books%3B
```

将在该数据库上执行如下 SQL 语句：

```sql
SELECT * FROM Books;
```

该 URL 返回的 JSON 数据如下所示：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "rowsAffected": 0,
    "rows": [
        { "Id": 1, "Title": "War and Peace",          "Author": "Leo Tolstoy",        "Isbn": "978-0345472403" },
        { "Id": 2, "Title": "The Brothers Karamazov", "Author": "Fyodor Dostoyevsky", "Isbn": "978-0486437910" },
        { "Id": 3, "Title": "Crime and Punishment",   "Author": "Fyodor Dostoyevsky", "Isbn": "978-1840224306" }
    ]
}
```

当我们在该数据库上执行如下 SQL 语句删除第一条记录时：

```sql
DELETE FROM Books WHERE Id=1;
```

对应的 URL 为：

```
lsql:///data/testdb/db?sqlQuery=DELETE%20FROM%20Books%20WHERE%20Id%3D1%3B
```

其结果为：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "rowsAffected": 1,
    "rows": null
}
```

如果我们一次执行多条 SQL 语句，则使用 `result` 数组来表示每条语句的执行结果。比如，我们一次执行两条 SQL 语句（先执行 `DELETE` 语句再执行 `SELECT` 语句），其 URL 为：

```
lsql:///data/testdb/db?sqlQuery=DELETE%20FROM%20Books%20WHERE%20Id%3D1%3B%20SELECT%20*%20FROM%20Books%3B
```

其结果为：

```json
{
    "statusCode": 200,
    "result": [
        {
            "errorMsg": null,
            "rowsAffected": 1,
            "rows": null
        },
        {
            "errorMsg": null,
            "rowsAffected": 0,
            "rows": [
                { "Id": 2, "Title": "The Brothers Karamazov", "Author": "Fyodor Dostoyevsky", "Isbn": "978-0486437910" },
                { "Id": 3, "Title": "Crime and Punishment",   "Author": "Fyodor Dostoyevsky", "Isbn": "978-1840224306" }
            ]
        }
    ]
}
```

为方便对 `lsql` 等请求的结果集作后续处理，除了 `sqlQuery` 参数用于传递 SQL 查询语句之外，合璧引入另外一个参数 `sqlRowFormat`，用于指定结果集的行输出格式，可取 `dict` 和 `array` 两个值之一，后者表示使用数组作为结果集的行输出格式。不指定 `sqlRowFormat` 或指定错误的值时，采取字典数据结构。比如，对上述结果，如果 `lsql` URL 中包含 `sqlRowFormat=array` 参数，则返回的结果为：

```json
{
    "statusCode": 200,
    "result": [
        {
            "errorMsg": null,
            "rowsAffected": 1,
            "rows": null
        },
        {
            "errorMsg": null,
            "rowsAffected": 0,
            "rows": [
                [ 2, "The Brothers Karamazov", "Fyodor Dostoyevsky", "978-0486437910" ],
                [ 3, "Crime and Punishment",   "Fyodor Dostoyevsky", "978-1840224306" ]
            ]
        }
    ]
}
```

为了方便在 HTML 文档中通过表单元素的名称和值构成不同的 SQL 语句，我们对作为参数传递的 SQL 语言做变量化处理：

1. 使用 `$` 作为前缀定义 SQL 语句中的变量，如 `$Id`，表示在生成最终的 SQL 查询语句时，该变量要使用同时传递的同名 URL 参数替代。
1. 使用其他 URL 参数定义该变量的值。

如：

```
lsql:///data/testdb/db?sqlQuery=DELETE%20FROM%20Books%20WHERE%20Id%3D$Id%3B&Id=1
```

上述 URL 包含两个参数：

- `sqlQuery`：对应的值是一个变量化 SQL 语句，解码后为：`DELETE FROM Books WHERE Id=$Id;`。
- `Id`：对应的值是1。这组参数将用来替代上面变量化 SQL 语句中的 `$Id`。

故上述 URL 最终执行的 SQL 语句为：

```sql
DELETE FROM Books WHERE Id=1;
```

我们通过变量化 SQL 语句，有利于通过表单中的输入项来设定 SQL 语句中的不同值，而变量化的 SQL 语句可以保持不变，从而降低编码量。而如果不使用变量化的 SQL 语句，我们将需要额外的脚本代码来生成最终的 SQL 语句。

再如，我们要从 `Books` 表中查找所有 `Fyodor Dostoyevsky` 的书，并使用 ISBN 对结果进行排序，则对应的 SQL 语句应该为：

```sql
SELECT Title, Isbn FROM Books WHERE Author='Fyodor Dostoyevsky' SORT BY Isbn ASC;
```

但我们可以对上述 SQL 做变量化处理：

```sql
SELECT Title, Isbn FROM Books WHERE Author='$author' SORT BY $sortBy $sortOrder;
```

之后在传递 URL 参数时，通过 URL 参数传递真实的变量值：

```
lsql:///data/testdb/db?sqlQuery=SELECT%20Title%2C%20Isbn%20FROM%20Books%20WHERE%20Author%3D'%24author'%20SORT%20BY%20%24sortBy%20%24sortOrder%3B&author=Fyodor%20Dostoyevsky&sortBy=Isbn&sortOrder=ASC
```

变量化 SQL 语句中的变量名称需满足如下规范：

1. 以 `$` 字符开头，紧随其后的字符必须为字母，且
1. 其后只包含数字、字母和下划线，且
1. 任何非数字、字母和下划线字符表示变量名称的终结。
1. 保留 `$` 字符后为数字的情形，可用作以顺序指代参数。
1. 变量名称是大小写敏感的；也就是说 `$abc` 和 `$Abc` 表示不同的变量。

为了正确处理 SQL 语句中原本含有 `$` 字符的情形，对 `$` 字符作转义处理：

1. 对原 SQL 语句中按以上规则会被处理为变量名称的，使用 `$$` 做转义处理。
1. 若按以上规则不会处理为变量名称的，无须做转义处理。

如下面的原始 SQL 语句：

```sql
SELECT * FROM Books WHERE Code='$name$15$'
```

需要做如下转义处理：

```sql
SELECT * FROM Books WHERE Code='$$name$$15$'
```

原始 SQL 语句中的最后一个 `$` 字符，由于不构成变量化 SQL 语句中的变量名称，故而无须转义。

需要注意的是，由于在 `lsql` 等 URL 请求中，我们已经使用了 `sqlQuery` 和 `sqlRowFormat` 这两个参数名称，因此，其他请求参数要避免使用这两个参数名称。

当使用 `lsql` 等图式的 URL 作为表单提交的 `action` 属性时，由于 `lsql` 等图式返回的内容是 JSON 格式的，并不能直接提供给用户代理展示。因此，我们需要同时定义一个正常的结果页 URL。

如下所示：

```html
    <form action="lsql:///data/testdb/db" method="get" destination="/books.html">
        <input type="hidden" name="sqlQuery" value="DELETE FROM Books WHERE Id=$Id" />
        <input type="hidden" name="Id" value="1" />
        <input type="submit" value="Delete" />
    </form>
```

我们在上面的 `form` 元素中使用 `destination` 属性定义了目标 URL。当表单提交成功（即 `lsql:///data/testdb/db?sqlQuery=...` 的状态码为 2xx 时），用户代理应该导航到 `destination` 属性定义的目标 URL 上。

### 用于执行本地系统命令的扩展图式

假定要执行本地系统中 `ls` 命令（路径为 `/bin/ls`），可通过如下 URL 运行该程序并获取其输出：

```
lcmd:///bin/ls?cmdFilter=delimiter('%20')&cmdLine=ls%20--color%3Dnever%20-l%20%2F
```

上述 URL 指定了如下信息：

1. `lcmd` 是合璧引入的新的 URL 图式（schema），用于指定本 URL 用于执行本地命令。
1. `/bin/ls` 指定了要运行的程序文件之路径。
1. `cmdFilter` 参数指定对命令行输出结果进行 JSON 化处理时的过滤器名称及参数；本例指定的过滤器为 `delimiter(' ')`，即列分割符过滤器，所使用的分割符在 `()` 中定义，上例中为空格（` `）。
1. `cmdLine` 参数指定命令行，经过 URI 编码（对 URL 中特殊字符，如空格、逗号、分号等做转义）处理，解码后的内容为：

```
ls --color=never -l /
```

也可以如此描述：上述 URL 发起了一个 `lcmd` 请求（或 LCMD 请求），其目标资源（可执行程序）为 `/bin/ls`，命令行为 `ls --color=never -l /`，命令输出的结果过滤器为 `delimiter(' ')`。

另外，除了使用 GET 方法通过 URL 的查询（query）部分来传递 `lcmd` 请求的参数之外，`lcmd` 请求也支持使用 POST 方法传递参数。

需要注意的是，`lcmd` 图式仅对命令行程序的标准输出进行处理，不对命令行程序的错误输出进行处理。

假如系统中存在一个成熟的 Shell 程序（如 Bash），我们也可以通过执行更为复杂的命令，如输出重定向、多条命令通过管道连接运行等。

比如，下面的命令将标准输出（文件描述符为 2）重定向到了标准输出（文件描述符为 1），从而可以让 `lcmd` 请求处理标准错误。

```
ls --color=never -l / 2>&1
```

对应的 `lcmd` URL 为：

```
lcmd:///bin/bash?cmdLine=ls%20--color%3Dnever%20-l%20%2F%202%3E%261
```

注意在上面的 URL 中，我们指定的目标资源位置为：`/bin/bash`；亦即，我们使用可执行程序 `/bin/bash` 来执行上述命令行。

使用 Bash 程序，我们还可以将多条命令通过管道符（`|`）连接起来，使前一个命令的输出成为后一个命令的输入，从而完成更为复杂的输出操作，比如下面的命令：

```
ls --color=never -l / | grep ^d --color=never
```

将列出文件系统根目录中所有的子目录，而忽略文件。

注：上述命令中的 `--color=never` 禁止了输出时的颜色控制字符，以方便后续处理可以获得干净的文本内容。

我们也可以使用 `lcmd` 图式来执行其他脚本程序，如 Python 脚本。

`ls -l` 命令会列出系统根目录下的目录项（文件及子目录），并给出目录项的创建日期、大小、所有者等信息。一般的输出形式如下：

```
total 152
drwxr-xr-x   2 root root  4096 Dec  2 15:29 bin
drwxr-xr-x 143 root root 12288 Dec  9 09:33 etc
```

为了便于 HTML 页面处理，合璧要求对上述输出进行 JSON 化处理。为此，我们通过在 `lcmd` URL 中指定过滤器实现该目的。

一个过滤器通常有一个名称以及一项参数或多项参数，比如 `delimiter('\t ')`，表示过滤器名称为 `delimiter`，分割符为制表符（`\t`）或空格（` `）。在 `lcmd` URL 中，我们还可以指定多个过滤器依次对命令行输出进行处理，多个过滤器之间用分号（`;`）分割。

合璧预定义如下几类过滤器：

1. 行分割过滤器：
   - `split(D)`：使用 `D` 中任意一个字符作为分割符分割行，该过滤器用于将单个行分割成多个行。`split('\t.')` 表示在行中遇到制表符（`\t`）或者句点（`.`）时会将当前行分割为两行。比如对 "Hello, world!" 执行 `split(' ')`，会将这行分割为 "Hello," 和 "world!" 两行。
   - `chars()`：将每个字符当作一行进行分割。
   - `letters()`：将每个字母当作一行进行分割，且忽略标点符号等特殊字符。如 "hello（你好！）" 会被分割为 "h"、"e"、"l"、"l"、"o"、"你"、"好" 等 7 行。该分割符基于语义，所以需要通过 `LANGUAGE` 参数传递语言编码，该编码使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
   - `words(LANGUAGE)`：将每个单词当作一行进行分割，且忽略标点符号等特殊字符。如 "hello, world!" 会被分割为 "hello"、"world" 两行。`LANGUAGE` 是用来表示语言的字符串，使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
   - `sentences(LANGUAGE)`：将每个句子当作一行进行分割，且忽略作为句子间的特殊字符（如空格），但保留标点符号。如 "hello, world! I love world!" 会被分割为 "hello, world!" 和 "I love world!" 两行。`LANGUAGE` 是用来表示语言的字符串，使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
1. 行截取过滤器：
   - `head(N)`：截取输出的前面 `N` 行。`head(0)` 等同于废弃所有行。
   - `tail(N)`：截取输出的后面 `N` 行。`tail(0)` 等同于废弃所有行。
   - `ignore(I,N)`：忽略从 `I` 开始的 `N` 行。`I` 表示行索引值，第一行的索引值为 0，最后一行用 `$` 表示；`N` 表示要忽略的行数，正值表示从 `I` 向后计算，负值表示从 `I` 向前计算。如 `ignore($,-10)` 表示忽略最后 10 行输出。`N` 为 0 将废弃所有行。
   - `pick(A, B)`：选择索引值匹配 `An + B` 公式的行，其中 `n` 是零或自然数。比如 `pick(2, 0)` 将选择第 0、2、4 等行；`pick(3, -1)` 将选择第 2、5、8 等行。
   - `cut(A, B)`：选择索引值不匹配 `An + B` 公式的行，其中 `n` 是零或自然数。比如 `cut(2, 0)` 将选择第 1、3、5 等行；`cut(3, -1)` 将选择第 0、1、3、4、6、7 等行。
1. 列分割过滤器：
   - `delimiter(D)`：使用 `D` 中任意一个字符作为分割符分割每一行输出，该过滤器用于将单个列分割成多个列。若未定义参数 `D` ，则使用空格为默认分割符。
   - `cchars()`：将每个字符当作一列进行分割。
   - `cletters()`：将每个字母当作一列进行分割，且忽略标点符号等特殊字符。如 "hello（你好！）" 会被分割为 "h"、"e"、"l"、"l"、"o"、"你"、"好" 等 7 列。该分割符基于语义，所以需要通过 `LANGUAGE` 参数传递语言编码，该编码使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
   - `cwords(LANGUAGE)`：将每个单词当作一列进行分割，且忽略标点符号等特殊字符。如 "hello, world!" 会被分割为 "hello"、"world" 两列。`LANGUAGE` 是用来表示语言的字符串，使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
   - `csentences(LANGUAGE)`：将每个句子当作一列进行分割，且忽略作为句子间的特殊字符（如空格），但保留标点符号。如 "hello, world! I love world!" 会被分割为 "hello, world!" 和 "I love world!" 两列。`LANGUAGE` 是用来表示语言的字符串，使用 ISO 639-2 语言编码，如 `en`，`zh` 等，默认为 `en`。
1. 列截取过滤器：
   - `chead(N)`：截取输出的前面 `N` 列。
   - `ctail(N)`：截取输出的后面 `N` 列。
   - `cignore(I,N)`：忽略从 `I` 开始的 `N` 列。`I` 表示列索引值，第一列的索引值为 0，最后一列用 `$` 表示；`N` 表示要忽略的列数，正值表示从 `I` 向后计算，负值表示从 `I` 向前计算。如 `cignore($,-10)` 表示忽略最后 10 列输出。
   - `cpick(A, B)`：选择索引值匹配 `An + B` 公式的列，其中 `n` 是零或自然数。比如 `cpick(2, 0)` 将选择第 0、2、4 等列；`cpick(3, -1)` 将选择第 2、5、8 等列。
   - `ccut(A, B)`：选择索引值不匹配 `An + B` 公式的列，其中 `n` 是零或自然数。比如 `ccut(2, 0)` 将选择第 1、3、5 等列；`ccut(3, -1)` 将选择第 0、1、3、4、6、7 等列。
1. 格式化过滤器：
   - `keys(KEY1, KEY2, ...)`：为已分割的列赋予键名，`KEY1`、`KEY2` 等表示各列的键名，若未定义，则使用 `c0`、`c1` 等。该格式化过滤器可被称为“字典过滤器”。
   - `array(N, C)`：将各列作为字符串形成数组进行格式化，N 表示最大的数组元素个数，超过 N 的其它列，将被合并到最后一个元素并使用 `C` 作为连接字符（默认为空格）。比如有如下三列 `file0`、`file1`、`file2`，执行 `array(3, ' ')` 后将被格式化为数组 `[ "file0", "file1", "file2" ]`；执行 `array(2, ':')` 后将被格式化为数组 `[ "file0", "file1:file2" ]`。若 N 未定义，则按照将每个列处理为一个数组元素。该格式化过滤器可被称为“数组过滤器”。
   - 若不指定格式化过滤器，则默认使用 `keys()` 字典过滤器。

比如对上述命令行输出，使用 `delimiter(' ')` 过滤器并指定分割符为空格时，JSON 化的处理结果为：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "lines": [
        { "c0": "total", "c1": "152" },
        { "c0": "drwxr-xr-x", "c1": "2",   "c2": "root", "c3": "root", "c4": "4096",  "c5": "Dec", "c6": "2", "c7": "15:29", "c8": "bin" },
        { "c0": "drwxr-xr-x", "c1": "143", "c2": "root", "c3": "root", "c4": "12288", "c5": "Dec", "c6": "9", "c7": "09:33", "c8": "etc" }
    ],
    "exitCode": 0
}
```

如果对上述命令行输出，先使用某个行截取过滤器，然后使用列分割过滤器，再使用某个列截取过滤器，最后使用字典格式化过滤器，如 `ingore(0, 1); delimiter(' '); ctail(8, 1); keys('name')` ，则结果为：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "lines": [
        { "name": "bin" },
        { "name": "etc" }
    ],
    "exitCode": 0
}
```

如果对上述命令行输出，先使用某个行截取过滤器，然后使用列分割过滤器，再使用某个列截取过滤器，最后使用数组格式化过滤器，如 `ingore(0, 1); delimiter(' '); ctail(5, 3); array(1, ' ')` ，则结果为：

若使用 `array()` 格式化过滤器，则结果为：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "lines": [
        [ "Dec 2 15:29" ],
        [ "Dec 9 09:33" ]
    ],
    "exitCode": 0
}
```

注意，过滤器的使用应该符合如下规则：

1. 始终在最后使用格式化过滤器；如果未指定格式化过滤器，则默认使用 `keys('c0', 'c1', ...)` 字典过滤器。
1. 可以多次应用行、列分割过滤器以及截取过滤器。

另外，如上所示，除了使用过滤器处理命令行输出之外，对 `lcmd` 图式的 URL 执行资源获取的结果作 JSON 化处理后有如下特点：

1. 使用 `statusCode` 返回命令的执行状态，类似 HTTP 状态码，如 200 表示成功执行，404 表示不存在的程序路径。
1. 使用 `errorMsg` 表示错误信息，取字符串或 NULL，NULL 时表示没有错误。
1. 使用 `exitCode` 表示命令行程序的退出码。一般而言，当一条命令正常执行后，退出码为 0；若未执行命令行程序而提前终止请求的处理，则 `exitCode` 设置为 NULL。
1. 使用 `lines` 数组返回每一行输出，数组中的每个单元为字典结构或者数组，且所有元素的值类型均为字符串。

为了方便在 HTML 文档中通过表单元素的名称和值构成不同的命令行，我们对作为参数传递的命令行做变量化处理：

1. 使用 `$` 作为前缀定义命令行中的变量，如 `$Id`，表示在生成最终的命令行时，该变量要使用同时传递的同名 URL 参数替代。
1. 使用其他 URL 参数定义该变量的值。

如：

```
lcmd:///bin/cat?cmdFilter=delimiter('%3A')&cmdLine=cat%20%24FILE&FILE=%2Fetc%2Fpasswd
```

上述 URL 包含三个参数：

- `cmdFilter`：定义过滤器，这里为 `delimiter(':')`。
- `cmdLine`：对应的值是一个变量化的命令行，解码后为：`cat $FILE`。
- `FILE`：对应的值是 `/etc/passwd`。该参数将用来替代上面变量化命令行字符串中的 `$FILE`。

故上述 URL 最终执行的命令行为：

```shell
cat /etc/passwd
```

我们通过变量化命令行，有利于通过表单中的输入项来设定命令后中的不同值，而变量化的命令行可以保持不变，从而降低编码量。而如果不使用变量化的命令行，我们将需要额外的脚本代码来生成最终的命令行。

变量化命令行中的变量名称需满足如下规范：

1. 以 `$` 字符开头，紧随其后的字符必须为字母，且，
1. 其后只包含数字、字母和下划线，且，
1. 任何非数字、字母和下划线字符表示变量名称的终结。
1. 保留 `$` 字符后为数字的情形，可用作以顺序指代参数。
1. 变量名称是大小写敏感的；也就是说 `$abc` 和 `$Abc` 表示不同的变量。

为了正确处理命令行中原本含有 `$` 字符的情形，需要对 `$` 字符作转义处理：

1. 对原命令行中按以上规则会被处理为变量名称的，使用 `$$` 做转义处理。
1. 若按以上规则不会处理为变量名称的，无须做转义处理。

如下面的原始命令行：

```shell
cat /$path/$
```

需要做如下转义处理：

```shell
cat /$$path/$
```

原始命令行语句中的最后一个 `$` 字符，由于不构成变量化命令行的变量名称，故而无须转义。

在一个典型的 Linux 系统上，上述 `lcmd` URL（`lcmd:///bin/cat?cmdFilter=delimiter('%3A')&cmdLine=cat%20%24FILE&FILE=%2Fetc%2Fpasswd`）给出的结果如下：

```json
{
    "statusCode": 200,
    "errorMsg": null,
    "lines": [
        { "c0": "root",   "c1": "x", "c2": "0", "c3": "0", "c4": "root",   "c5": "/root",     "c6": "/bin/bash" },
        { "c0": "daemon", "c1": "x", "c2": "1", "c3": "1", "c4": "daemon", "c5": "/usr/sbin", "c6": "/usr/sbin/nologin" },
        { "c0": "bin",    "c1": "x", "c2": "2", "c3": "2", "c4": "bin",    "c5": "/bin",      "c6": "/usr/sbin/nologin" },
        { "c0": "sys",    "c1": "x", "c2": "3", "c3": "3", "c4": "sys",    "c5": "/dev",      "c6": "/usr/sbin/nologin" }
    ],
    "exitCode": 0
}
```

注：限于篇幅，上述结果仅给出了前四行。

需要注意的是，由于在 `lcmd` URL 请求中，我们已经使用了 `cmdFilter` 和 `cmdLine` 两个请求参数名称，因此，其他请求参数要避免使用这两个参数名称。

当使用 `lcmd` 图式的 URL 作为表单提交的 `action` 属性时，由于 `lcmd` 图式返回的内容是 JSON 格式，并不能直接提供给用户代理展示。因此，我们需要同时定义一个正常的结果页 URL。

如下所示：

```html
    <form action="lcmd:///bin/rm" method="get" destination="/files.html">
        <input type="hidden" name="cmdFilter" value="head(0)" />
        <input type="hidden" name="cmdLine" value="rm $FILE" />
        <input type="hidden" name="FILE" value="test.txt" />
        <input type="submit" value="Delete" />
    </form>
```

我们在上面的 `form` 元素中使用 `destination` 属性定义了目标 URL。当表单提交成功（即 `lcmd:///bin/rm?cmdFilter=head(0)&cmdLine=rm%20%24FILE&FILE=test.txt` 的状态码为 2xx 时），用户代理应该导航到 `destination` 属性定义的目标 URL（`/files.html`）上。

---

更多合璧操作系统的技术突破，请关注后续文章。

