#ifndef LYNX_BASE_TRACE_EVENT_TRACE_EVENT_COMMON_H_
#define LYNX_BASE_TRACE_EVENT_TRACE_EVENT_COMMON_H_

#include "base/trace_event/trace_event.h"
#include "base/trace_event/trace_logger.h"

namespace base {
class ScopedTracer {
 public:
  ScopedTracer() {}
  ~ScopedTracer() { 
    event_->end_timestamp_ = CurrentTimeMicroseconds();
    TraceLogger::Instance()->AddTraceEvent(event_.Release());
  }

  void Initialize(const char* category, const char* name) {
    event_.Reset(new TraceEvent(category, name));
    event_->begin_timestamp_ = CurrentTimeMicroseconds();
  }

 private:
  base::ScopedPtr<TraceEvent> event_;
};
}  // namespace base

#define INTERNAL_TRACE_EVENT_UID2(a, b) trace_event_uid_##a##b
#define INTERNAL_TRACE_EVENT_UID(name) INTERNAL_TRACE_EVENT_UID2(name, __LINE__)

#define TRACE_EVENT0(category, name)                   \
  base::ScopedTracer INTERNAL_TRACE_EVENT_UID(tracer); \
  INTERNAL_TRACE_EVENT_UID(tracer).Initialize(category, name);

#endif