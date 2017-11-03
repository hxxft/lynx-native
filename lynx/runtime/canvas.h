// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_CANVAS_H_
#define LYNX_RUNTIME_CANVAS_H_


#include "runtime/element.h"
#include "runtime/canvas_2d_context.h"

namespace jscore {
    class Canvas : public Element {
    public:
        Canvas(JSContext* context, lynx::RenderObject* render_object);
        virtual ~Canvas();

    private:
        base::ScopedPtr<LynxValue>
        static GetContextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        Canvas2DContext* context_2d_;

    };
}

#endif //LYNX_RUNTIME_CANVAS_H_
