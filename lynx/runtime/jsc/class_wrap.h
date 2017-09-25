// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_CLASS_WRAP_H_
#define LYNX_RUNTIME_JSC_CLASS_WRAP_H_

#include <string>
#include <vector>
#include <JavaScriptCore/JavaScript.h>
#include <third_party/JavaScriptCore/JavaScriptCore/JSObjectRef.h>
#include "runtime/jsc/object_wrap.h"

namespace jscore {
    class ClassWrap {
    public:
        
        ClassWrap() {
            Invalidate();
        }
        
        ClassWrap(const std::string& name, JSObjectFinalizeCallback callback) : finalize_callback_(callback){
            Invalidate();
            js_class_definition_.className = name.c_str();
        }

        ClassWrap(const std::string& name) : finalize_callback_(NULL) {
            Invalidate();
            js_class_definition_.className = name.c_str();
        }
        
        ~ClassWrap() {
            if (class_ref_ != 0) {
                JSClassRelease(class_ref_);
            }
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
        
        void SetJSStaticValue(JSStaticValue value) {
            js_static_values_.push_back(value);
        }
        
        void SetJSStaticValues(const JSStaticValue* values) {
            js_static_values_.clear();
            js_class_definition_.staticValues = values;
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
        
        void SetJSStaticFunction(JSStaticFunction function) {
            js_static_functions_.push_back(function);
        }
        
        void SetJSStaticFunctions(const JSStaticFunction* functions) {
            js_static_functions_.clear();
            js_class_definition_.staticFunctions = functions;
        }
        
        void SetInitializeCallback(JSObjectInitializeCallback initialize) {
            js_class_definition_.initialize = initialize;
        }
        
        void SetFinalizeCallback(JSObjectFinalizeCallback finalize) {
            js_class_definition_.finalize = finalize;
        }

        void SetSetPropertyCallback(JSObjectSetPropertyCallback callback) {
            js_class_definition_.setProperty = callback;
        }

        void SetGetPropertyCallback(JSObjectGetPropertyCallback callback) {
            js_class_definition_.getProperty = callback;
        }

        void SetCallAsFunctionCallback(JSObjectCallAsFunctionCallback callback) {
            js_class_definition_.callAsFunction = callback;
        }
        
        JSClassRef MakeClass() {
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
            return class_ref;
        }
        
        JSObjectRef MakeObject(JSContextRef context) {
            return MakeObject(context, NULL);
        }
        
        JSObjectRef MakeObject(JSContextRef context, void* data) {
            JSClassRef class_ref;
            if (class_ref_ != 0) {
                class_ref = class_ref_;
            } else {
                class_ref = MakeClass();
                class_ref_ = class_ref;
            }
            JSObjectRef object_ref = JSObjectMake(context, class_ref, data);
            return object_ref;
        }
        
        void Invalidate() {
            class_ref_ = 0;
            js_static_values_.clear();
            js_static_functions_.clear();
            attributes_ = kJSClassAttributeNone;
            js_class_definition_ = kJSClassDefinitionEmpty;
            // Default initialize and finalize callback
            js_class_definition_.initialize = ObjectWrap::InitializeCallback;
            js_class_definition_.finalize = finalize_callback_;
        }
        
    private:
        std::vector<JSStaticValue> js_static_values_;
        std::vector<JSStaticFunction> js_static_functions_;
        JSClassDefinition js_class_definition_;
        JSClassAttributes attributes_;
        JSClassRef class_ref_;
        const JSStaticFunction kJSStaticFunctionEndFlag = {0, 0, 0};
        const JSStaticValue kJSStaticValueEndFlag = {0, 0, 0, 0};
        JSObjectFinalizeCallback finalize_callback_;
    };
}

#endif // LYNX_RUNTIME_JSC_CLASS_WRAP_H_
