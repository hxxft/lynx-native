// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_DEBUG_MEMORY_DEBUG_H_
#define LYNX_BASE_DEBUG_MEMORY_DEBUG_H_

#include <cstdlib>
#include "base/debug/memory_tracker.h"

#if DEBUG_MEMORY

void * operator new (size_t size, const char* file, int line) noexcept;
void * operator new [](size_t size, const char* file, int line) noexcept;
void operator delete (void * pointer, const char* file, int line) noexcept;
void operator delete[](void * pointer, const char* file, int line) noexcept;

#define lynx_new new(__FILE__, __LINE__)
#define lynx_delete(ptr) \
        do { \
             base::MemoryTracker::Instance()->RemoveMemInfo(reinterpret_cast<intptr_t>(ptr)); \
            delete ptr; \
        }while(0); \

#define lynx_deleteA(ptr) \
        do { \
             base::MemoryTracker::Instance()->RemoveMemInfo(reinterpret_cast<intptr_t>(ptr)); \
            delete[] ptr; \
        }while(0); \


#else

#define lynx_new new
#define lynx_delete(ptr) \
            delete ptr; \

#define lynx_deleteA(ptr) \
            delete[] ptr; \

#endif
#endif
