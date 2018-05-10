// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_PROTOTYPE_BUILDER_H_
#define LYNX_RUNTIME_JSC_PROTOTYPE_BUILDER_H_

#include <JavaScriptCore/JavaScript.h>
#include "runtime/js/class_template.h"

namespace jscore {
    class JSCPrototypeBuilder : public ClassTemplate::PrototypeBuilder {
    public:

        JSCPrototypeBuilder() : class_ref_(NULL),
                                js_static_values_(),
                                js_static_functions_(),
                                attributes_(kJSClassAttributeNone),
                                js_class_definition_() {
            // Default finalize callback
            js_class_definition_.finalize = JSCObjectWrap::FinalizeCallback;
        }

        ~JSCPrototypeBuilder() {
            if (class_ref_ != 0) {
                JSClassRelease(class_ref_);
            }
        }

        void SetJSClassName(const std::string &name) {
            js_class_definition_.className = name.c_str();
        }

        void SetJSClassAttributes(JSClassAttributes attributes) {
            attributes_ = attributes;
        }

        void SetJSStaticValue(const char* name,
                              JSObjectGetPropertyCallback getProperty,
                              JSObjectSetPropertyCallback setProperty,
                              JSPropertyAttributes attributes) {
            js_static_values_.push_back({
                                                name,
                                                getProperty,
                                                setProperty,
                                                attributes_
                                        });
        }

        void SetJSStaticFunction(const char* name,
                                 JSObjectCallAsFunctionCallback callAsFunction,
                                 JSPropertyAttributes attributes) {
            js_static_functions_.push_back({
                                                   name,
                                                   callAsFunction,
                                                   attributes
                                           });
        }

        void SetParentClass(JSCPrototypeBuilder* parent_class) {
            if (parent_class != NULL) {
                js_class_definition_.parentClass = parent_class->class_ref();
            }
        }

        JSObjectRef MakeObject(JSContextRef context, void* data = NULL) {
            JSObjectRef object_ref = JSObjectMake(context, class_ref_, data);
            return object_ref;
        }

        void FinishCreation(ClassTemplate* class_template);

        inline JSClassRef class_ref() {
            return class_ref_;
        }

    private:

        JSClassRef MakeClass() {
            if (class_ref_ != NULL) return class_ref_;
            js_class_definition_.attributes = attributes_;
            if (!js_static_values_.empty()) {
                js_static_values_.push_back(kJSStaticValueEndFlag);
                js_class_definition_.staticValues = &js_static_values_.front();
            }
            if (!js_static_functions_.empty()) {
                js_static_functions_.push_back(kJSStaticFunctionEndFlag);
                js_class_definition_.staticFunctions = &js_static_functions_.front();
            }
            JSClassRef class_ref = JSClassCreate(&js_class_definition_);
            class_ref_ = class_ref;
            return class_ref;
        }

        JSClassRef class_ref_;
        std::vector<JSStaticValue> js_static_values_;
        std::vector<JSStaticFunction> js_static_functions_;
        JSClassAttributes attributes_;
        JSClassDefinition js_class_definition_;
        const JSStaticFunction kJSStaticFunctionEndFlag = {0, 0, 0};
        const JSStaticValue kJSStaticValueEndFlag = {0, 0, 0, 0};
    };
}

#endif //LYNX_RUNTIME_JSC_PROTOTYPE_BUILDER_H_