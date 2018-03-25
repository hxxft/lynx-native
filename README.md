## Lynx
Lynx 是一个使用Javascript来构建Android和iOS本地应用的框架。Lynx直接使用平台UI来进行渲染，同时可以使用CSS进行排版。

Lynx可以直接使用同一份Javascript生成Android和iOS本地应用，并且同时也提供了使用本地APIs的能力，开发者无需通过Android Studio和XCode来维护本地应用的代码。[`lynx-cli`](https://github.com/hxxft/lynx-cli)提供了基本工程模版以及编译打包工具
。

如果希望调试Lynx SDK，可以直接供Android Studio或者XCode编译运行Example工程，具体可以按照Build步骤进行编译。

## How To Build

Lynx是一个跨平台开发框架，底层基于C++编写，能方便的在Android/iOS平台上编译运行。

+ Android平台编译

  - 执行`{$dir}/lynx-native/lynx/build/prebuild.sh`
  - 使用Android Studio打开`{$dir}/lynx-native/Android`
  - 编译运行example工程

+ iOS平台编译

  - 在`{$dir}/lynx-native/iOS/`执行`pod install`,没有安装pod的请先安装pd

  + 使用xcode打开`{$dir}/lynx-native/iOS/lynx.xcworkspace`

## Design

![image](https://github.com/hxxft/lynx-native/raw/master/images/lynx-native-design.png)

具体可以参看[design document](https://github.com/hxxft/lynx-doc/blob/master/lynx-native-design.md)

## API

Lynx对排版和CSS属性进行了选择性支持，具体请参看[API文档(后续提供...)]()



