// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/js/class_template_storage.h"

#include "runtime/js/js_context.h"

namespace jscore {
    ClassTemplate* ClassTemplateStorage::GetEmptyClassTemplate(JSContext* context) {
        ClassTemplate *class_template = GetClassTemplate(kEmptyClassTemplateName);
        if (class_template == NULL) {
            class_template = ClassTemplate::Generate(context, kEmptyClassTemplateName);
            class_template->FinishCreation();
        }
        return class_template;
    }
}