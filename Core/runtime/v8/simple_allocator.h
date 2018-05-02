// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#ifndef LYNX_RUNTIME_V8_SIMPLE_ALLOCATOR_H_
#define LYNX_RUNTIME_V8_SIMPLE_ALLOCATOR_H_

#include "v8.h"

namespace jscore {
class SimpleAllocator: public v8::ArrayBuffer::Allocator {
 public:
    SimpleAllocator();

    ~SimpleAllocator() override;

    void* Allocate(size_t length) override;

    void* AllocateUninitialized(size_t length) override;

    void Free(void* data, size_t length) override;
};

}  // namespace jscore
#endif

#endif  // LYNX_RUNTIME_V8_SIMPLE_ALLOCATOR_H_
