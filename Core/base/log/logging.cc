#include "base/log/logging.h"
#include "base/threading/thread.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <string>

#if defined(OS_ANDROID)
#include <android/log.h>
#endif

namespace logging {
namespace {

const char* const log_severity_names[LOG_NUM_SEVERITIES] = {"INFO", "WARNING",
                                                            "ERROR", "FATAL", "LOG"};

const char* log_severity_name(int severity) {
  if (severity >= 0 && severity < LOG_NUM_SEVERITIES)
    return log_severity_names[severity];
  return "UNKNOWN";
}

int g_min_log_level = 0;
LoggingDestination g_logging_destination = LOG_DEFAULT;
}  // namespace

void SetMinLogLevel(int level) {
  g_min_log_level = std::min(LOG_FATAL, level);
}

int GetMinLogLevel() {
  return g_min_log_level;
}

LogMessage::LogMessage(const char* file, int line, LogSeverity severity)
    : severity_(severity), file_(file), line_(line) {
  Init(file, line);
}

LogMessage::LogMessage(const char* file, int line, std::string* result)
    : severity_(LOG_FATAL), file_(file), line_(line) {
  Init(file, line);
  stream_ << "Check failed: " << *result;
  delete result;
}

LogMessage::LogMessage(const char* file,
                       int line,
                       LogSeverity severity,
                       std::string* result)
    : severity_(severity), file_(file), line_(line) {
  Init(file, line);
  stream_ << "Check failed: " << *result;
  delete result;
}

LogMessage::~LogMessage() {
  stream_ << std::endl;
  std::string str_newline(stream_.str());

  if ((g_logging_destination & LOG_TO_SYSTEM_DEBUG_LOG) != 0) {
#if defined(OS_ANDROID)
    android_LogPriority priority =
        (severity_ < 0) ? ANDROID_LOG_VERBOSE : ANDROID_LOG_UNKNOWN;
    switch (severity_) {
      case LOG_INFO:
      case LOG_LOG:
        priority = ANDROID_LOG_INFO;
        break;
      case LOG_WARNING:
        priority = ANDROID_LOG_WARN;
        break;
      case LOG_ERROR:
        priority = ANDROID_LOG_ERROR;
        break;
      case LOG_FATAL:
        priority = ANDROID_LOG_FATAL;
        break;
    }
    __android_log_write(priority, "lynx", str_newline.c_str());
#elif defined(OS_IOS)
      printf("lynx: %s\n", str_newline.c_str());
#endif
  }

  // write to log file
  if ((g_logging_destination & LOG_TO_FILE) != 0) {
  }
}

// writes the common header info to the stream
void LogMessage::Init(const char* file, int line) {

  std::string filename(file);
  size_t last_slash_pos = filename.find_last_of("\\/");
  if (last_slash_pos != std::string::npos) {
    size_t size = last_slash_pos + 1;
    filename = filename.substr(size, filename.length() - size);
  }

  stream_ << '[';
  stream_ << base::Thread::CurrentId() << ':';

  time_t t = time(NULL);
  struct tm local_time = {0};

  localtime_r(&t, &local_time);

  struct tm* tm_time = &local_time;
  stream_ << std::setfill('0') << std::setw(2) << 1 + tm_time->tm_mon
          << std::setw(2) << tm_time->tm_mday << '/' << std::setw(2)
          << tm_time->tm_hour << std::setw(2) << tm_time->tm_min << std::setw(2)
          << tm_time->tm_sec << ':';

  if (severity_ >= 0)
    stream_ << log_severity_name(severity_);
  else
    stream_ << "VERBOSE" << -severity_;
    
  if(severity_ != LOG_LOG) {
    stream_ << ":" << filename << "(" << line << ")] ";
  } else {
    stream_ << "] ";
  }
  
  message_start_ = stream_.str().length();
}

}  // namespace logging
