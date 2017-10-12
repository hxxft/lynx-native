// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_OBJECT_TEMPLATE_H_
#define LYNX_RUNTIME_LYNX_OBJECT_TEMPLATE_H_

#include <string>
#include <unordered_map>
#include <vector>
#include "base/scoped_ptr.h"

#include "JavaScriptCore/JavaScript.h"

namespace jscore {
    class LynxValue;
    class LynxArray;
    class ObjectTemplate;

    class LynxObjectTemplate {

    public:

        typedef base::ScopedPtr<jscore::LynxValue> (*LynxMethodCallback)
                (jscore::LynxObjectTemplate*, base::ScopedPtr<jscore::LynxArray>);
        typedef base::ScopedPtr<jscore::LynxValue> (*LynxGetPropertyCallback)
                (jscore::LynxObjectTemplate*);
        typedef void (*LynxSetPropertyCallback)
                (jscore::LynxObjectTemplate*, base::ScopedPtr<jscore::LynxValue>);

        struct Field{
            LynxSetPropertyCallback set_callback = 0;
            LynxGetPropertyCallback get_callback = 0;
        };

        LynxObjectTemplate();
        virtual ~LynxObjectTemplate();
        void RegisterMethodCallback(std::string method_name,
                                    LynxMethodCallback callback);
        void RegisterRawMethodCallback(std::string method_name,
                                       JSObjectCallAsFunctionCallback callback);
        void RegisterAccessorCallback(std::string field_name,
                                     LynxGetPropertyCallback get_callback,
                                     LynxSetPropertyCallback set_callback);

        base::ScopedPtr<LynxValue> MethodCallback(std::string name, base::ScopedPtr<LynxArray> value);
        void SetPropertyCallback(std::string name, base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetPropertyCallback(std::string name);

        inline void set_class_name(std::string class_name) {
            class_name_ = class_name;
        }

        inline std::string &class_name() {
            return class_name_;
        }

        inline const std::unordered_map<std::string, LynxMethodCallback>& methods() const {
            return methods_;
        }

        inline const std::unordered_map<std::string, JSObjectCallAsFunctionCallback>& raw_methods() const {
            return raw_methods_;
        }

        inline const std::unordered_map<std::string, Field>& fields() const {
            return fields_;
        }

        void set_object_wrap(ObjectTemplate* object_template) {
            object_template_ = object_template;
        }

        ObjectTemplate* object_wrap() {
            return object_template_;
        }

    private:

        std::unordered_map<std::string, LynxMethodCallback> methods_;
        std::unordered_map<std::string, JSObjectCallAsFunctionCallback> raw_methods_;
        std::unordered_map<std::string, Field> fields_;
        std::string class_name_;

        ObjectTemplate* object_template_;
    };
}

#endif //LYNX_RUNTIME_LYNX_OBJECT_TEMPLATE_H_
