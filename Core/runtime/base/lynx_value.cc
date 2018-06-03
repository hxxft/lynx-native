// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_value.h"
#include <sstream>

#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_object.h"
#include "runtime/platform_value.h"

namespace jscore {
LynxValue::LynxValue() {}

LynxValue::LynxValue(Type type) : type_(type) {}

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

std::string LynxValue::ToString() {
  std::string result = "";
  switch (type_) {
    case LynxValue::Type::VALUE_INT: {
      double number = data_.i;
      std::stringstream stream;
      stream << number;
      result = stream.str();
    } break;
    case LynxValue::Type::VALUE_DOUBLE: {
      double number = data_.d;
      std::stringstream stream;
      stream << number;
      result = stream.str();
    } break;
    case LynxValue::Type::VALUE_BOOL: {
      if (data_.b) {
        result = "true";
      } else {
        result = "false";
      }
    } break;
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

LynxValue* LynxValue::Clone() {
  LynxValue* v = nullptr;
  switch (type_) {
    case LynxValue::Type::VALUE_INT:
      v = MakeInt(data_.i).Release();
      break;
    case LynxValue::Type::VALUE_DOUBLE:
      v = MakeDouble(data_.d).Release();
      break;
    case LynxValue::Type::VALUE_BOOL:
      v = MakeBool(data_.b).Release();
      break;
    case LynxValue::Type::VALUE_STRING: {
      std::string str(data_.str);
      v = MakeString(str).Release();
    } break;
    case LynxValue::Type::VALUE_LYNX_OBJECT:
    case LynxValue::Type::VALUE_NULL:
    case LynxValue::Type::VALUE_UNDEFINED:

      break;
    default:
      break;
  }
  return v;
}
}  // namespace jscore
