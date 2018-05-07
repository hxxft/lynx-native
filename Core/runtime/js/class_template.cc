// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"
#include "runtime/js/class_template_storage.h"

namespace jscore {

    ClassTemplate* ClassTemplate::Generate(JSContext* context, const std::string& name) {
        if (name.empty()) return NULL;
        ClassTemplateStorage* storage = context->class_template_storage();
        ClassTemplate* temp = storage->GetClassTemplate(name);
        if (temp == NULL) {
            temp = lynx_new ClassTemplate(context, name);
            storage->Store(name, temp);
        }
        return temp;
    }

}