// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_VALUE_H_
#define LYNX_RUNTIME_LYNX_VALUE_H_

#include "base/scoped_ptr.h"
#include <string>

namespace jscore {

    class LynxArray;
    class LynxObject;
    class LynxObjectTemplate;
    class ObjectWrap;
    class PlatformValue;
    class LynxFunctionObject;
    class LynxFunction;

    class LynxValue {
    public:
        enum Type {
            VALUE_INT,
            VALUE_LONG,
            VALUE_BOOL,
            VALUE_FLOAT,
            VALUE_DOUBLE,
            VALUE_STRING,
            VALUE_LYNX_OBJECT,
            VALUE_LYNX_ARRAY,
            VALUE_LYNX_FUNCTION_OBJECT,
            VALUE_LYNX_OBJECT_TEMPLATE,
            VALUE_LYNX_FUNCTION,
            VALUE_PLATFORM,
            VALUE_NULL,
        };

        Type type_;
        union {
            int i;
            bool b;
            long l;
            float f;
            double d;
            const char* str;
            LynxArray* lynx_array;
            LynxObject* lynx_object;
            LynxFunctionObject* lynx_function_object;
            LynxObjectTemplate* lynx_object_template;
            LynxFunction* lynx_function;
            PlatformValue* platform_value;
        } data_;

        inline static base::ScopedPtr<LynxValue> MakeBool(bool value) {
            base::ScopedPtr<LynxValue> lynx_value(lynx_new LynxValue(LynxValue::Type::VALUE_BOOL));
            lynx_value->data_.b = value;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakeLong(long value) {
            base::ScopedPtr<LynxValue> lynx_value(lynx_new LynxValue(LynxValue::Type::VALUE_LONG));
            lynx_value->data_.l = value;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakeInt(int value) {
            base::ScopedPtr<LynxValue> lynx_value(lynx_new LynxValue(LynxValue::Type::VALUE_INT));
            lynx_value->data_.i = value;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakeFloat(float value) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_FLOAT));
            lynx_value->data_.f = value;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakeDouble(double value) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_DOUBLE));
            lynx_value->data_.d = value;
            return lynx_value;
        }

        static base::ScopedPtr<LynxValue> MakeString(std::string& value) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_STRING));
            char* str = lynx_new char[value.size() + 1];
            strcpy(str, value.c_str());
            lynx_value->data_.str = str;
            return lynx_value;
        }

        static base::ScopedPtr<LynxValue> MakeString(const std::string& value) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_STRING));
            char* str = lynx_new char[value.size() + 1];
            strcpy(str, value.c_str());
            lynx_value->data_.str = str;
            return lynx_value;
        }

        static base::ScopedPtr<LynxValue> MakeString(std::string&& value) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_STRING));
            char* str = lynx_new char[value.size() + 1];
            strcpy(str, value.c_str());
            lynx_value->data_.str = str;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakeFunctionObject(LynxFunctionObject* object) {
            base::ScopedPtr<LynxValue> lynx_object(
                    lynx_new LynxValue(LynxValue::Type::VALUE_LYNX_FUNCTION_OBJECT));
            lynx_object->data_.lynx_function_object = object;
            return lynx_object;
        }

        inline static base::ScopedPtr<LynxValue> MakeObjectTemplate(LynxObjectTemplate *object) {
            base::ScopedPtr<LynxValue> lynx_object(
                    lynx_new LynxValue(LynxValue::Type::VALUE_LYNX_OBJECT_TEMPLATE));
            lynx_object->data_.lynx_object_template = object;
            return lynx_object;
        }

        inline static base::ScopedPtr<LynxValue> MakeLynxFunction(LynxFunction* function) {
            base::ScopedPtr<LynxValue> lynx_value(
                    lynx_new LynxValue(LynxValue::Type::VALUE_LYNX_FUNCTION));
            lynx_value->data_.lynx_function = function;
            return lynx_value;
        }

        inline static base::ScopedPtr<LynxValue> MakePlatformValue(PlatformValue* platform_value) {
            base::ScopedPtr<LynxValue> lynx_object(
                    lynx_new LynxValue(LynxValue::Type::VALUE_PLATFORM));
            lynx_object->data_.platform_value = platform_value;
            return lynx_object;
        }

        static base::ScopedPtr<LynxValue> MakeValueScoped(LynxValue* lynx_value) ;

        static base::ScopedPtr<LynxArray> MakeArrayScoped(LynxArray* lynx_array) ;

        static base::ScopedPtr<LynxObject> MakeObjectScoped(LynxObject* lynx_object) ;

        virtual ~LynxValue();

    protected:

        LynxValue();

        LynxValue(Type type);

    };
}


#endif //LYNX_RUNTIME_LYNX_VALUE_H_
