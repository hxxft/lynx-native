// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECT_WRAP_H_
#define LYNX_RUNTIME_V8_OBJECT_WRAP_H_

#if USING_V8

#include "v8.h"
#include <assert.h>
#include "runtime/js/object_wrap.h"
#include "base/debug/memory_debug.h"

namespace jscore {
class V8Context;

class V8ObjectWrap : public ObjectWrap {
 public:
    V8ObjectWrap(V8Context* context, LynxObject* object, v8::Local<v8::Object> handle);

    virtual ~V8ObjectWrap();

    virtual void Update();

    virtual void Protect() {
        assert(!persistent().IsEmpty());
        persistent().ClearWeak();
        refs_++;
    }

    virtual void Unprotect() {
        assert(!persistent().IsEmpty());
        assert(!persistent().IsWeak());
        assert(refs_ > 0);
        if (--refs_ == 0)
            MakeWeak();
    }

    virtual void RegisterMethodCallback(const std::string& method_name, JSMethodCallback callback);
    virtual void OnJSObjectInitialize();
    virtual void OnJSObjectFinalize();

    template<class T>
    static inline T *Unwrap(v8::Local<v8::Object> handle) {
        assert(!handle.IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        // Cast to ObjectWrap before casting to T.  A direct cast from void
        // to T won't work right when T has more than one base class.
        void *ptr = handle->GetAlignedPointerFromInternalField(0);
        ObjectWrap *wrap = static_cast<ObjectWrap *>(ptr);
        return static_cast<T *>(wrap->lynx_object());
    }

    static inline void Wrap(V8Context* context, LynxObject* lynx_object, v8::Local<v8::Object> handle) {
        lynx_new V8ObjectWrap(context, lynx_object, handle);
    }

    inline v8::Local <v8::Object> handle() {
        return handle(v8::Isolate::GetCurrent());
    }

    inline v8::Local <v8::Object> handle(v8::Isolate *isolate) {
        return v8::Local<v8::Object>::New(isolate, persistent());
    }

    inline v8::Persistent <v8::Object> &persistent() {
        return handle_;
    }

 protected:
    inline void Wrap(v8::Isolate* isolate, v8::Local<v8::Object> handle) {
        assert(persistent().IsEmpty());
        assert(handle->InternalFieldCount() > 0);
        handle->SetAlignedPointerInInternalField(0, this);
        persistent().Reset(isolate, handle);
        MakeWeak();
    }

    inline void MakeWeak(void) {
        persistent().SetWeak(this,
                             WeakCallback,
                             v8::WeakCallbackType::kParameter);
        persistent().MarkIndependent();
    }

    static void WeakCallback(
            const v8::WeakCallbackInfo<V8ObjectWrap>& data) {
        v8::Isolate *isolate = data.GetIsolate();
        v8::HandleScope scope(isolate);
        V8ObjectWrap *wrap = data.GetParameter();
        assert(wrap->refs_ == 0);
        assert(wrap->handle_.IsNearDeath());
        wrap->handle_.Reset();
        lynx_delete(wrap);
    }

 private:

    v8::Persistent<v8::Object> handle_;

    int refs_;
};

}  // namespace jscore
#endif

#endif  // LYNX_RUNTIME_V8_OBJECT_WRAP_H_
