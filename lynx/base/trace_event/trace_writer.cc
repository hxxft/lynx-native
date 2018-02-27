#include "base/trace_event/trace_writer.h"

#include "base/trace_event/trace_event.h"

#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>

#include "base/print.h"

namespace base {
void TraceWriter::Start() {
  last_trace_ = "{\"displayTimeUnit\": \"ns\", \"traceEvents\" :[";
  //last_trace_ = "{\"traceEvents\" :[";
  std::ostringstream file_path;
  file_path << "/sdcard/"<<"trace_"<<CurrentTimeMillis()<<".log";

  fd_ = open(file_path.str().c_str(), O_CREAT | O_RDWR);

  LOGD("lynx-debug", "file open: %s, %d", file_path.str().c_str(), fd_);
}

void TraceWriter::Stop() {
  if(last_trace_.back() != '[') {
    last_trace_[last_trace_.length() - 1] = ']';
  }else{
    last_trace_ = "[]";
  }
  if(fd_ != -1) {
    write(fd_, last_trace_.c_str(), last_trace_.length());
    write(fd_, "}", 1);
  }
  LOGD("lynx-debug", "stop file writer");
}
void TraceWriter::Write(TraceEvent* event) {
  if(!last_trace_.empty() && fd_ != -1) {
    write(fd_, last_trace_.c_str(), last_trace_.length());
  }
  ScopedPtr<TraceEvent> trace_event(event);
  last_trace_ = Format(event);
}

std::string TraceWriter::Format(TraceEvent* event) {
  std::ostringstream formater;
  formater << "{";
  formater << "\"name\""
           << ":" << "\"" << event->name_ << "\"";
  formater << ",";

  formater << "\"cat\""
           << ":" << "\"" << event->category_ << "\"";
  formater << ",";

  formater << "\"ph\""
           << ":"
           << "\"B\"";
  formater << ",";

  formater << "\"pid\""
           << ":" << event->pid_;
  formater << ",";

  formater << "\"tid\""
           << ":" <<  event->tid_;
  formater << ",";

  //  formater << "\"dur\""
  //          << ":" << "\"" << event->end_timestamp_ - event->begin_timestamp_<< "\"";
  // formater << ",";

  formater << "\"ts\""
           << ":"  << event->begin_timestamp_ ;

  formater << "}";

  formater << ",";

  formater << "{";
  formater << "\"name\""
           << ":" << "\"" << event->name_ << "\"";
  formater << ",";

  formater << "\"cat\""
           << ":" << "\"" << event->category_ << "\"";
  formater << ",";

  formater << "\"ph\""
           << ":"
           << "\"E\"";
  formater << ",";

  formater << "\"pid\""
           << ":"  << event->pid_;
  formater << ",";

  formater << "\"tid\""
           << ":" << event->tid_;
  formater << ",";

  formater << "\"ts\""
           << ":" << event->end_timestamp_;

  formater << "}";
  formater << ",";

  return formater.str();
}

}  // namespace base