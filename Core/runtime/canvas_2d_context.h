// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
#define LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_

#include "base/macros.h"
#include "runtime/element.h"

namespace jscore{
    class  Canvas2DContext : public LynxObject {
    public:
        Canvas2DContext(JSContext* context, lynx::RenderObject* render_object);
        virtual ~Canvas2DContext();
        lynx::RenderObject* render_object() {
            return render_object_;
        }

        void SetLineWidth(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetLineWidth();

        void SetFillStyle(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetFillStyle();

        void SetStrokeStyle(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetStrokeStyle();

        void SetLineCap(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetLineCap();

        void SetTextAlign(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetTextAlign();

        void SetLineJoin(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetLineJoin();

        void SetGlobalCompositeOperation(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetGlobalCompositeOperation();

        void SetFont(base::ScopedPtr<LynxValue> value);
        base::ScopedPtr<LynxValue> GetFont();

        base::ScopedPtr<LynxValue> Transform(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Arc(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Save(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Fill(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Stroke(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Render(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> StrokeRect(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> AppendUpdate(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Restore(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> BeginPath(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> ClosePath(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> MoveTo(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> LineTo(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Translate(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Scale(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> DrawImage(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> Rotate(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> FillRect(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> ClearRect(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> FillText(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> StrokeText(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> QuadraticCurveTo(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> BezierCurveTo(base::ScopedPtr<LynxArray> &array);
        base::ScopedPtr<LynxValue> GetImageData(base::ScopedPtr<LynxArray> &array);

    private:

        double ConvertToNum(LynxValue* value);
        static std::string ConvertToString(double value);

        lynx::RenderObject *render_object_;

        DISALLOW_COPY_AND_ASSIGN(Canvas2DContext);

    };
}



#endif //LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
