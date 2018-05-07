// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_OBJECT_WRAP_H_
#define LYNX_RUNTIME_JSC_OBJECT_WRAP_H_

#include <assert.h>
#include <JavaScriptCore/JavaScript.h>

#include "base/observer/observer.h"
#include "base/debug/memory_debug.h"
#include "runtime/js/object_wrap.h"

namespace jscore {
    class JSCContext;
    class LynxObject;

    class JSCObjectWrap : public ObjectWrap {
    public:

        virtual ~JSCObjectWrap();
        
        virtual void Update();

        void RegisterMethodCallback(const std::string& method_name, JSMethodCallback callback);

        void Protect();

        void Unprotect();
        
        static void Wrap(JSCContext* context, LynxObject* obj, JSObjectRef js_obj) {
            lynx_new JSCObjectWrap(context, obj, js_obj);
        }

        template<class T>
        static T* Unwrap(JSObjectRef object) {
            assert(object != NULL);
            //assert(JSObjectGetPrivate(object) != NULL);
            void* data = JSObjectGetPrivate(object);
            JSCObjectWrap* wrap = static_cast<JSCObjectWrap*>(data);
            return static_cast<T*>(wrap->lynx_object());
        }

        inline JSObjectRef js_ref() {
            return js_object_;
        }

        static void FinalizeCallback(JSObjectRef object);

    private:

        void OnJSObjectInitialize();
        void OnJSObjectFinalize();

        JSCObjectWrap(JSCContext* context, LynxObject* obj, JSObjectRef js_obj);

        JSObjectRef js_object_;
        
    };
}

#endif // LYNX_RUNTIME_JSC_OBJECT_WRAP_H_
