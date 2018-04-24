#ifndef LYNX_BASE_LAZY_INSTANCE_H_
#define LYNX_BASE_LAZY_INSTANCE_H_

#include "base/threading/lock.h"

namespace base {

template <typename Type>
class LazyInstance {
 public:
  LazyInstance() : instance_(nullptr), lock_() {}

  ~LazyInstance() {
    if (instance_ == nullptr)
      delete instance_;
    instance_ = nullptr;
  }

  Type* Get() {
    AutoLock lock(lock_);
    if (instance_ == nullptr) {
      instance_ = new Type();
    }
    return instance_;
  }

 private:
  Type* instance_;
  Lock lock_;
};
}  // namespace base

#endif  // LYNX_BASE_LAZY_INSTANCE_H_
