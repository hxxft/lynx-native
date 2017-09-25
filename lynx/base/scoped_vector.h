// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_SCOPED_VECTOR_H_
#define LYNX_BASE_SCOPED_VECTOR_H_

#include <vector>
#include "base/debug/memory_debug.h"

namespace base {
template<class T>
class ScopedVector {
 public:
    typedef typename std::vector<T*>::iterator iterator;

    ScopedVector() {}
    ScopedVector(ScopedVector&& other) { vector_.swap(other.vector_); }

    ~ScopedVector() { clear(); }

    ScopedVector& operator= (ScopedVector&& other) {
        vector_.swap(other.vector_);
        return *this;
    }

    iterator begin() {return vector_.begin();}
    iterator end() {return vector_.end();}

    int size() { return vector_.size(); }

    bool empty() { return vector_.empty(); }

    T* operator[] (int index) {
        return vector_[index];
    }

    void push_back(T* t) {
        vector_.push_back(t);
    }

    void pop_back() {
        T* t = vector_.at(vector_.size() - 1);
        lynx_delete(t);
        vector_.pop_back();
    }

    T* at(int n) {
        return vector_.at(n);
    }

    iterator erase(iterator iter) {
        lynx_delete(*iter);
        return vector_.erase(iter);
    }

    void clear() {
        iterator iter = vector_.begin();
        for (iter; iter != vector_.end(); ++iter) {
            lynx_delete(*iter);
        }
        vector_.clear();
    }

 private:
    std::vector<T*> vector_;
};
}  // namespace base

#endif  // LYNX_BASE_SCOPED_VECTOR_H_
