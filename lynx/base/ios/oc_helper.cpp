// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ios/oc_helper.h"
#include "base/ios/common.h"

namespace base {
    namespace ios {
        
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
                case jscore::LynxValue::VALUE_LYNX_ARRAY:
                    obj = ConvertToOCArray(value->data_.lynx_array);
                    break;
                default:
                    break;
            }
            return obj;
        }
        
        NSMutableArray* OCHelper::ConvertToOCArray(jscore::LynxArray* lynx_array) {
            NSMutableArray *array = [[NSMutableArray alloc] init];
            for (int i = 0; i < lynx_array->Size(); ++i) {
                [array addObject:ConvertToOCValue(lynx_array->Get(i))];
            }
            return array;
        }
        
        jscore::LynxValue* OCHelper::ConvertToLynxValue(id value) {
            jscore::LynxValue* lynx_value = 0;
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
            } else if ([value isKindOfClass:[NSString class]]){
                NSString *str = SAFE_CONVERT(value, NSString);
                lynx_value = jscore::LynxValue::MakeString([str UTF8String]);
            } else if ([value isKindOfClass:[NSMutableDictionary class]]) {
                lynx_value = ConvertToLynxObject(SAFE_CONVERT(value, NSMutableDictionary));
            }
            return lynx_value;
        }
        
        jscore::LynxObject* OCHelper::ConvertToLynxObject(NSMutableDictionary *dic) {
            if (!dic) return NULL;
            jscore::LynxObject* lynx_object = lynx_new jscore::LynxObject();
            NSArray *key_array = [dic allKeys];
            for (NSUInteger i = 0; i < key_array.count; ++i) {
                NSString *key = SAFE_CONVERT([key_array objectAtIndex:i], NSString);
                id item = [dic objectForKey:key];
                lynx_object->Set([key UTF8String], ConvertToLynxValue(item));
            }
            return lynx_object;
        }
        
        jscore::LynxArray* OCHelper::ConvertToLynxArray(NSMutableArray *array) {
            if (!array) return NULL;
            NSUInteger length = array.count;
            jscore::LynxArray* lynx_array = lynx_new jscore::LynxArray();
            for (NSUInteger i = 0; i < length; ++i) {
                id value = [array objectAtIndex:i];
                jscore::LynxValue* lynx_value = ConvertToLynxValue(value);
                lynx_array->Push(lynx_value);
            }
            return lynx_array;
        }
        
    }
}