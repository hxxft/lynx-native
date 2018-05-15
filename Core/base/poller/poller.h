#ifndef LYNX_BASE_POLLER_H_
#define LYNX_BASE_POLLER_H_

#include "base/scoped_ptr.h"
#include "base/scoped_ptr_map.h"
#include "base/task/callback.h"

#include "stdint.h"

namespace base {
#define FD_EVENT_NONE 0x0000
#define FD_EVENT_IN 0x0001
#define FD_EVENT_OUT 0x0002
#define FD_EVENT_ERR 0x0004

class FileDescriptor;

class Poller {
 public:
  class Watcher {
   public:
    virtual void OnFileCanRead(int fd) = 0;
    virtual void OnFileCanWrite(int fd) = 0;
  };
  Poller() {}

  virtual ~Poller() {}

  virtual void WatchFileDescriptor(FileDescriptor* descriptor) = 0;

  virtual void RemoveFileDescriptor(int fd) = 0;

  virtual void Poll(int64_t timeout) = 0;

 protected:
  ScopedPtrMap<int, FileDescriptor> file_descriptors_;
};

class FileDescriptor {
 public:
  FileDescriptor(Poller::Watcher* watcher, int fd, int event)
      : watcher_(watcher), fd_(fd), event_(event) {}

  int fd() { return fd_; }

  int event() { return event_; }

  void OnFileCanRead(int fd) {
    if (watcher_) {
      watcher_->OnFileCanRead(fd);
    }
  }

  void OnFileCanWrite(int fd) {
    if (watcher_) {
      watcher_->OnFileCanWrite(fd);
    }
  }

 private:
  Poller::Watcher* watcher_;

  int fd_;
  int event_;
};
}

#endif  // LYNX_BASE_POLLER_H_
