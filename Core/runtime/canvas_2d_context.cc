// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/canvas_2d_context.h"

#include <sstream>
#include "render/canvas_view.h"
#include "layout/css_color.h"
#include "runtime/runtime.h"
#include "runtime/canvas_cmd.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/js_context.h"
#include "runtime/js/class_template.h"
#include "config/global_config_data.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)      \
        V(Canvas2DContext, Transform)       \
        V(Canvas2DContext, Arc)             \
        V(Canvas2DContext, Save)            \
        V(Canvas2DContext, Fill)            \
        V(Canvas2DContext, Stroke)          \
        V(Canvas2DContext, Render)          \
        V(Canvas2DContext, StrokeRect)      \
        V(Canvas2DContext, AppendUpdate)    \
        V(Canvas2DContext, Restore)         \
        V(Canvas2DContext, BeginPath)       \
        V(Canvas2DContext, ClosePath)       \
        V(Canvas2DContext, MoveTo)          \
        V(Canvas2DContext, LineTo)          \
        V(Canvas2DContext, Translate)       \
        V(Canvas2DContext, Scale)           \
        V(Canvas2DContext, DrawImage)       \
        V(Canvas2DContext, Rotate)          \
        V(Canvas2DContext, FillRect)        \
        V(Canvas2DContext, ClearRect)       \
        V(Canvas2DContext, FillText)        \
        V(Canvas2DContext, StrokeText)      \
        V(Canvas2DContext, QuadraticCurveTo)\
        V(Canvas2DContext, BezierCurveTo)   \
        V(Canvas2DContext, GetImageData)

    #define FOR_EACH_FIELD_GET_BINDING(V)   \
        V(Canvas2DContext, LineWidth)       \
        V(Canvas2DContext, FillStyle)       \
        V(Canvas2DContext, StrokeStyle)     \
        V(Canvas2DContext, LineCap)         \
        V(Canvas2DContext, TextAlign)       \
        V(Canvas2DContext, LineJoin)        \
        V(Canvas2DContext, Font)            \
        V(Canvas2DContext, GlobalCompositeOperation)

    #define FOR_EACH_FIELD_SET_BINDING(V)   \
        V(Canvas2DContext, LineWidth)       \
        V(Canvas2DContext, FillStyle)       \
        V(Canvas2DContext, StrokeStyle)     \
        V(Canvas2DContext, LineCap)         \
        V(Canvas2DContext, TextAlign)       \
        V(Canvas2DContext, LineJoin)        \
        V(Canvas2DContext, Font)            \
        V(Canvas2DContext, GlobalCompositeOperation)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)
    FOR_EACH_FIELD_GET_BINDING(DEFINE_GET_CALLBACK)
    FOR_EACH_FIELD_SET_BINDING(DEFINE_SET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Canvas2DContext)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
        FOR_EACH_FIELD_GET_BINDING(REGISTER_GET_CALLBACK)
        FOR_EACH_FIELD_SET_BINDING(REGISTER_SET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    #define SPToPixel(value) (value * \
    config::GlobalConfigData::GetInstance()->screen_density())

    Canvas2DContext::Canvas2DContext(JSContext *context, lynx::RenderObject* render_object)
            : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)),
              render_object_(render_object) {

    }

    Canvas2DContext::~Canvas2DContext() {

    }

    void Canvas2DContext::SetLineWidth(base::ScopedPtr<LynxValue> value) {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->line_width_ = ConvertToNum(value.Get());
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lw"));
        cmd->Set("value", LynxValue::MakeInt(SPToPixel(ConvertToNum(value.Get()))).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineWidth() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeInt(render_object->line_width_);
    }

    void Canvas2DContext::SetLineCap(base::ScopedPtr<LynxValue> value){
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->line_cap_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lc"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineCap() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->line_cap_);
    }

    void Canvas2DContext::SetTextAlign(base::ScopedPtr<LynxValue> value){
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (render_object_);
        render_object->text_align_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ta"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetTextAlign() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->text_align_);
    }

    void Canvas2DContext::SetLineJoin(base::ScopedPtr<LynxValue> value){
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->line_join_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lj"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineJoin() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->line_join_);
    }

    void Canvas2DContext::SetFont(base::ScopedPtr<LynxValue> value){
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->font_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("sf"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetFont() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->font_);
    }


    void Canvas2DContext::SetGlobalCompositeOperation(base::ScopedPtr<LynxValue> value){
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->global_composite_operation_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("gco"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetGlobalCompositeOperation() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->global_composite_operation_);
    }

    void Canvas2DContext::SetFillStyle(base::ScopedPtr<LynxValue> value) {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->fill_style_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fs"));
        lynx::CSSColor color;
        lynx::CSSColor::Parse(value->ToString(), color);
        cmd->Set("value", LynxValue::MakeInt(color.Cast()).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetFillStyle() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->fill_style_);
    }

    void Canvas2DContext::SetStrokeStyle(base::ScopedPtr<LynxValue> value) {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        render_object->stroke_style_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ss"));
        lynx::CSSColor color;
        lynx::CSSColor::Parse(value->ToString(), color);
        cmd->Set("value", LynxValue::MakeInt(color.Cast()).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetStrokeStyle() {
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(render_object_);
        return LynxValue::MakeString(render_object->stroke_style_);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Transform(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 5) {
            double m11 = ConvertToNum(array->Get(0));
            double m21 = ConvertToNum(array->Get(1));
            double m12 = ConvertToNum(array->Get(2));
            double m22 = ConvertToNum(array->Get(3));
            double dx = SPToPixel(ConvertToNum(array->Get(4))) ;
            double dy = SPToPixel(ConvertToNum(array->Get(5))) ;

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("tf"));
            cmd->Set("m11", LynxValue::MakeFloat(m11).Release());
            cmd->Set("m12", LynxValue::MakeFloat(m12).Release());
            cmd->Set("m21", LynxValue::MakeFloat(m21).Release());
            cmd->Set("m22", LynxValue::MakeFloat(m22).Release());
            cmd->Set("dx", LynxValue::MakeFloat(dx).Release());
            cmd->Set("dy", LynxValue::MakeFloat(dy).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Arc(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 5) {
            double x = SPToPixel(ConvertToNum(array->Get(0)));
            double y = SPToPixel(ConvertToNum(array->Get(1)));
            double r = SPToPixel(ConvertToNum(array->Get(2)));
            double sAngle = ConvertToNum(array->Get(3));
            double eAngle = ConvertToNum(array->Get(4));
            bool counterclockwise = array->Get(5)->data_.b;

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("arc"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("r", LynxValue::MakeFloat(r).Release());
            cmd->Set("sAngle", LynxValue::MakeFloat(sAngle).Release());
            cmd->Set("eAngle", LynxValue::MakeFloat(eAngle).Release());
            cmd->Set("counterclockwise", LynxValue::MakeBool(counterclockwise).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Save(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("save"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Fill(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fill"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Stroke(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("stroke"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Render(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("drawCmd"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::StrokeRect(base::ScopedPtr<LynxArray> &array) {

        if (array->Size() > 3) {
            double x = ConvertToNum(array->Get(0));
            double y = ConvertToNum(array->Get(1));
            double width = ConvertToNum(array->Get(2));
            double height = ConvertToNum(array->Get(3));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("sr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::AppendUpdate(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("appendCmd"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Restore(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("restore"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::BeginPath(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("bp"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::ClosePath(base::ScopedPtr<LynxArray> &array) {
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("cp"));
        render_object_->ReceiveCanvasRenderCmd(cmd);
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::MoveTo(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 1) {
            double x = SPToPixel(ConvertToNum(array->Get(0)));
            double y = SPToPixel(ConvertToNum(array->Get(1)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("mt"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::LineTo(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 1) {
            double x = SPToPixel(ConvertToNum(array->Get(0)));
            double y = SPToPixel(ConvertToNum(array->Get(1)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lt"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Translate(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 1) {
            double x = SPToPixel(ConvertToNum(array->Get(0)));
            double y = SPToPixel(ConvertToNum(array->Get(1)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("tl"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Scale(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 1) {
            double x = ConvertToNum(array->Get(0));
            double y = ConvertToNum(array->Get(1));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("scale"));
            cmd->Set("scalewidth", LynxValue::MakeFloat(x).Release());
            cmd->Set("scaleheight", LynxValue::MakeFloat(y).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::DrawImage(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() == 5) {
            double x = SPToPixel(ConvertToNum(array->Get(1)));
            double y = SPToPixel(ConvertToNum(array->Get(2)));
            double width = SPToPixel(ConvertToNum(array->Get(3)));
            double height = SPToPixel(ConvertToNum(array->Get(4)));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("di"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("width", LynxValue::MakeFloat(width).Release());
            cmd->Set("height", LynxValue::MakeFloat(height).Release());
            cmd->Set("img", array->Release(0).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);

        }else if(array->Size() == 9){
            double sx = ConvertToNum(array->Get(5));
            double sy = ConvertToNum(array->Get(6));
            double swidth = ConvertToNum(array->Get(7));
            double sheight = ConvertToNum(array->Get(8));
            double x = SPToPixel(ConvertToNum(array->Get(5)));
            double y = SPToPixel(ConvertToNum(array->Get(6)));
            double width = SPToPixel(ConvertToNum(array->Get(7)));
            double height = SPToPixel(ConvertToNum(array->Get(8)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("di"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("width", LynxValue::MakeFloat(width).Release());
            cmd->Set("height", LynxValue::MakeFloat(height).Release());
            cmd->Set("sx", LynxValue::MakeFloat(sx).Release());
            cmd->Set("sy", LynxValue::MakeFloat(sy).Release());
            cmd->Set("swidth", LynxValue::MakeFloat(swidth).Release());
            cmd->Set("sheight", LynxValue::MakeFloat(sheight).Release());
            cmd->Set("img", array->Release(0).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::Rotate(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 0) {
            double angle = ConvertToNum(array->Get(0));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("rotate"));
            render_object_->ReceiveCanvasRenderCmd(cmd);
            cmd->Set("angle", LynxValue::MakeFloat(angle).Release());
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::FillRect(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 3) {
            double x = ConvertToNum(array->Get(0));
            double y = ConvertToNum(array->Get(1));
            double width = ConvertToNum(array->Get(2));
            double height = ConvertToNum(array->Get(3));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::ClearRect(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 3) {
            double x = ConvertToNum(array->Get(0));
            double y = ConvertToNum(array->Get(1));
            double width = ConvertToNum(array->Get(2));
            double height = ConvertToNum(array->Get(3));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("cr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::FillText(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 2) {
            double x = SPToPixel(ConvertToNum(array->Get(1)));
            double y = SPToPixel(ConvertToNum(array->Get(2)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ft"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("text", array->Release(0).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::StrokeText(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() > 2) {
            double x = SPToPixel(ConvertToNum(array->Get(1)));
            double y = SPToPixel(ConvertToNum(array->Get(2)));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("st"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("text", array->Release(0).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::QuadraticCurveTo(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() == 4) {
            double cpx = SPToPixel(ConvertToNum(array->Get(0)));
            double cpy = SPToPixel(ConvertToNum(array->Get(1)));
            double x = SPToPixel(ConvertToNum(array->Get(2)));
            double y = SPToPixel(ConvertToNum(array->Get(3)));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("qct"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("cpx", LynxValue::MakeFloat(cpx).Release());
            cmd->Set("cpy", LynxValue::MakeFloat(cpy).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::BezierCurveTo(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() == 6) {
            double cp1x = SPToPixel(ConvertToNum(array->Get(0)));
            double cp1y = SPToPixel(ConvertToNum(array->Get(1)));
            double cp2x = SPToPixel(ConvertToNum(array->Get(2)));
            double cp2y = SPToPixel(ConvertToNum(array->Get(3)));
            double x = SPToPixel(ConvertToNum(array->Get(4)));
            double y = SPToPixel(ConvertToNum(array->Get(5)));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("bct"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("cp1x", LynxValue::MakeFloat(cp1x).Release());
            cmd->Set("cp1y", LynxValue::MakeFloat(cp1y).Release());
            cmd->Set("cp2x", LynxValue::MakeFloat(cp2x).Release());
            cmd->Set("cp2y", LynxValue::MakeFloat(cp2y).Release());
            render_object_->ReceiveCanvasRenderCmd(cmd);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetImageData(base::ScopedPtr<LynxArray> &array) {
        double x = ConvertToNum(array->Get(0));
        double y = ConvertToNum(array->Get(1));
        double width = ConvertToNum(array->Get(2));
        double height = ConvertToNum(array->Get(3));
        return static_cast<lynx::CanvasView *>(render_object_)->GetImageData(x, y, width, height);
    }

    double Canvas2DContext::ConvertToNum(LynxValue *value) {
        double result = 0;
        if (value == NULL) {
            return result;
        }
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT: {
                result = value->data_.i;
            }
                break;
            case LynxValue::Type::VALUE_DOUBLE: {
                result = value->data_.d;
            }
                break;
            case LynxValue::Type::VALUE_FLOAT: {
                result = value->data_.f;
            }
                break;
            case LynxValue::Type::VALUE_LONG: {
                result = value->data_.l;
            }
                break;
            default:
                break;
        }
        return result;
    }


    std::string Canvas2DContext::ConvertToString(double value) {
        std::string result = "0";
        std::stringstream stream;
        stream << (int)value;
        result = stream.str();
        return result;
    }

}