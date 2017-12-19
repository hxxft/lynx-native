// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_REF_COUNTED_PTR_H_
#define LYNX_BASE_REF_COUNTED_PTR_H_

#include <stddef.h>

#include "base/macros.h"
#include "base/atomic_ref_count.h"

#include "base/debug/memory_debug.h"

namespace base {
template<class T>
class RefCountPtr {
 public:
    RefCountPtr() : ref_count_(0) {}
    virtual ~RefCountPtr() {}

    void AddRef() {
        AtomicRefCountInc(&ref_count_);
    }

    void Release() {
        if (!AtomicRefCountDec(&ref_count_)) {
            lynx_delete(static_cast<T*>(this));
        }
    }

 private:
    mutable AtomicRefCount ref_count_;
    DISALLOW_COPY_AND_ASSIGN(RefCountPtr);
};

template<class T>
class ScopedRefPtr {
 public:
    using type_name = T;

    ScopedRefPtr() : ptr_(NULL) {}
    explicit ScopedRefPtr(type_name* ptr) : ptr_(ptr) {
        if (ptr_) {
            ptr_->AddRef();
        }
    }
    ScopedRefPtr(ScopedRefPtr<type_name>& other) {
        ptr_ = NULL;
        *this = other.ptr_;
    }
    
    ~ScopedRefPtr() {
        if (ptr_) {
            ptr_->Release();
        }
    }
    
    ScopedRefPtr<type_name>& operator=(const
                                       ScopedRefPtr<type_name>& other) {
        return *this = other.ptr_;
    }

    ScopedRefPtr<type_name>& operator=(
                ScopedRefPtr<type_name>& other) {
        if (other.ptr_) {
            other.ptr_->AddRef();
        }

        type_name* old_ptr = ptr_;

        ptr_ = other.ptr_;

        if (old_ptr) {
            old_ptr->Release();
        }

        return *this;
    }

    ScopedRefPtr<type_name>& operator=(type_name* ptr) {
        if (ptr) {
            ptr->AddRef();
        }

        type_name* old_ptr = ptr_;

        ptr_ = ptr;

        if (old_ptr) {
            old_ptr->Release();
        }

        return *this;
    }
    
    void Swap(ScopedRefPtr<type_name>& other) {
        type_name* ptr = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = ptr;
    }


    T* Get() const { return ptr_; }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

 private:
    type_name* ptr_;
};
}  // namespace base

#endif  // LYNX_BASE_REF_COUNTED_PTR_H_
