## Lynx##

Lynx 是一个使用Javascript来构建Android和iOS本地应用的框架。Lynx直接使用平台UI来进行渲染，同时可以使用CSS进行排版。

Lynx可以直接使用同一份Javascript生成Android和iOS本地应用，同时提供了使用本地APIs的能力，开发者无需通过Android Studio和XCode来维护本地应用的代码。[`lynx-cli`](https://github.com/hxxft/lynx-cli)提供了基本工程模版以及编译打包工具。

Lynx跨平台框架则相对RN有更近一步的Native体验，将使用Web开发框架的效率与Native体验做了更深层次的结合，拥有更接近Native的体验。



### 特性

Lynx选择了Vue.js作为开发框架，当然的利于Lynx的API设计，使用其他前端框架也是非常方便的。相对于React Native，Lynx拥有和Native一致的首屏体验和交互动画，这点与Flutter有着相同的流畅体验。而相对于Flutter，Lynx更多态化，不仅仅在能调试的时候进行hot reload来方便UI调试，同时还能进行运行时的热更新。

- 首屏直接展示

  React Native在进入页面的时候，或多或少会有一段时间的白屏，这点相对于Native流畅的首屏体验来说是一定的劣势。Lynx则可以做到和Native一致的流畅体验，在页面动画过程中，页面已经完成渲染展示，因此不会出现白屏或者闪屏。

  <img src="https://github.com/hxxft/lynx-native/raw/master/images/about_page2.gif" width="240"/> <img src="https://github.com/hxxft/lynx-native/raw/master/images/about_page1.gif" width="240"/>  

- 交互动画

  Lynx提供了更丰富的交互动画能力，能满足绝大部分场景的交互动画需求。相比于React Native以及Web在交互动画能力上的缺陷，Lynx的交互动画可以媲美于Flutter，可达到60fps的动画。

  <img src="https://github.com/hxxft/lynx-native/raw/master/images/about_animation1.gif" width="240"/> <img src="https://github.com/hxxft/lynx-native/raw/master/images/about_animation2.gif" width="240"/> <img src="https://github.com/hxxft/lynx-native/raw/master/images/about_animation3.gif" width="240"/>

- 更快的JSBinding

  Lynx在对JSBinding上做了近一步处理，避免了React Native中不必要的JSBridge调用耗时，因此Lynx有着非常高效的JSBridge。

- 动态化发布调试

  Lynx提供了hot reload调试能力，使用lynx-cli工具，可以自动编译并且重新加载项目。这样能非常高效的调试UI，对UI进行实时的调整。

  ​

### How To Build

  Lynx是一个跨平台开发框架，底层基于C++编写，能方便的在Android/iOS平台上编译运行。

  + Android平台编译  
    + 执行`{$dir}/lynx-native/lynx/build/prebuild.sh`  
    + 使用Android Studio打开`{$dir}/lynx-native/Android`  
    + 编译运行example工程

  + iOS平台编译  

    + 在`{$dir}/lynx-native/iOS/`执行`pod install`, 没有安装pod的请先安装pd  

    + 使用xcode打开`{$dir}/lynx-native/iOS/lynx.xcworkspace`

      ​

### Lynx工作流程

![image](https://github.com/hxxft/lynx-native/raw/master/images/lynx-native-design.png)

从一个页面打开到展示流程和输入事件到页面响应流程来介绍Lynx的工作流程，当前Lynx使用Vue.js前端框架：

1. 先从Page节点出发。当页面开发完成后，通过lynx-cli进行项目构建，编译过程生成排版文件(LayoutFile)和执行文件。

2. LayoutFile通过解析器Parser生成RenderTree，同时生成Element（JS可控制的元素，处于Binding模块）。

3. RenderTree通过排版引擎LayoutEngine计算布局信息，与对应的Platform层的UI进行位置信息同步，最终由各平台渲染View并绘制上屏。

4. 在首屏排版流程进行完，会执行JS文件，正式进入到应用页面的生命周期。

5. 当屏幕接收到响应事件，由事件处理模块进行手势识别，通过PlatformUI 将事件传同步到Render层，通过Binding同步到JS。

6. 当涉及到实时动画操作，由PlatformUI将事件同步传递给动画处理引擎AnimateVirtualMachine。引擎将根据AnimateScript（交互动画执行脚本，通过JS设置给交互动画引擎），执行平台View动画。

具体设计文档可以参看[design document](https://github.com/hxxft/lynx-doc/blob/master/lynx-native-design.md)




### 联系方式

Lynx项目目前处于alpha阶段，已经具有完备的雏形，喜欢尝鲜体验的同学可以先尝试编译项目中的demo进行体验。后续一个月内我们将会提供完善的API文档，以及更详细设计文档，也欢迎有兴趣的同学加入的我们的项目中，给我们提Issue和Pull Request。 

有兴趣的同学也可以加入Lynx交流群(674140735)，一起参与讨论。

