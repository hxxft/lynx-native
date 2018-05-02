// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_PRINT_H_
#define LYNX_BASE_PRINT_H_

#if OS_ANDROID
#include "android/log.h"
#endif


#define NATIVE_PRINT 1

static bool TRACE_ENABLED_ = false;

#if NATIVE_PRINT
#ifndef LOGD
#if OS_ANDROID
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,  __VA_ARGS__)
#elif OS_IOS
#define LOGD(tag, ...) printf(tag); printf(": "); printf(__VA_ARGS__); putchar('\n');
#endif
#endif

#ifndef LOGI
#if OS_ANDROID
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __VA_ARGS__)
#elif OS_IOS
#define LOGI(tag, ...) printf(tag); printf(": "); printf(__VA_ARGS__); putchar('\n');
#endif
#endif

#ifndef LOGE
#if OS_ANDROID
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__)
#elif OS_IOS
#define LOGE(tag, ...) printf(tag); printf(": "); printf(__VA_ARGS__); putchar('\n');
#endif
#endif

#ifndef LOGS
#if OS_ANDROID
#define LOGS(tag, log) if(TRACE_ENABLED_) __android_log_print(ANDROID_LOG_DEBUG, tag, "%s", log);
#elif OS_IOS
#define LOGS(tag, log) if(TRACE_ENABLED_) printf(tag); printf(": "); printf("%s", log); putchar('\n');
#endif
#endif
#else
#define LOGD(...)
#define LOGI(...)
#define LOGE(...)
#endif

#endif  // LYNX_BASE_PRINT_H_
