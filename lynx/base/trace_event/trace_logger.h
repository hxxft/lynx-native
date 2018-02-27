#ifndef LYNX_BASE_TRACE_EVENT_TRACE_LOGGER_H_
#define LYNX_BASE_TRACE_EVENT_TRACE_LOGGER_H_

#include "base/ref_counted_ptr.h"
#include "base/scoped_ptr.h"
#include "base/threading/thread.h"

#include <string>

namespace base {
class TraceEvent;
class TraceWriter;
class TraceLogger {
 public:
  static TraceLogger* Instance();
  TraceLogger();
  ~TraceLogger();
  void Start();
  void Stop();
  void Initialize();
  void AddTraceEvent(TraceEvent* event);

 private:
  ScopedPtr<Thread> logger_thread_;
  ScopedRefPtr<TraceWriter> writer_;
  Lock lock_;
};
}  // namespace base

#endif  // LYNX_BASE_TRACE_EVENT_TRACE_LOGGER_H_