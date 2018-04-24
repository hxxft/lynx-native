// Copyright 2017 The Lynx Authors. All rights reserved.

#include <iostream>

#if OS_ANDROID
#include "base/android/android_jni.h"
#endif

#include "base/threading/thread.h"

namespace base {

std::string CreateJSThreadName() {
  char name[32];
  static int index = 0;
  memset(name, 0, 32);
  sprintf(name, "JSThread-%d", index++);
  return name;
}

void* ThreadFunc(void* params) {
  Thread* thread = static_cast<Thread*>(params);
#if OS_IOS
  pthread_setname_np(thread->thread_name().c_str());
#elif OS_ANDROID
  pthread_setname_np(thread->thread_handle(), thread->thread_name().c_str());
#endif
  thread->Run();
#if OS_ANDROID
  android::DetachFromVM();
#endif
  return NULL;
}

Thread::Thread(MessageLoop::MESSAGE_LOOP_TYPE type, const std::string& name)
    : message_loop_(type), thread_name_(name) {
  if (type == MessageLoop::MESSAGE_LOOP_UI) {
    message_loop_.BindToCurrentThread();
  }
}

Thread::Thread(MessageLoop::MESSAGE_LOOP_TYPE type)
    : message_loop_(type), thread_name_() {
  if (type == MessageLoop::MESSAGE_LOOP_UI) {
    message_loop_.BindToCurrentThread();
  }
}

Thread::~Thread() {}

void Thread::Start() {
  bool err = pthread_create(&thread_handle_, NULL, ThreadFunc, this);

  if (err) {
    std::cout << "thread start failed!!!" << std::endl;
  }
}

void Thread::Join(const Thread& thread) {
  pthread_join(thread.thread_handle_, NULL);
}

void Thread::Run() {
  message_loop_.BindToCurrentThread();
  message_loop_.Run();
}

void Thread::Quit(base::Clouse* closue) {
  message_loop_.Quit(closue);
}

}  // namespace base
