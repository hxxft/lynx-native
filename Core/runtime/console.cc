// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/console.h"
#include <runtime/base/lynx_value.h>
#include <runtime/jsc/jsc_helper.h>
#include "base/log/logging.h"
#include "runtime/base/lynx_array.h"

#include "debugger/debug_client.h"

namespace jscore {
Console::Console() {
  set_class_name("Console");
  RegisterMethodCallback("log", &LogCallback);
  RegisterMethodCallback("warn", &LogCallback);
  RegisterMethodCallback("error", &LogCallback);
}

Console::~Console() {}

base::ScopedPtr<LynxValue> Console::LogCallback(
    LynxObjectTemplate* object,
    base::ScopedPtr<LynxArray>& array) {
  std::string log;
  if (array.Get() != NULL && array->Size() > 0) {
    if (array->Size() == 1) {
      log = JSCHelper::ConvertToString(array->Get(0));
    } else {
      log = JSCHelper::ConvertToString(array.Get());
    }
    DLOG(LOG) << log;
  }

  debug::DebugClient::Debugger()->Send("log", log);
  return base::ScopedPtr<LynxValue>(NULL);
}

}  // namespace jscore
