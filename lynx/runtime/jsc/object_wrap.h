// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_OBJECT_WRAP_H_
#define LYNX_RUNTIME_JSC_OBJECT_WRAP_H_

#include <assert.h>
#include <JavaScriptCore/JavaScript.h>

#include "base/observer/observer.h"
#include "runtime/js_context.h"
#include "runtime/jsc/jsc_context.h"
#include "base/debug/memory_debug.h"


namespace jscore {
    class ObjectWrap : base::Observer{
    public:
        
        ObjectWrap(JSContext* context) : js_ref_(NULL), context_(context){
            context_->AddObserver(this);
        }
        
        virtual ~ObjectWrap() {
            context_->RemoveObserver(this);
        }
        
        virtual void Update() {
            if(js_ref_) {
                Wrap(NULL, js_ref_);
            }
            lynx_delete(this);
        }
        
        JSObjectRef js_ref() {
            return js_ref_;
        }

        void Protect() {
            if (js_ref_ != NULL) {
                JSContextRef context = static_cast<JSCContext*>(context_)->GetContext();
                JSValueProtect(context, js_ref_);
            }
        }

        void Unprotect() {
            if (js_ref_ != NULL) {
                JSContextRef context = static_cast<JSCContext*>(context_)->GetContext();
                JSValueUnprotect(context, js_ref_);
            }
        }
        
        template<class T>
        static inline T* Unwrap(JSObjectRef object) {
            assert(object != NULL);
            //assert(JSObjectGetPrivate(object) != NULL);
            void* data = JSObjectGetPrivate(object);
            ObjectWrap* wrap = static_cast<ObjectWrap*>(data);
            return static_cast<T*>(wrap);
        }
        
        static inline void Wrap(ObjectWrap* wrap, JSObjectRef object) {
            assert(object != NULL);
            //assert(JSObjectGetPrivate(object) == NULL);
            assert(JSObjectSetPrivate(object, wrap));
            if(wrap != NULL) {
                wrap->js_ref_ = object;
            }
        }
        
        static void InitializeCallback(JSContextRef ctx, JSObjectRef object) {
        }
        
        static void FinalizeCallback(JSObjectRef object) {
            ObjectWrap* object_wrap = Unwrap<ObjectWrap>(object);
            if(object_wrap != NULL) {
                lynx_delete(object_wrap);
            }
        }
        
    private:
        JSObjectRef js_ref_;
        JSContext* context_;
        
    };
}

#endif // LYNX_RUNTIME_JSC_OBJECT_WRAP_H_
