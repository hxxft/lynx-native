#ifndef LYNX_BASE_TRACE_EVENT_TRACE_EVENT_H_
#define LYNX_BASE_TRACE_EVENT_TRACE_EVENT_H_

#include <cstdint>
#include <string>

#include "base/threading/thread.h"
#include "base/timer/time_utils.h"

namespace base {
struct TraceEvent {
  TraceEvent(const char* category, const char* name):
    name_(name),
    category_(category),
    begin_timestamp_(0),
    end_timestamp_(0),
    pid_(0),
    tid_(static_cast<uint32_t>(base::Thread::CurrentId())) {

    }
  const char* name_;
  const char* category_;
  uint64_t begin_timestamp_;
  uint64_t end_timestamp_;
  uint32_t pid_;
  uint32_t tid_;
};
}

#endif  // LYNX_BASE_TRACE_EVENT_TRACE_EVENT_H_