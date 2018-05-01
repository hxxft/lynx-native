// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_DEBUGGER_DEBUG_HOST_H_
#define LYNX_DEBUGGER_DEBUG_HOST_H_

#include <map>
#include <string>

namespace debug {
enum DebugType {
  Debug_Reload = 0,
};

class DebugHost {
 public:
  virtual void Run(DebugType type) = 0;
};
}  // namespace debug

#endif  // LYNX_DEBUGGER_DEBUG_HOST_H_
