#ifndef LYNX_BASE_SELECT_POLLER_H_
#define LYNX_BASE_SELECT_POLLER_H_

#include <set>
#include <vector>
#include <sys/select.h>

#include "base/poller/poller.h"

namespace base {
    class SelectPoller : public Poller {
    public:
        SelectPoller();
        
        virtual ~SelectPoller();
        
        virtual void WatchFileDescriptor(FileDescriptor* descriptor);
        
        virtual void RemoveFileDescriptor(int fd);
        
        virtual void Poll(int timeout);
    private:

        void ResetFileDescriptor();
        
        void ActiveFileDescriptor();
        
        fd_set read_fds_;
        fd_set write_fds_;

        std::set<int, std::greater<int> >  fd_set_;
        std::vector<int> active_fds_;
    };
}

#endif  // LYNX_BASE_SELECT_POLLER_H_
