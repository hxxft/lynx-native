// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8
#include <cstdlib>
#include <string>

#include "runtime/v8/simple_allocator.h"

namespace jscore {

SimpleAllocator::SimpleAllocator() {
}

SimpleAllocator::~SimpleAllocator() {
}

void *SimpleAllocator::Allocate(size_t length) {
    void *data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
}

void *SimpleAllocator::AllocateUninitialized(size_t length) {
    void *data = malloc(length);
    return data;
}

void SimpleAllocator::Free(void *data, size_t length) {
    free(data);
}

}  // namespace jscore
#endif
