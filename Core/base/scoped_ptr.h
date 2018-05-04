// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_SCOPED_PTR_H_
#define LYNX_BASE_SCOPED_PTR_H_

#include <cstddef>
#include <memory>

#include "base/debug/memory_debug.h"
#include "base/debug/memory_tracker.h"

namespace base {

template <class T>
class ScopedPtr {
 public:
  using element_type = T;

  ScopedPtr(const ScopedPtr<T>& other) {
    ptr_ = other.ptr_;
    const_cast<ScopedPtr<T>*>(&other)->ptr_ = NULL;
  }

  template <typename U>
  ScopedPtr(ScopedPtr<U>&& other) {
    ptr_ = other.Release();
  }

  explicit ScopedPtr(element_type* p) : ptr_(p) {}

  ScopedPtr() : ptr_(nullptr) {}

  ~ScopedPtr() {
    if (ptr_) {
      lynx_delete(static_cast<element_type*>(ptr_));
    }
    ptr_ = nullptr;
  }

  ScopedPtr& operator=(ScopedPtr&& other) {
    Reset(other.Release());
    return *this;
  }

  ScopedPtr& operator=(ScopedPtr& other) {
    Reset(other.Release());
    return *this;
  }

  template <typename U>
  ScopedPtr& operator=(ScopedPtr<U>&& other) {
    Reset(other.Release());
    return *this;
  }

  template <typename U>
  ScopedPtr& operator=(ScopedPtr<U>& other) {
    Reset(other.Release());
    return *this;
  }

  void Swap(ScopedPtr& scoped_ptr) {
    element_type* ptr = ptr_;
    ptr_ = scoped_ptr.ptr_;
    scoped_ptr.ptr_ = ptr;
  }

  void Reset(element_type* p = nullptr) {
    element_type* old_ptr = ptr_;
    ptr_ = p;
    if (old_ptr) {
      lynx_delete(static_cast<element_type*>(old_ptr));
    }
  }

  element_type* Release() {
    element_type* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  element_type* Get() const { return ptr_; }

  element_type& operator*() const { return *ptr_; }
  element_type* operator->() const { return ptr_; }

 private:
  element_type* ptr_;
};

template <class T>
class ScopedPtr<T[]> {
 public:
  using element_type = T;

  ScopedPtr(const ScopedPtr<element_type[]>& other) {
    ptr_ = other.ptr_;
    const_cast<ScopedPtr<element_type[]>*>(&other)->ptr_ = NULL;
  }

  ScopedPtr(ScopedPtr<element_type[]>& other) {
    ptr_ = other.ptr_;
    other.ptr_ = NULL;
  }

  explicit ScopedPtr(element_type* p) : ptr_(p) {}

  ScopedPtr() : ptr_(nullptr) {}

  ~ScopedPtr() {
    if (ptr_) {
      lynx_deleteA(static_cast<element_type*>(ptr_));
    }
  }

  ScopedPtr& operator=(ScopedPtr&& other) {
    Reset(other.Release());
    return *this;
  }

  ScopedPtr& operator=(ScopedPtr& other) {
    Reset(other.Release());
    return *this;
  }

  void Reset(element_type* p = nullptr) {
    element_type* old_ptr = ptr_;
    ptr_ = p;
    if (old_ptr) {
      lynx_deleteA(static_cast<element_type*>(old_ptr));
    }
  }

  element_type* Release() {
    element_type* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  element_type* Get() const { return ptr_; }

  element_type* operator->() const { return ptr_; }

  element_type& operator[](size_t index) { return ptr_[index]; }

 private:
  element_type* ptr_;
};

}  // namespace base

#endif  // LYNX_BASE_SCOPED_PTR_H_
