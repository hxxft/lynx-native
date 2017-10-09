// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_context.h"
#include "config/global_config_data.h"
#include "runtime/canvas.h"
#include "runtime/js_context.h"
#include "runtime/runtime.h"
#include "runtime/jsc/objects/object_template.h"

namespace jscore {
    Canvas::Canvas(JSContext* context, lynx::RenderObject* render_object)
            : Element(context, render_object),
              context_2d_(NULL){
        set_class_name("Canvas");


        RegisterMethodCallback("getContext", &GetContextCallback);
    }

    Canvas::~Canvas() {

    }

    base::ScopedPtr<LynxValue>
    Canvas::GetContextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas *canvas = static_cast<Canvas *>(object);
        if(canvas->context_2d_ == NULL) {
            canvas->context_2d_ = lynx_new Canvas2DContext(canvas->context(), canvas->render_object());
        }

        if (canvas->context_2d_->object_wrap() == NULL) {
            // Create JSObject immediately
            jscore::JSCHelper::ConvertToJSObject(static_cast<JSCContext*>(canvas->context())->GetContext(),
                                                 canvas->context_2d_);
        }
        canvas->context_2d_->object_wrap()->Protect();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeObjectTemplate(canvas->context_2d_));
    }
}