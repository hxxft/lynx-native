// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_WEAK_PTR_H_
#define LYNX_BASE_WEAK_PTR_H_

#include "base/debug/memory_debug.h"
#include "base/ref_counted_ptr.h"

namespace base {

class WeakReference {
 public:
  class Flag : public RefCountPtr<Flag> {
   public:
    Flag() : is_valid_(true) {}
    void Invalidate() { is_valid_ = false; }
    bool IsValid() { return is_valid_; }

   private:
    bool is_valid_;
  };

  WeakReference() {}
  WeakReference(const WeakReference& other) : invalidate_flag_(other.invalidate_flag_) {}
  explicit WeakReference(Flag* flag) : invalidate_flag_(flag) {}
  ~WeakReference() {
  }

  void Invalidate() { invalidate_flag_->Invalidate(); }

  bool IsValid() const {
    return invalidate_flag_.Get() != NULL && invalidate_flag_.Get()->IsValid();
  }

  WeakReference& operator=(const WeakReference& ref) {
    invalidate_flag_ = ref.invalidate_flag_;
  }

  WeakReference& operator=(WeakReference&& ref) {
    invalidate_flag_ = ref.invalidate_flag_;
  }

 private:
  ScopedRefPtr<Flag> invalidate_flag_;
};

// It's not thread-safety.
template <typename T>
class WeakPtr {
 public:
  WeakPtr() : ptr_(0), ref_() {}

  WeakPtr(T* ptr) : ptr_(ptr), ref_(lynx_new WeakReference::Flag) {}

  WeakPtr(const WeakPtr& other)
      : ptr_(other.ptr_), ref_(other.ref_) {}

  template <typename U>
  WeakPtr(WeakPtr<U>&& other) : ptr_(other.ptr_), ref_(other.ref_){
  }

  ~WeakPtr() {}

  T* Get() const { return ref_.IsValid() ? ptr_ : NULL; }

  void Invalidate() { ref_.Invalidate(); }

  bool IsValid() {
    return ref_.IsValid();
  }

  T* operator->() const { return Get(); }
  T& operator*() const { return *Get(); }

  WeakPtr& operator=(WeakPtr& other) {
    ptr_ = other.ptr_;
    ref_ = other.ref_;
    return *this;
  }

  WeakPtr& operator=(WeakPtr&& other) {
    ptr_ = other.ptr_;
    ref_ = other.ref_;
    return *this;
  }

  template <typename U>
  WeakPtr& operator=(WeakPtr<U>& other) {
    ptr_ = other.ptr_;
    ref_ = other.ref_;
    return *this;
  }

  template <typename U>
  WeakPtr& operator=(WeakPtr<U>&& other) {
    ptr_ = other.ptr_;
    ref_ = other.ref_;
    return *this;
  }

 private:
  template <typename U>
  friend class WeakPtr;
  T* ptr_;
  WeakReference ref_;
};
}  // namespace base

#endif  // LYNX_BASE_WEAK_PTR_H_
