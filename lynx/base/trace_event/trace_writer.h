#ifndef LYNX_BASE_TRACE_EVENT_TRACE_WRITER_H_
#define LYNX_BASE_TRACE_EVENT_TRACE_WRITER_H_

#include <string>
#include <stdio.h>
#include "base/ref_counted_ptr.h"

namespace base {
class TraceEvent;
class TraceWriter : public RefCountPtr<TraceWriter> {
 public:
  TraceWriter() : last_trace_(), fd_(-1) {}
  ~TraceWriter(){}

  void Start();
  void Stop();
  void Write(TraceEvent* event);

 private:
  std::string Format(TraceEvent* event);

  std::string last_trace_;
  int fd_;

};

}  // namespace base

#endif  // LYNX_BASE_TRACE_EVENT_TRACE_WRITER_H_