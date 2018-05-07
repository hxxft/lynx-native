// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/canvas.h"

#include "config/global_config_data.h"
#include "runtime/runtime.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/js_context.h"
#include "runtime/js/class_template.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)   \
        V(Canvas, GetContext)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Canvas)
        REGISTER_PARENT(Element)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Canvas::Canvas(JSContext* context, lynx::RenderObject* render_object)
            : Element(context, render_object), context_2d_(NULL){
        set_class_template(DEFAULT_CLASS_TEMPLATE(context));
    }

    Canvas::~Canvas() {

    }

    base::ScopedPtr<LynxValue> Canvas::GetContext(base::ScopedPtr<LynxArray>& array) {
        if(context_2d_ == NULL) {
            context_2d_ = lynx_new Canvas2DContext(context(), render_object());
        }

        context_2d_->ProtectJSObject();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeObject(context_2d_));
    }
}