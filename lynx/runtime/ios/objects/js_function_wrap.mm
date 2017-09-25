// Copyright 2017 The Lynx Authors. All rights reserved.

#include "js_function_wrap.h"
#include "element_object.h"
#include "common.h"

namespace jscore {
    JSFunctionWrap::JSFunctionWrap(JSValue* value):js_function_(value){
        
    }
    
    JSFunctionWrap::JSFunctionWrap(JSFunctionWrap& wrap):js_function_(wrap.js_function_) {
        
    }
    
    
    JSFunctionWrap::~JSFunctionWrap() {
        js_function_ = nil;
    }
    
    void JSFunctionWrap::Run() {
        [js_function_ callWithArguments:nil];
    }
    
    void JSFunctionWrap::Run(void* target, void* args) {
        if (args == 0) {
            [js_function_ callWithArguments:nil];
        }
        ElementObject* element = (__bridge ElementObject*) (target);
        NSMutableArray* args_array = CFBridgingRelease(args);
        // insert target element to event obj
        id event = [args_array objectAtIndex:0];
        if (event && [event isKindOfClass: [NSMutableDictionary class]]) {
            NSMutableDictionary *event_array = SAFE_CONVERT(event, NSMutableDictionary);
            [event_array setValue:element forKey:@"target"];
        }
        [js_function_ callWithArguments:args_array];
    }
}