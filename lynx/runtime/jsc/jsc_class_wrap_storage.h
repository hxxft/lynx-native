// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_CLASS_WRAP_STORAGE_H_
#define LYNX_RUNTIME_JSC_JSC_CLASS_WRAP_STORAGE_H_

#include <JavaScriptCore/JavaScript.h>
#include "base/debug/memory_debug.h"
#include "runtime/jsc/class_wrap.h"
#include <unordered_map>
#include <string>

#include "runtime/jsc/class_wrap.h"
#include "runtime/jsc/object_wrap.h"

namespace jscore {
    
    class JSCClassWrapStorage {
        public:
        
        JSCClassWrapStorage() {
            
        }
        
        ~JSCClassWrapStorage() {
            for (auto it = class_map.begin(); it != class_map.end(); ++it) {
                lynx_delete(it->second);
            }
            class_map.clear();
        }
        
        ClassWrap* CreateClassWrap(const std::string& name,
                                   JSObjectFinalizeCallback callback = ObjectWrap::FinalizeCallback) {
            if (class_map.find(name) == class_map.end()) {
                class_map[name] = lynx_new ClassWrap(name, callback);
            }
            return class_map[name];
        }

        ClassWrap* GetClassWrap(const std::string& name) {
            ClassWrap* class_wrap = 0;
            if (class_map.find(name) != class_map.end()) {
                class_wrap = class_map[name];
            }
            return class_wrap;
        }

        const ClassWrap* GetEmptyClassWrap() {
            ClassWrap *class_wrap = GetClassWrap(kEmptyClassWrapName);
            if (class_wrap == NULL) {
                class_wrap = CreateClassWrap(kEmptyClassWrapName);
            }
            return class_wrap;
        }
        
    private:
        // Scoped Ptr to manage heap
        std::unordered_map<std::string, ClassWrap*> class_map;
        const std::string kEmptyClassWrapName = "_EmptyClass_";

    };
}

#endif  // LYNX_RUNTIME_JSC_JSC_CLASS_WRAP_STORAGE_H_
