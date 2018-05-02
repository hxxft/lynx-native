#include "base/trace_event/trace_logger.h"

#include "base/trace_event/trace_event.h"
#include "base/trace_event/trace_writer.h"

#include "base/threading/thread.h"

namespace base {

static TraceLogger* kTraceLogger = nullptr;

TraceLogger::TraceLogger() : logger_thread_(), writer_(), lock_() {}
TraceLogger::~TraceLogger(){}

TraceLogger* TraceLogger::Instance() {
  if(!kTraceLogger) {
    kTraceLogger = new TraceLogger;
    kTraceLogger->Initialize();
  }
  return kTraceLogger;
}

void TraceLogger::Start() {
  AutoLock lock(lock_);
  if(writer_.Get() || !logger_thread_.Get())
    return;
  
  writer_ = lynx_new TraceWriter;
  logger_thread_->Looper()->PostTask(
      Bind(&TraceWriter::Start, writer_));
}

void TraceLogger::Stop() {
  AutoLock lock(lock_);
  if(!writer_.Get() || !logger_thread_.Get())
    return;
  logger_thread_->Looper()->PostTask(Bind(&TraceWriter::Stop, writer_));
  writer_ = nullptr;
}

void TraceLogger::Initialize() {
  if(!logger_thread_.Get()) {
    logger_thread_.Reset(lynx_new Thread(base::MessageLoop::MESSAGE_LOOP_POSIX));
    logger_thread_->Start();
  }
}

void TraceLogger::AddTraceEvent(TraceEvent* event) {
  AutoLock lock(lock_);
  base::ScopedPtr<TraceEvent> trace_event(event);
  if (logger_thread_.Get() && writer_.Get()) {
    trace_event.Release();
    logger_thread_->Looper()->PostTask(
        Bind(&TraceWriter::Write, writer_, event));
  }
}

}  // namespace base