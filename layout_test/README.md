## Layout Test

排版测试是通过测试用例完成对排版引擎的的验证



### How to Use

1. 安装Ruby，cmake

2. 将layout_test/Tools/加入path中

   ```shell
   $ cd lynx-native/layout_test/
   $ export PATH="$PATH:`pwd`/Tools"
   ```

   ​

3. 在mac平台编译执行

   ```shell
   $ mkdir build
   $ cd build
   $ cmake .. -G "Xcode"
   $ xcodebuild
   $ ./bin/Debug/layout_test
   ```

   ​

