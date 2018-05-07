// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_OBJECT_H_
#define LYNX_RUNTIME_LYNX_OBJECT_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "base/scoped_ptr.h"
#include "runtime/js/defines.h"

namespace jscore {
    class LynxValue;
    class LynxArray;
    class ObjectWrap;
    class ClassTemplate;
    class JSContext;

    class LynxObject {

    public:

        LynxObject(JSContext* context, ClassTemplate* class_template);
        virtual ~LynxObject();

        void RegisterMethodCallback(const std::string& method_name,
                                    JSMethodCallback callback);

        void SetObjectWrap(ObjectWrap* object_template);

        inline void set_class_template(ClassTemplate* class_template) {
            class_template_ = class_template;
        }

        inline ObjectWrap* object_wrap() {
            return object_wrap_;
        }

        inline ClassTemplate* class_template() {
            return class_template_;
        }

        inline JSContext* context() {
            return context_;
        }

        void ProtectJSObject();
        void UnprotectJSObject();

    protected:
        JSContext* context_;

    private:
        ObjectWrap* object_wrap_;
        ClassTemplate* class_template_;

        std::unordered_map<std::string, JSMethodCallback> methods_;
        int protect_times_;
    };
}

#endif //LYNX_RUNTIME_LYNX_OBJECT_H_
