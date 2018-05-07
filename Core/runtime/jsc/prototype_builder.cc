// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/jsc/prototype_builder.h"

#include "runtime/jsc/jsc_context.h"

namespace jscore {
    ClassTemplate::PrototypeBuilder* ClassTemplate::CreatePrototypeBuilder() {
        return lynx_new JSCPrototypeBuilder();
    }

    void JSCPrototypeBuilder::FinishCreation(ClassTemplate* class_template) {
        SetJSClassName(class_template->class_name());
        if (class_template->parent_class_template() != NULL) {
            SetParentClass(static_cast<JSCPrototypeBuilder*>(class_template
                    ->parent_class_template()->prototype_builder()));
        }

        auto& field_map = class_template->fields();
        for (auto& it : field_map) {
            Field field = it.second;
            JSObjectGetPropertyCallback get = field.get_callback;
            JSObjectSetPropertyCallback set = field.set_callback;
            if (field.get_callback == 0) get = 0;
            if (field.set_callback == 0) set = 0;
            SetJSStaticValue(it.first.c_str(), get, set, JSPropertyAttributes());
        }

        auto& method_map = class_template->methods();
        for (auto& it : method_map) {
            SetJSStaticFunction(it.first.c_str(), it.second, JSPropertyAttributes());
        }

        MakeClass();
        if (class_template->constructor_expose_enable()) {
            auto context = static_cast<JSCContext*>(class_template->context())->GetContext();
            JSObjectRef constructor =
                    JSObjectMakeConstructor(context,
                                            class_ref_,
                                            js_class_definition_.callAsConstructor);
            JSCHelper::SetValueProperty(context, JSContextGetGlobalObject(context),
                                        js_class_definition_.className, constructor,
                                        kJSPropertyAttributeDontDelete, 0);
        }
    }
}