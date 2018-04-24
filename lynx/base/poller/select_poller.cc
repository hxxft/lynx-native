#include "base/poller/select_poller.h"
#include <vector>

#include <errno.h>

namespace base {
SelectPoller::SelectPoller() {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);
}

SelectPoller::~SelectPoller() {}

void SelectPoller::WatchFileDescriptor(FileDescriptor* descriptor) {
  int fd = descriptor->fd();
  int event = descriptor->event();
  auto iter = file_descriptors_.find(fd);
  if (iter != file_descriptors_.end()) {
    return;
  }
  //        if(event & FD_EVENT_IN) FD_SET(fd, &read_fds_);
  //        else FD_CLR(fd, &read_fds_);
  //        if(event & FD_EVENT_OUT) FD_SET(fd, &write_fds_);
  //        else FD_CLR(fd, &write_fds_);
  file_descriptors_.add(fd, descriptor);
  fd_set_.insert(fd);
}

void SelectPoller::RemoveFileDescriptor(int fd) {
  auto iter = file_descriptors_.find(fd);
  if (iter == file_descriptors_.end()) {
    return;
  }

  //        int event = iter->second->event();
  //
  //        if(event & FD_EVENT_IN) FD_CLR(fd, &read_fds_);
  //        if(event & FD_EVENT_OUT) FD_CLR(fd, &write_fds_);

  file_descriptors_.erase(iter);
  fd_set_.erase(fd);
}

void SelectPoller::Poll(int timeout) {
  struct timeval tv = {0, 0};
  struct timeval* ptv = &tv;

  timeout = timeout < 0 ? INT_MAX : timeout;

  tv.tv_sec = timeout / 1000;
  tv.tv_usec = (timeout % 1000) * 1000;

  ResetFileDescriptor();
  int active_fd = 0;
  if ((active_fd = select((*fd_set_.begin()) + 1, &read_fds_, &write_fds_,
                          nullptr, ptv)) > 0) {
    for (auto iter = fd_set_.begin(); active_fd > 0 && iter != fd_set_.end();
         ++iter) {
      int fd = *iter;
      if (FD_ISSET(fd, &read_fds_) || FD_ISSET(fd, &write_fds_)) {
        active_fds_.push_back(fd);
      }
    }
  }
  ActiveFileDescriptor();
}

void SelectPoller::ResetFileDescriptor() {
  FD_ZERO(&read_fds_);
  FD_ZERO(&write_fds_);

  for (auto iter = fd_set_.begin(); iter != fd_set_.end(); ++iter) {
    FileDescriptor* descriptor = file_descriptors_.find(*iter)->second;
    int event = descriptor->event();
    int fd = descriptor->fd();
    if (event & FD_EVENT_IN)
      FD_SET(fd, &read_fds_);
    else
      FD_CLR(fd, &read_fds_);
    if (event & FD_EVENT_OUT)
      FD_SET(fd, &write_fds_);
    else
      FD_CLR(fd, &write_fds_);
  }
}

void SelectPoller::ActiveFileDescriptor() {
  for (auto iter = active_fds_.begin(); iter != active_fds_.end(); ++iter) {
    FileDescriptor* descriptor = file_descriptors_.find(*iter)->second;
    int event = descriptor->event();
    int fd = descriptor->fd();
    if (event & FD_EVENT_IN) {
      descriptor->OnFileCanRead(fd);
    } else if (event & FD_EVENT_OUT) {
      descriptor->OnFileCanWrite(fd);
    }
  }
  active_fds_.clear();
}
}  // namespace base
