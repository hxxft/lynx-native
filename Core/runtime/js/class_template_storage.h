// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_class_template_STORAGE_H_
#define LYNX_RUNTIME_JSC_JSC_class_template_STORAGE_H_

#include <string>
#include "base/scoped_ptr_map.h"
#include "base/debug/memory_debug.h"
#include "runtime/js/class_template.h"

namespace jscore {
    class ClassTemplate;
    class ClassTemplateStorage {
    public:

        ClassTemplateStorage() {}
        
        ~ClassTemplateStorage() {}
        
        void Store(const std::string& name, ClassTemplate *class_template) {
            class_template_[name] = class_template;
        }

        ClassTemplate* GetClassTemplate(const std::string &name) {
            ClassTemplate* class_template = 0;
            if (class_template_.find(name) != class_template_.end()) {
                class_template = class_template_[name];
            }
            return class_template;
        }

        ClassTemplate* GetEmptyClassTemplate(JSContext* context);
        
    private:

        base::ScopedPtrMap<std::string, ClassTemplate> class_template_;
        const std::string kEmptyClassTemplateName = "_EmptyClass_";

    };
}

#endif  // LYNX_RUNTIME_JSC_JSC_class_template_STORAGE_H_
