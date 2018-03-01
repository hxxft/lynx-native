// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_CONFIG_H_
#define LYNX_RUNTIME_CONFIG_H_

#include <string>

#define VERSION "1.0.0"
#define APP_NAME "lynx"
#if OS_ANDROID
#define PLATFORM "Android"
#elif OS_IOS
#define PLATFORM "iOS"
#endif

#endif  // LYNX_RUNTIME_CONFIG_H_
