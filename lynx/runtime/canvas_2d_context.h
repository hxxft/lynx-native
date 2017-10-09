//
// Created by chicheng on 2017/9/26.
//

#ifndef LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
#define LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_

#include "runtime/element.h"

namespace jscore{
    class  Canvas2DContext : public LynxObjectTemplate {
    public:
        Canvas2DContext(JSContext* context, lynx::RenderObject* render_object);
        virtual ~Canvas2DContext();
        lynx::RenderObject* render_object() {
            return render_object_;
        }

        JSContext* context() {
            return context_;
        }
    private:
        static base::ScopedPtr<LynxValue>
        FillRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        StrokeRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        ClearRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        FillCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        StrokeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        RenderCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        SaveCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        RestoreCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        BeginPathCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        ClosePathCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        TransformCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        TranslateCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        ScaleCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        ArcCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        MoveToCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        LineToCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        FillTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        RotateCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        DrawImageCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static void SetLineWidthCallback(LynxObjectTemplate* object,
                                          base::ScopedPtr<jscore::LynxValue> value);

        static void SetFillStyleCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);

        static void SetStrokeStyleCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);

        static std::string ConvertNumToIntString(jscore::LynxValue* value);


        static double ConvertNumToIntBySp(jscore::LynxValue* value);

        static std::string ConvertToString(double value);

        lynx::RenderObject *render_object_;
        JSContext* context_;


        DISALLOW_COPY_AND_ASSIGN(Canvas2DContext);

    };
}



#endif //LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
