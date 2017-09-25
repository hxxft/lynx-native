// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_OBJECTS_LOCATION_OBJECT_H_
#define LYNX_RUNTIME_JSC_OBJECTS_LOCATION_OBJECT_H_

#include "base/scoped_ptr.h"
#include "runtime/location.h"
#include "runtime/jsc/object_wrap.h"

namespace jscore {
    
    class JSCContext;
    class ClassWrap;
    
    class LocationObject : public ObjectWrap {
    public:
        LocationObject(JSCContext* context, Location* location);
        virtual ~LocationObject();
        static void BindingClass(ClassWrap* class_wrap);
        static JSObjectRef Create(JSCContext* context, ClassWrap* class_wrap);

        Location* target() {
            return target_.Get();
        }

        base::ScopedPtr<Location> target_;
    };
}

#endif // LYNX_RUNTIME_JSC_OBJECTS_LOCATION_OBJECT_H_
