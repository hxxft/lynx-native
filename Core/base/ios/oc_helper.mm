// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ios/oc_helper.h"
#include "base/ios/common.h"

namespace base {
    namespace ios {
        
        NSString* OCHelper::ConvertToOCString(const std::string& str) {
            return [NSString stringWithUTF8String:str.c_str()];
        }
        
        NSString* OCHelper::ConvertToOCString(jscore::LynxValue* value) {
            if (value->type_ == jscore::LynxValue::VALUE_STRING) {
                return [NSString stringWithUTF8String:value->data_.str];
            }
            return nil;
        }
        
        id OCHelper::ConvertToOCValue(jscore::LynxValue* value) {
            id obj = nil;
            switch (value->type_) {
                case jscore::LynxValue::VALUE_INT:
                    obj = [NSNumber numberWithInt:value->data_.i];
                    break;
                case jscore::LynxValue::VALUE_LONG:
                    obj = [NSNumber numberWithLong:value->data_.l];
                    break;
                case jscore::LynxValue::VALUE_BOOL:
                    obj = [NSNumber numberWithBool:value->data_.b];
                    break;
                case jscore::LynxValue::VALUE_FLOAT:
                    obj = [NSNumber numberWithFloat:value->data_.f];
                    break;
                case jscore::LynxValue::VALUE_DOUBLE:
                    obj = [NSNumber numberWithInt:value->data_.i];
                    break;
                case jscore::LynxValue::VALUE_STRING:
                    obj = [NSString stringWithUTF8String:value->data_.str];
                    break;
                case jscore::LynxValue::VALUE_LYNX_MAP:
                    obj = ConvertToOCMap(value->data_.lynx_map);
                    break;
                case jscore::LynxValue::VALUE_LYNX_ARRAY:
                    obj = ConvertToOCArray(value->data_.lynx_array);
                    break;
                default:
                    break;
            }
            return obj;
        }
        
        NSDictionary* OCHelper::ConvertToOCMap(jscore::LynxMap* lynx_object) {
            NSMutableDictionary *object = [[NSMutableDictionary alloc] init];
            for (int i = 0; i < lynx_object->Size(); ++i) {
                std::string name = lynx_object->GetName(i);
                [object setObject:ConvertToOCValue(lynx_object->GetProperty(name))
                           forKey:ConvertToOCString(name)];
            }
            return object;
        }
        
        NSArray* OCHelper::ConvertToOCArray(jscore::LynxArray* lynx_array) {
            NSMutableArray *array = [[NSMutableArray alloc] init];
            for (int i = 0; i < lynx_array->Size(); ++i) {
                [array addObject:ConvertToOCValue(lynx_array->Get(i))];
            }
            return array;
        }
        
        base::ScopedPtr<jscore::LynxValue> OCHelper::ConvertToLynxValue(id value) {
            base::ScopedPtr<jscore::LynxValue> lynx_value;
            if ([value isKindOfClass:[NSNumber class]]) {
                NSNumber *number = SAFE_CONVERT(value, NSNumber);
                char type = number.objCType[0];
                switch (type) {
                    case 'c': // bool | char
                        lynx_value = jscore::LynxValue::MakeBool(number.boolValue);
                        break;
                    case 'Q': // UnsignedInteger
                    case 'q': // long
                    case 'i': // int
                        lynx_value = jscore::LynxValue::MakeInt(number.intValue);
                        break;
                    case 'd': // double
                    case 'f': // float
                        lynx_value = jscore::LynxValue::MakeDouble(number.doubleValue);
                        break;
                    default:
                        break;
                }
            } else if ([value isKindOfClass:[NSString class]] || [value isKindOfClass:[NSMutableString class]]){
                NSString *str = SAFE_CONVERT(value, NSString);
                lynx_value = jscore::LynxValue::MakeString([str UTF8String]);
            } else if ([value isKindOfClass:[NSMutableDictionary class]] || [value isKindOfClass:[NSDictionary class]]) {
                lynx_value = ConvertToLynxMap(SAFE_CONVERT(value, NSDictionary));
            }
            return lynx_value;
        }

        base::ScopedPtr<jscore::LynxMap> OCHelper::ConvertToLynxMap(NSDictionary *dic) {
            if (!dic) return base::ScopedPtr<jscore::LynxMap>();
            base::ScopedPtr<jscore::LynxMap> lynx_object(lynx_new jscore::LynxMap());
            NSArray *key_array = [dic allKeys];
            for (NSUInteger i = 0; i < key_array.count; ++i) {
                NSString *key = SAFE_CONVERT([key_array objectAtIndex:i], NSString);
                id item = [dic objectForKey:key];
                lynx_object->Set([key UTF8String], ConvertToLynxValue(item).Release());
            }
            return lynx_object;
        }
        
        base::ScopedPtr<jscore::LynxArray> OCHelper::ConvertToLynxArray(NSArray *array) {
            if (!array) return base::ScopedPtr<jscore::LynxArray>();
            NSUInteger length = array.count;
            base::ScopedPtr<jscore::LynxArray> lynx_array(lynx_new jscore::LynxArray());
            for (NSUInteger i = 0; i < length; ++i) {
                id value = [array objectAtIndex:i];
                jscore::LynxValue* lynx_value = ConvertToLynxValue(value).Release();
                lynx_array->Push(lynx_value);
            }
            return lynx_array;
        }
        
    }
}
