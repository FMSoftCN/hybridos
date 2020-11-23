# 合璧操作系统设备端hiAceJs设计


本文阐述合璧操作系统设备端hiAceJs设计。

- [基本框架及术语](#基本框架及术语)
- [架构及关键模块](#架构及关键模块)
   + [架构](#架构)
   + [JS应用框架层](#JS应用框架层)
   + [JS引擎与运行时层](#JS引擎与运行时层)
   + [基础组件](#基础组件)


## 基本框架及术语

下图给出了 HybridOS 设备侧的系统架构图：

```
 ---------------------------------------------------------------------------------
| DockerBar, StatusBar,   |                   |           | Input Method Window,  |
| Screen Lock, Launcher,  | App Main Windows  |  N/A      | System Alert Window,  |        - main windows
| Notification...         |                   |           | ...                   |
 ---------------------------------------------------------------------------------
|      System Manager     |     App Agent     | Wallpaper | mginit w/ compositor  |____    - processes
 ---------------------------------------------------------------------------------     |
|       hiWebKit          |    hiAceJs        |                                   |    |
 ---------------------------------------------                                    |    |
|  MiniGUI, hiCairo, hiMesa, SQLite, FreeType, HarfBuzz, LibPNG, LibJPEG, ...     |    | hiBus
|                                                                                 |    |
 ---------------------------------------------------------------------------------     |
|                           HybridOS servers and user daemons                     |____|
 ---------------------------------------------------------------------------------
|                          Python runtime environment (optional)                  |
|                             LibZ, CURL, LibGcrypt, ...                          |
|                               C/C++ runtime environment                         |
 ---------------------------------------------------------------------------------
|                               Linux Kernel/Drivers                              |
 ---------------------------------------------------------------------------------
```

在 HybridOS 中，提供了两种应用开发框架。一种是基于hiWebKit，另一种是基于hiAceJs。

hiWebKit 基础代码来自于 WebKit，提供了完整理web应用支持。

hiAceJs 的一些思想和部分代码来自于 OpenHarmony 的 ace_lite_jsfwk，提供了一套跨平台的类web应用
开发框架，通过Toolkit将开发者编写的HML、CSS和JS 文件编译打包成JS Bundle，解析运行JS Bundle，
生成native UI View组件树并进行渲染显示。通过支持三方开发者使用声明式的API进行应用开发，以数
据驱动视图变化，避免大量的视图操作，大大降低应用开发难度，提升开发者开发体验。


## 架构及关键模块

hiAceJs 使用 C/C++ 语言开发，使用 jerryscript 作为JS runtime, 大体上由JS 应用框架层、 JS 引擎
与运行时层与运行时层和图形渲染层三部分组成。

### 架构

下图描述了 hiAceJs 的架构以及主要模块构成。

```
 ---------------------------------------------------------------------------------
|                                       App                                       |
 ---------------------------------------------------------------------------------
|                                  JS应用框架层                                   |
 ---------------------------------------------------------------------------------
|                                JS引擎与运行时层                                 |
 ---------------------------------------------------------------------------------
|                         Component          |                                    |
 ---------------------------------------------                                    |
|                          MiniGUI图形渲染层                                      |
 ---------------------------------------------------------------------------------
```


如上图所示，App 构建在 JS应用框架之上，由JS引擎与运行时层装载和运行，其使用的控制通过JS引擎
和运行时层绑定到图形渲染层对应的Component上，通过MiniGUI 来处理事件并最终渲染到屏幕上。

- JS 应用框架层：提供了一套跨平台的类web应用开发框架
- JS 引擎与运行时层：该部分主要使用 JerryScript 实现
- 图形渲染层：该部分主要包括使用C/C++实现的一套Component，以及相关动画支持。


### JS应用框架层 



### JS引擎与运行时层 

JS 引擎目前使用的是 JerryScript，这是一款由三星开发的嵌入式 JS 引擎。运行时主要提供了JS 应用框架层
使用的组件到平台的绑定，例如，每种形如 <text> 和 <div> 的 XML 标签组件，都对应一个绑定到 JerryScript 
上的 C++ Component 类，如 TextComponent 和 DivComponent 等。


### 图形渲染层

* 基础组件
  - AnalogClockComponent
  - CameraComponent
  - CanvasComponent
  - ChartComponent
  - ClockHandComponent
  - DivComponent
  - ImageAnimatorComponent
  - ImageComponent
  - InputComponent
  - ListComponent
  - PickerViewComponent
  - ProgressComponent
  - SliderComponent
  - SwiperComponent
  - SwitchComponent
  - TabBarComponent
  - TabContentComponent
  - TabsComponent
  - TextComponent
  - VideoComponent

* 动画支持

实现了简单的动画支持，主要由Animator, AnimatorCallback 和 AnimatorManager 三个类来实现。

当需要实现动画时:
-  首先，需要继承 AnimatorCallback 类，并实现其成员函数 Callback，在该函数里实现相关动作； 

```c

    virtual void Callback(UIView* view)

```

-  其次，需要实例化一个 Animator 对象，并将 AnimatorCallback 作为参数传入;

```c

    Animator(AnimatorCallback* callback, UIView* view, uint32_t time, bool repeat)

```

-  最后，将 Animator 加入到 AnimatorManager 中，由 AnimatorManager 类的 AnimatorTask 函数来调度。

```c

void AnimatorManager::AnimatorTask()
{
    ListNode<Animator*>* pos = list_.Begin();
    Animator* animator = nullptr;

    while (pos != list_.End()) {
        animator = pos->data_;
        if (animator->GetState() == Animator::START) {
            if (animator->IsRepeat() || animator->GetRunTime() <= animator->GetTime()) {
                animator->Run();
            } else {
                animator->Stop();
            }
        }

        pos = pos->next_;
    }
}

```


AnimatorManager 类的 AnimatorTask 函数由MiniGUI的定时器，每10ms调用一次。

* 事件处理 : 使用MiniGUI的消息(鼠标，键盘等)

* 渲染

提供 RenderManager 类来进行渲染，其核心函数是 RenderManager::RenderRect，它接收参数 Rect作为
目标区域进行渲染。最终在MiniGUI的 MSG_PAINT 消息处理时将渲染结果输出到屏幕上。
 

```c

void RenderManager::RenderRect(const Rect& rect, RootView* rootView)
{
    if (rootView == nullptr) {
        return;
    }

    Rect mask = rect;
#if ENABLE_WINDOW
    if (rootView->GetBoundWindow()) {
        Rect winRect = rootView->GetBoundWindow()->GetRect();
        winRect.SetPosition(0, 0);
        mask.Intersect(rect, winRect);
    }
#endif
    int32_t bufferHeight = ScreenDeviceProxy::GetInstance()->GetScreenArea() / mask.GetWidth();
    if (bufferHeight > mask.GetHeight()) {
        bufferHeight = mask.GetHeight();
    }

    Rect& bufferRect = ScreenDeviceProxy::GetInstance()->GetBufferRect();
    bufferRect.SetLeft(mask.GetLeft());
    bufferRect.SetRight(mask.GetRight());

    int16_t bottom = mask.GetBottom();
    for (int16_t bufferTop = mask.GetTop(); bufferTop <= bottom; bufferTop += bufferHeight) {
        bufferRect.SetTop(bufferTop);
        int16_t bufferBottom = bufferTop + bufferHeight - 1;
        if (bufferBottom >= bottom) {
            bufferRect.SetBottom(bottom);
        } else {
            bufferRect.SetBottom(bufferBottom);
        }

        UIView* topView = rootView->GetTopUIView(bufferRect);
        rootView->DrawTop(topView, bufferRect);
#if !ENABLE_WINDOW
        ScreenDeviceProxy::GetInstance()->Flush();
#endif
    }
}

```
