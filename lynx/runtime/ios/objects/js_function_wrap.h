// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef JS_FUNCTION_WRAP_H_
#define JS_FUNCTION_WRAP_H_

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>

#include "callback.h"
#include "js_function.h"

namespace jscore {
    class JSFunctionWrap : public base::Clouse, public JSFunction{
    public:
        JSFunctionWrap(JSValue* value);
        JSFunctionWrap(JSFunctionWrap& wrap);
        ~JSFunctionWrap();
        
        virtual void Run();
        virtual void Run(void* target, void* args);
    private:
        JSValue* js_function_;
    };
}

#endif /* JS_FUNCTION_WRAP_H_ */
