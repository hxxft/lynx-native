// Copyright 2017 The Lynx Authors. All rights reserved.

#include <sstream>
#include "runtime/base/lynx_value.h"

#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_array.h"
#include "runtime/platform_value.h"

namespace jscore {
    LynxValue::LynxValue() {

    }

    LynxValue::LynxValue(Type type) : type_(type) {

    }

    LynxValue::~LynxValue() {
        if (type_ == VALUE_LYNX_ARRAY && data_.lynx_array != 0) {
            lynx_delete(data_.lynx_array);
        } else if (type_ == VALUE_LYNX_MAP && data_.lynx_map != 0) {
            lynx_delete(data_.lynx_map);
        } else if (type_ == VALUE_PLATFORM && data_.platform_value != 0) {
            lynx_delete(data_.platform_value);
        } else if (type_ == VALUE_STRING && data_.str != 0) {
            lynx_deleteA(data_.str);
        }
    }

    base::ScopedPtr<LynxValue> LynxValue::MakeValueScoped(LynxValue* js_value) {
        return base::ScopedPtr<LynxValue>(js_value);
    }

    base::ScopedPtr<LynxArray> LynxValue::MakeArrayScoped(LynxArray* lynx_array) {
        return base::ScopedPtr<LynxArray>(lynx_array);
    }

    base::ScopedPtr<LynxMap> LynxValue::MakeMapScoped(LynxMap *lynx_object) {
        return base::ScopedPtr<LynxMap>(lynx_object);
    }

    std::string LynxValue::ToString() {
        std::string result = "";
        switch (type_) {
            case LynxValue::Type::VALUE_INT: {
                double number = data_.i;
                std::stringstream stream;
                stream << number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_DOUBLE: {
                double number = data_.d;
                std::stringstream stream;
                stream << number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_BOOL: {
                if (data_.b) {
                    result = "true";
                } else {
                    result = "false";
                }
            }
                break;
            case LynxValue::Type::VALUE_STRING:
                result = data_.str;
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT:
                result = "LynxObject";
                break;
            case LynxValue::Type::VALUE_NULL:
                result = "null";
                break;
            case LynxValue::Type::VALUE_UNDEFINED:
                result = "undefined";
                break;
            default:
                break;
        }
        return result;
    }
}