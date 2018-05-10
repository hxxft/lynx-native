// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js/js_vm.h"

#include "v8.h"
#include "libplatform/libplatform.h"
#include "runtime/v8/simple_allocator.h"

namespace {
bool g_is_vm_initialized_ = false;
    jscore::SimpleAllocator g_allocator;
}
namespace jscore {

    JSVM::~JSVM() {
        if(vm_) {
            v8::Isolate* isolate = static_cast<v8::Isolate*>(vm_);
            isolate->Dispose();
        }
    }

void JSVM::Initialize() {
    if (!g_is_vm_initialized_) {
        const char* flag = "--noflush_code --noage_code --nocompact_code_space --expose_gc";
        v8::V8::SetFlagsFromString(flag, static_cast<int>(strlen(flag)));

        v8::V8::InitializeICU();
        v8::V8::InitializeExternalStartupData("");
        v8::V8::InitializePlatform(v8::platform::CreateDefaultPlatform());
        v8::V8::Initialize();
        g_is_vm_initialized_ = true;
    }
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &g_allocator;
    vm_ = static_cast<void*>(v8::Isolate::New(create_params));
}
}  // namespace jscore
