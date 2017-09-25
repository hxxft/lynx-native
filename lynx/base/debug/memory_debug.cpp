// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/debug/memory_debug.h"
#include "base/debug/memory_tracker.h"

#if DEBUG_MEMORY
void * operator new (size_t size, const char* file, int line) {
    if(0 == size){
        return NULL;
    }
    void *p = malloc(size);
    base::MemoryTracker::Instance()->AddMemInfo(reinterpret_cast<intptr_t>(p), file, line);
    return p;
}

void * operator new [](size_t size, const char* file, int line) {
    void *p = ::operator new(size);
    base::MemoryTracker::Instance()->AddMemInfo(reinterpret_cast<intptr_t>(p), file, line);
    return p;
}

void operator delete (void * pointer, const char* file, int line) noexcept{
    base::MemoryTracker::Instance()->RemoveMemInfo(reinterpret_cast<intptr_t>(pointer));
    if(0 != pointer){
        free(pointer);
    }
}

void operator delete[](void * pointer, const char* file, int line) noexcept{
    base::MemoryTracker::Instance()->RemoveMemInfo(reinterpret_cast<intptr_t>(pointer));
    ::operator delete(pointer);
}

#endif