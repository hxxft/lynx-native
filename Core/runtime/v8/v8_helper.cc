// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "base/android/android_jni.h"
#include "runtime/global.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_holder.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"
#include "runtime/v8/v8_function.h"
#include "runtime/v8/object_wrap.h"
#include "runtime/v8/prototype_builder.h"

namespace jscore {

    base::ScopedPtr<LynxValue> V8Helper::ConvertToLynxValue(v8::Local<v8::Context> context,
                                                            v8::Local<v8::Value> value) {
        base::ScopedPtr<LynxValue> v8_value;
        if (value->IsNull()) {
            v8_value = LynxValue::MakeNull();
        } else if (value->IsUndefined()) {
            v8_value = LynxValue::MakeUndefined();
        } else if (value->IsBoolean() || value->IsBooleanObject()) {
            v8_value = LynxValue::MakeBool(value->BooleanValue(context).ToChecked());
        } else if (value->IsNumber() || value->IsNumberObject()) {
            if (value->IsInt32()) {
                v8_value = LynxValue::MakeInt(value->Int32Value(context).ToChecked());
            } else if (value->IsUint32()) {
                v8_value = LynxValue::MakeInt(value->Uint32Value(context).ToChecked());
            } else {
                v8_value = LynxValue::MakeDouble(value->NumberValue(context).ToChecked());
            }
        } else if (value->IsString() || value->IsStringObject()) {
            v8_value = LynxValue::MakeString(ConvertToString(value->ToString(context).ToLocalChecked()));
        } else if (value->IsArray()) {
            auto v8_array = v8::Local<v8::Array>::Cast(value);
            v8_value = base::ScopedPtr<LynxValue>(ConvertToLynxArray(context, v8_array).Release());
        } else if (value->IsFunction()) {
            auto v8_function = v8::Local<v8::Function>::Cast(value);
            v8_value = ConvertToLynxFunction(context, v8_function);
        } else if (value->IsObject()) {
            auto v8_object = v8::Local<v8::Object>::Cast(value);
            if (v8_object->InternalFieldCount() > 0) {
                v8_value = LynxValue::MakeObject(V8ObjectWrap::Unwrap<LynxObject>(v8_object));
            } else {
                v8_value = base::ScopedPtr<LynxValue>(ConvertToLynxMap(context, v8_object).Release());
            }
        }
        return v8_value;
    }

    base::ScopedPtr<LynxArray> V8Helper::ConvertToLynxArray(v8::Local<v8::Context> context,
                                                            v8::Local<v8::Array> array) {
        int length = array->Length();
        base::ScopedPtr<LynxArray> lynx_array(new LynxArray);
        for (int i = 0; i < length; ++i) {
            lynx_array->Push(ConvertToLynxValue(context,
                                                array->Get(context, i).ToLocalChecked()).Release());
        }
        return lynx_array;
    }

    base::ScopedPtr<LynxArray> V8Helper::ConvertToLynxArray(v8::Local<v8::Context> context,
                                                            const v8::FunctionCallbackInfo<v8::Value>& info) {
        int length = info.Length();
        base::ScopedPtr<LynxArray> lynx_array(new LynxArray);
        for (int i = 0; i < length; ++i) {
            lynx_array->Push(ConvertToLynxValue(context, info[i]).Release());
        }
        return lynx_array;
    }

    base::ScopedPtr<LynxMap> V8Helper::ConvertToLynxMap(v8::Local<v8::Context> context,
                                                        v8::Local<v8::Object> object) {
        auto properties = object->GetPropertyNames();
        int length = properties->Length();
        base::ScopedPtr<LynxMap> lynx_map(new LynxMap);
        for (int i = 0; i < length; ++i) {
            auto property = properties->Get(i);
            if (property->IsString() || property->IsStringObject()) {
                std::string key = ConvertToString(property->ToString());
                auto value = ConvertToLynxValue(context, object->Get(property));
                lynx_map->Set(key, value.Release());
            }
        }
        return lynx_map;
    }

    base::ScopedPtr<LynxValue> V8Helper::ConvertToLynxFunction(v8::Local<v8::Context> context,
                                                               v8::Local<v8::Function> func) {
        Global* global = V8ObjectWrap::Unwrap<Global>(context->Global());
        V8Function* function = lynx_new V8Function(static_cast<V8Context*>(global->context()), func);
        return LynxValue::MakeLynxFunction(function);
    }

    v8::Local<v8::Value> V8Helper::ConvertToV8Value(v8::Isolate* isolate, LynxValue* value) {
        if (value == 0) {
            return v8::Local<v8::Value>(v8::Null(isolate));
        }
        v8::Local<v8::Value> v8_obj;
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT:
                v8_obj = ConvertToV8Int(isolate, value);
                break;
            case LynxValue::Type::VALUE_LONG:
                v8_obj = ConvertToV8Long(isolate, value);
                break;
            case LynxValue::Type::VALUE_FLOAT:
                v8_obj = ConvertToV8Float(isolate, value);
                break;
            case LynxValue::Type::VALUE_DOUBLE:
                v8_obj = ConvertToV8Double(isolate, value);
                break;
            case LynxValue::Type::VALUE_BOOL:
                v8_obj = ConvertToV8Boolean(isolate, value);
                break;
            case LynxValue::Type::VALUE_STRING:
                v8_obj = ConvertToV8String(isolate, value);
                break;
            case LynxValue::Type::VALUE_LYNX_ARRAY:
                v8_obj = ConvertToV8Array(isolate, value->data_.lynx_array);
                break;
            case LynxValue::Type::VALUE_LYNX_MAP:
                v8_obj = ConvertToV8Object(isolate, value->data_.lynx_map);
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT:
                v8_obj = ConvertToV8Object(isolate, value->data_.lynx_object);
                break;
            case LynxValue::Type::VALUE_LYNX_HOLDER:
                v8_obj = ConvertToV8Value(isolate, value->data_.lynx_holder->GetLynxValue().Get());
                break;
            case LynxValue::Type::VALUE_NULL:
                v8_obj = v8::Null(isolate);
                break;
            case LynxValue::Type::VALUE_UNDEFINED:
            default:
                v8_obj = v8::Undefined(isolate);
                break;
        }
        return v8_obj;
    }

    v8::Local<v8::Array> V8Helper::ConvertToV8Array(v8::Isolate* isolate, LynxArray* array) {
        int length = array->Size();
        v8::Local<v8::Array> v8_array = v8::Array::New(isolate, length);
        for (int i = 0; i < length; ++i) {
            v8::Local<v8::Value> v8_arg = ConvertToV8Value(isolate, array->Get(i));
            v8_array->Set(i, v8_arg);
        }
        return v8_array;
    }

    v8::Local<v8::Object> V8Helper::ConvertToV8Object(v8::Isolate* isolate, LynxMap* object) {
        v8::Local<v8::Object> v8_obj = v8::Object::New(isolate);
        for (int i = 0; i < object->Size(); ++i) {
            std::string name = object->GetName(i);
            v8_obj->Set(ConvertToV8String(isolate, name),
                        ConvertToV8Value(isolate, object->GetProperty(name)));
        }
        return v8_obj;
    }

    v8::Local<v8::Object> V8Helper::ConvertToV8Object(v8::Isolate* isolate, LynxObject* object) {
        if (object->object_wrap() != NULL) {
            return static_cast<V8ObjectWrap*>(object->object_wrap())->handle();
        }
        Global* global = object->context()->global();
        V8Context* context = static_cast<V8Context*>(global->context());
        auto prototype_builder = static_cast<V8PrototypeBuilder*>(object->class_template()->prototype_builder());
        v8::Local<v8::Object> v8_object = prototype_builder->MakeObject(context->GetContext());
        V8ObjectWrap::Wrap(context, object, v8_object);
        return v8_object;
    }

    std::string V8Helper::ConvertToString(const v8::Local<v8::String>& s) {
        if (s.IsEmpty()) {
            return std::string("");
        }
        if (s->IsNull()) {
            return std::string("Null");
        } else if (!(s->IsString() || s->IsStringObject())) {
            return std::string("value from v8 is not a string or string object");
        } else {
            v8::String::Utf8Value str(s);
            return std::string(*str);
        }
    }

    v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, const std::string& s) {
        v8::Local<v8::String> str =
            v8::String::NewFromUtf8(isolate, s.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
        return str;
    }

    v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, const char* s) {
        v8::Local<v8::String> str =
            v8::String::NewFromUtf8(isolate, s, v8::NewStringType::kNormal).ToLocalChecked();
        return str;
    }

    v8::Local<v8::String> V8Helper::JsonStringify(v8::Isolate* isolate, const v8::Local<v8::Value>& arg) {
        auto context = isolate->GetCurrentContext();
        // Get the global object.
        // Same as using 'global' in Node
        v8::Local<v8::Object> global = v8::Isolate::GetCurrent()->GetCurrentContext()->Global();

        // Get JSON
        // Same as using 'global.JSON'
        v8::Local<v8::Object> JSON = v8::Local<v8::Object>::Cast(
                                         global->Get(context, ConvertToV8String(isolate, "JSON")).ToLocalChecked());

        // Get stringify
        // Same as using 'global.JSON.stringify'
        v8::Local<v8::Function> stringify = v8::Local<v8::Function>::Cast(
                                                JSON->Get(context, ConvertToV8String(isolate, "stringify")).ToLocalChecked());

        // Stringify the object
        // Same as using 'global.JSON.stringify.apply(global.JSON, [ obj ])
        v8::Local<v8::Value> args[] = { arg };

        auto maybeResult = stringify->Call(context, JSON, 1, args);
        if (!maybeResult.IsEmpty()) {
            v8::Local<v8::String> result = v8::Local<v8::String>::Cast(maybeResult.ToLocalChecked());
            return result;
        }

    }

}
#endif
