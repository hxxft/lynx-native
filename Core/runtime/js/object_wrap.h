// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JS_OBJECT_WRAP_H_
#define LYNX_RUNTIME_JS_OBJECT_WRAP_H_

#include "base/scoped_ptr.h"
#include "base/observer/observer.h"
#include "base/debug/memory_debug.h"
#include "runtime/js/base.h"

namespace jscore {
    class JSContext;
    class LynxObject;

    class ObjectWrap : base::Observer {
    public:

        ObjectWrap(JSContext* context, LynxObject* obj);

        virtual ~ObjectWrap();

        virtual void Update() = 0;

        inline LynxObject* lynx_object() {
            return lynx_object_.Get();
        }

    protected:

        virtual void RegisterMethodCallback(const std::string& method_name,
                                            JSMethodCallback callback) = 0;

        virtual void Protect() = 0;

        virtual void Unprotect() = 0;

        virtual void OnJSObjectInitialize() = 0;
        virtual void OnJSObjectFinalize() = 0;

        JSContext* context_;
        base::ScopedPtr<LynxObject> lynx_object_;

        friend class LynxObject;

    };
}

#endif // LYNX_RUNTIME_JSC_OBJECT_WRAP_H_
