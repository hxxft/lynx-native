#ifndef BASE_LOG_LOGGING_H_
#define BASE_LOG_LOGGING_H_

#include <sstream>
#include <string>

#include "base/macros.h"



namespace logging {

enum LoggingDestination {
  LOG_NONE                = 0,
  LOG_TO_FILE             = 1 << 0,
  LOG_TO_SYSTEM_DEBUG_LOG = 1 << 1,

  LOG_TO_ALL = LOG_TO_FILE | LOG_TO_SYSTEM_DEBUG_LOG,

  LOG_DEFAULT = LOG_TO_SYSTEM_DEBUG_LOG,
};

void SetMinLogLevel(int level);

int GetMinLogLevel();

typedef int LogSeverity;
const LogSeverity LOG_VERBOSE = -1;  
const LogSeverity LOG_INFO = 0;
const LogSeverity LOG_WARNING = 1;
const LogSeverity LOG_ERROR = 2;
const LogSeverity LOG_FATAL = 3;
const LogSeverity LOG_LOG = 4;
const LogSeverity LOG_NUM_SEVERITIES = 5;

#ifdef NDEBUG
const LogSeverity LOG_DFATAL = LOG_ERROR;
#define ENABLE_DLOG 0
#define DCHECK_IS_ON() 0
#else
const LogSeverity LOG_DFATAL = LOG_FATAL;
#define ENABLE_DLOG 1
#define DCHECK_IS_ON() 1
#endif

// This class is used to explicitly ignore values in the conditional
// logging macros.  This avoids compiler warnings like "value computed
// is not used" and "statement has no effect".
class LogMessageVoidify {
 public:
  LogMessageVoidify() { }
  // This has to be an operator with a precedence lower than << but
  // higher than ?:
  void operator&(std::ostream&) { }
};

#define LOG_IS_ON(severity) \
  ((::logging::LOG_ ## severity) >= ::logging::GetMinLogLevel())

#define LOG_STREAM(severity)  \
  logging::LogMessage(__FILE__, __LINE__, logging::LOG_ ## severity).stream()

#define LAZY_STREAM(stream, condition)                                  \
  !(condition) ? (void) 0 : ::logging::LogMessageVoidify() & (stream)

#define DLOG(severity)                                          \
  LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))

#define DCHECK(condition)                                                \
  LAZY_STREAM(LOG_STREAM(FATAL), DCHECK_IS_ON() ? !(condition) : false) \
      << "Check failed: " #condition ". "

class LogMessage {
 public:
  LogMessage(const char* file, int line, LogSeverity severity);
  LogMessage(const char* file, int line, std::string* result);
  LogMessage(const char* file,
             int line,
             LogSeverity severity,
             std::string* result);
  ~LogMessage();
  std::ostream& stream() { return stream_; }

 private:
  void Init(const char* file, int line);

  LogSeverity severity_;
  std::ostringstream stream_;
  size_t message_start_;

  const char* file_;
  const int line_;
  DISALLOW_COPY_AND_ASSIGN(LogMessage);
};
}  // namespace logging

#endif