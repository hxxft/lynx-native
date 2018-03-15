//
// Created by chicheng on 2017/9/26.
//

#include <runtime/base/lynx_value.h>
#include <sstream>
#include <runtime/jsc/object_wrap.h>
#include <runtime/jsc/jsc_helper.h>
#include <render/canvas_view.h>
#include "runtime/canvas_2d_context.h"
#include "runtime/js_context.h"
#include "runtime/runtime.h"
#include "layout/css_color.h"
#include "runtime/jsc/objects/object_template.h"
#include "runtime/canvas_cmd.h"

namespace  jscore{

    Canvas2DContext::Canvas2DContext(JSContext *context, lynx::RenderObject *render_object):
            render_object_(render_object), context_(context)  {
        set_class_name("Canvas2DContext");

        RegisterAccessorCallback("lineWidth", &GetLineWidthCallback, &SetLineWidthCallback);
        RegisterAccessorCallback("fillStyle", &GetFillStyleCallback, &SetFillStyleCallback);
        RegisterAccessorCallback("strokeStyle", &GetStrokeStyleCallback, &SetStrokeStyleCallback);
        RegisterAccessorCallback("lineCap", &GetLineCapCallback, &SetLineCapCallback);
        RegisterAccessorCallback("textAlign", &GetTextAlignCallback, &SetTextAlignCallback);
        RegisterAccessorCallback("lineJoin", &GetLineJoinCallback, &SetLineJoinCallback);
        RegisterAccessorCallback("font", &GetFontCallback, &SetFontCallback);
        RegisterAccessorCallback("globalCompositeOperation", &GetGlobalCompositeOperationCallback, &SetGlobalCompositeOperationCallback);

        RegisterRawMethodCallback("transform", &RawTransformCallback);
        RegisterRawMethodCallback("save", &RawSaveCallback);
        RegisterRawMethodCallback("fill", &RawFillCallback);
        RegisterRawMethodCallback("stroke", &RawStrokeCallback);
        RegisterRawMethodCallback("render", &RawRenderCallback);
        RegisterRawMethodCallback("appendUpdate", &RawUpdateCallback);
        RegisterRawMethodCallback("restore", &RawRestoreCallback);
        RegisterRawMethodCallback("beginPath", &RawBeginPathCallback);
        RegisterRawMethodCallback("closePath", &RawClosePathCallback);
        RegisterRawMethodCallback("translate", &RawTranslateCallback);
        RegisterRawMethodCallback("scale", &RawScaleCallback);
        RegisterRawMethodCallback("drawImage", &RawDrawImageCallback);
        RegisterRawMethodCallback("rotate", &RawRotateCallback);
        RegisterRawMethodCallback("fillRect", &RawFillRectCallback);
        RegisterRawMethodCallback("strokeRect", &RawStrokeRectCallback);
        RegisterRawMethodCallback("clearRect", &RawClearRectCallback);
        RegisterRawMethodCallback("moveTo", &RawMoveToCallback);
        RegisterRawMethodCallback("lineTo", &RawLineToCallback);
        RegisterRawMethodCallback("arc", &RawArcCallback);
        RegisterRawMethodCallback("fillText", &RawFillTextCallback);
        RegisterRawMethodCallback("strokeText", &RawStrokeTextCallback);
        RegisterRawMethodCallback("quadraticCurveTo", &RawQuadraticCurveToCallback);
        RegisterRawMethodCallback("bezierCurveTo", &RawBezierCurveToCallback);
        RegisterRawMethodCallback("getImageData", &RawGetImageDataCallback);


    }

    Canvas2DContext::~Canvas2DContext() {

    }



    void Canvas2DContext::SetLineWidthCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->line_width_ = ConvertToNum(value.Get());
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lw"));
        cmd->Set("value", LynxValue::MakeInt(SPToPixel(ConvertToNum(value.Get()))).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineWidthCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeInt(render_object->line_width_);
    }

    void Canvas2DContext::SetLineCapCallback(LynxObjectTemplate* object,
                                             base::ScopedPtr<jscore::LynxValue> value){
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->line_cap_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lc"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineCapCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->line_cap_);
    }

    void Canvas2DContext::SetTextAlignCallback(LynxObjectTemplate* object,
                                             base::ScopedPtr<jscore::LynxValue> value){
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->text_align_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ta"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetTextAlignCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->text_align_);
    }

    void Canvas2DContext::SetLineJoinCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value){
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->line_join_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lj"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetLineJoinCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->line_join_);
    }

    void Canvas2DContext::SetFontCallback(LynxObjectTemplate* object,
                                              base::ScopedPtr<jscore::LynxValue> value){
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->font_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("sf"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetFontCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->font_);
    }


    void Canvas2DContext::SetGlobalCompositeOperationCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value){
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->global_composite_operation_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("gco"));
        cmd->Set("value", value.Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::GetGlobalCompositeOperationCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->global_composite_operation_);
    }


    void Canvas2DContext::SetFillStyleCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->fill_style_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fs"));
        lynx::CSSColor color;
        lynx::CSSColor::Parse(JSCHelper::ConvertToString(value.Get()), color);
        cmd->Set("value", LynxValue::MakeInt(color.Cast()).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetFillStyleCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->fill_style_);
    }

    void Canvas2DContext::SetStrokeStyleCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (context->render_object());
        render_object->stroke_style_ = value.Get()->data_.str;
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ss"));
        lynx::CSSColor color;
        lynx::CSSColor::Parse(JSCHelper::ConvertToString(value.Get()), color);
        cmd->Set("value", LynxValue::MakeInt(color.Cast()).Release());
        render_object->ReceiveCanvasRenderCmd(cmd);
    }

    base::ScopedPtr<LynxValue> Canvas2DContext::GetStrokeStyleCallback(LynxObjectTemplate* object) {
        Canvas2DContext *canvas_context = static_cast<Canvas2DContext *>(object);
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *> (canvas_context->render_object());
        return LynxValue::MakeString(render_object->stroke_style_);
    }

    JSValueRef Canvas2DContext::RawTransformCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 5) {
            lynx::RenderObject *render_object = context->render_object();
            double m11 = JSValueToNumber(ctx, arguments[0], NULL) ;
            double m21 = JSValueToNumber(ctx, arguments[1], NULL) ;
            double m12 = JSValueToNumber(ctx, arguments[2], NULL) ;
            double m22 = JSValueToNumber(ctx, arguments[3], NULL) ;
            double dx = SPToPixel(JSValueToNumber(ctx, arguments[4], NULL)) ;
            double dy = SPToPixel(JSValueToNumber(ctx, arguments[5], NULL)) ;

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("tf"));
            cmd->Set("m11", LynxValue::MakeFloat(m11).Release());
            cmd->Set("m12", LynxValue::MakeFloat(m12).Release());
            cmd->Set("m21", LynxValue::MakeFloat(m21).Release());
            cmd->Set("m22", LynxValue::MakeFloat(m22).Release());
            cmd->Set("dx", LynxValue::MakeFloat(dx).Release());
            cmd->Set("dy", LynxValue::MakeFloat(dy).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawArcCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 5) {
            lynx::RenderObject *render_object = context->render_object();
            double x = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL)) ;
            double y = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL)) ;
            double r = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL)) ;
            double sAngle = JSValueToNumber(ctx, arguments[3], NULL) ;
            double eAngle = JSValueToNumber(ctx, arguments[4], NULL) ;
            bool counterclockwise = JSValueToBoolean(ctx, arguments[5]) ;

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("arc"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("r", LynxValue::MakeFloat(r).Release());
            cmd->Set("sAngle", LynxValue::MakeFloat(sAngle).Release());
            cmd->Set("eAngle", LynxValue::MakeFloat(eAngle).Release());
            cmd->Set("counterclockwise", LynxValue::MakeBool(counterclockwise).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawSaveCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("save"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawFillCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fill"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawBeginPathCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("bp"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawClosePathCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("cp"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawStrokeCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("stroke"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawRenderCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("drawCmd"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }

    JSValueRef Canvas2DContext::RawUpdateCallback(JSContextRef ctx,
                                                  JSObjectRef function,
                                                  JSObjectRef thisObject,
                                                  size_t argumentCount,
                                                  const JSValueRef arguments[],
                                                  JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("appendCmd"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }


    JSValueRef Canvas2DContext::RawRestoreCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::RenderObject *render_object = context->render_object();
        base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("restore"));
        render_object->ReceiveCanvasRenderCmd(cmd);
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawTranslateCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 1) {
            lynx::RenderObject *render_object = context->render_object();
            double x = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("tl"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
            return JSValueMakeNull(ctx);

        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawMoveToCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 1) {
            lynx::RenderObject *render_object = context->render_object();
            double x = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("mt"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawStrokeTextCallback(JSContextRef ctx,
                                                  JSObjectRef function,
                                                  JSObjectRef thisObject,
                                                  size_t argumentCount,
                                                  const JSValueRef arguments[],
                                                  JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 2) {
            lynx::RenderObject *render_object = context->render_object();

            double x = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("st"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("text", LynxValue::MakeString(JSCHelper::ConvertToString(ctx, arguments[0])).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }

    JSValueRef Canvas2DContext::RawQuadraticCurveToCallback(JSContextRef ctx,
                                                      JSObjectRef function,
                                                      JSObjectRef thisObject,
                                                      size_t argumentCount,
                                                      const JSValueRef arguments[],
                                                      JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount == 4) {
            lynx::RenderObject *render_object = context->render_object();

            double cpx = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL));
            double cpy = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            double x = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[3], NULL));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("qct"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("cpx", LynxValue::MakeFloat(cpx).Release());
            cmd->Set("cpy", LynxValue::MakeFloat(cpy).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawBezierCurveToCallback(JSContextRef ctx,
                                                            JSObjectRef function,
                                                            JSObjectRef thisObject,
                                                            size_t argumentCount,
                                                            const JSValueRef arguments[],
                                                            JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount == 6) {
            lynx::RenderObject *render_object = context->render_object();

            double cp1x = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL));
            double cp1y = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            double cp2x = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL));
            double cp2y = SPToPixel(JSValueToNumber(ctx, arguments[3], NULL));
            double x = SPToPixel(JSValueToNumber(ctx, arguments[4], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[5], NULL));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("bct"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("cp1x", LynxValue::MakeFloat(cp1x).Release());
            cmd->Set("cp1y", LynxValue::MakeFloat(cp1y).Release());
            cmd->Set("cp2x", LynxValue::MakeFloat(cp2x).Release());
            cmd->Set("cp2y", LynxValue::MakeFloat(cp2y).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }



    JSValueRef Canvas2DContext::RawFillTextCallback(JSContextRef ctx,
                                                    JSObjectRef function,
                                                    JSObjectRef thisObject,
                                                    size_t argumentCount,
                                                    const JSValueRef arguments[],
                                                    JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 2) {
            lynx::RenderObject *render_object = context->render_object();

            double x = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("ft"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("text", LynxValue::MakeString(JSCHelper::ConvertToString(ctx, arguments[0])).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }


    JSValueRef Canvas2DContext::RawLineToCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 1) {
            lynx::RenderObject *render_object = context->render_object();
            double x = SPToPixel(JSValueToNumber(ctx, arguments[0], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("lt"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);

    }
    JSValueRef Canvas2DContext::RawScaleCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 1) {
            lynx::RenderObject *render_object = context->render_object();
            double x = JSValueToNumber(ctx, arguments[0], NULL);
            double y = JSValueToNumber(ctx, arguments[1], NULL);
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("scale"));
            cmd->Set("scalewidth", LynxValue::MakeFloat(x).Release());
            cmd->Set("scaleheight", LynxValue::MakeFloat(y).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawDrawImageCallback(JSContextRef ctx,
                                                 JSObjectRef function,
                                                 JSObjectRef thisObject,
                                                 size_t argumentCount,
                                                 const JSValueRef arguments[],
                                                 JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount == 5) {
            lynx::RenderObject *render_object = context->render_object();
            double x = SPToPixel(JSValueToNumber(ctx, arguments[1], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[2], NULL));
            double width = SPToPixel(JSValueToNumber(ctx, arguments[3], NULL));
            double height =SPToPixel(JSValueToNumber(ctx, arguments[4], NULL));

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("di"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("width", LynxValue::MakeFloat(width).Release());
            cmd->Set("height", LynxValue::MakeFloat(height).Release());
            cmd->Set("img", LynxValue::MakeString(JSCHelper::ConvertToString(ctx, arguments[0])).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);

        }else if(argumentCount == 9){
            lynx::RenderObject *render_object = context->render_object();
            double sx = JSValueToNumber(ctx, arguments[5], NULL);
            double sy = JSValueToNumber(ctx, arguments[6], NULL);
            double swidth = JSValueToNumber(ctx, arguments[7], NULL);
            double sheight =JSValueToNumber(ctx, arguments[8], NULL);
            double x = SPToPixel(JSValueToNumber(ctx, arguments[5], NULL));
            double y = SPToPixel(JSValueToNumber(ctx, arguments[6], NULL));
            double width = SPToPixel(JSValueToNumber(ctx, arguments[7], NULL));
            double height = SPToPixel(JSValueToNumber(ctx, arguments[8], NULL));
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("di"));
            cmd->Set("x", LynxValue::MakeFloat(x).Release());
            cmd->Set("y", LynxValue::MakeFloat(y).Release());
            cmd->Set("width", LynxValue::MakeFloat(width).Release());
            cmd->Set("height", LynxValue::MakeFloat(height).Release());
            cmd->Set("sx", LynxValue::MakeFloat(sx).Release());
            cmd->Set("sy", LynxValue::MakeFloat(sy).Release());
            cmd->Set("swidth", LynxValue::MakeFloat(swidth).Release());
            cmd->Set("sheight", LynxValue::MakeFloat(sheight).Release());
            cmd->Set("img", LynxValue::MakeString(JSCHelper::ConvertToString(ctx, arguments[0])).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawRotateCallback(JSContextRef ctx,
                                                 JSObjectRef function,
                                                 JSObjectRef thisObject,
                                                 size_t argumentCount,
                                                 const JSValueRef arguments[],
                                                 JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 0) {
            lynx::RenderObject *render_object = context->render_object();
            double angle = JSValueToNumber(ctx, arguments[0], NULL);
            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("rotate"));
            render_object->ReceiveCanvasRenderCmd(cmd);
            cmd->Set("angle", LynxValue::MakeFloat(angle).Release());
            return JSValueMakeNull(ctx);
        }
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawFillRectCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double x = JSValueToNumber(ctx, arguments[0], NULL);
            double y = JSValueToNumber(ctx, arguments[1], NULL);
            double width = JSValueToNumber(ctx, arguments[2], NULL);
            double height = JSValueToNumber(ctx, arguments[3], NULL);

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("fr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawClearRectCallback(JSContextRef ctx,
                                                    JSObjectRef function,
                                                    JSObjectRef thisObject,
                                                    size_t argumentCount,
                                                    const JSValueRef arguments[],
                                                    JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double x = JSValueToNumber(ctx, arguments[0], NULL);
            double y = JSValueToNumber(ctx, arguments[1], NULL);
            double width = JSValueToNumber(ctx, arguments[2], NULL);
            double height = JSValueToNumber(ctx, arguments[3], NULL);

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("cr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);
    }
    JSValueRef Canvas2DContext::RawStrokeRectCallback(JSContextRef ctx,
                                                     JSObjectRef function,
                                                     JSObjectRef thisObject,
                                                     size_t argumentCount,
                                                     const JSValueRef arguments[],
                                                     JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());

        if (argumentCount > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double x = JSValueToNumber(ctx, arguments[0], NULL);
            double y = JSValueToNumber(ctx, arguments[1], NULL);
            double width = JSValueToNumber(ctx, arguments[2], NULL);
            double height = JSValueToNumber(ctx, arguments[3], NULL);

            base::ScopedPtr<base::CanvasRenderCommand> cmd(lynx_new base::CanvasRenderCommand("sr"));
            cmd->Set("x", LynxValue::MakeFloat(SPToPixel(x)).Release());
            cmd->Set("y", LynxValue::MakeFloat(SPToPixel(y)).Release());
            cmd->Set("width", LynxValue::MakeFloat(SPToPixel(width)).Release());
            cmd->Set("height", LynxValue::MakeFloat(SPToPixel(height)).Release());
            render_object->ReceiveCanvasRenderCmd(cmd);
        }
        return JSValueMakeNull(ctx);
    }

    JSValueRef Canvas2DContext::RawGetImageDataCallback(JSContextRef ctx, JSObjectRef function,
                                                        JSObjectRef thisObject,
                                                        size_t argumentCount,
                                                        JSValueRef const* arguments,
                                                        JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);
        Canvas2DContext *context = static_cast<Canvas2DContext *>(obj->target());
        lynx::CanvasView *render_object = static_cast<lynx::CanvasView *>(context->render_object());
        double x = JSValueToNumber(ctx, arguments[0], NULL);
        double y = JSValueToNumber(ctx, arguments[1], NULL);
        double width = JSValueToNumber(ctx, arguments[2], NULL);
        double height = JSValueToNumber(ctx, arguments[3], NULL);
        base::ScopedPtr<jscore::LynxObject> ret = render_object->getImageData(x, y, width, height);
        return jscore::JSCHelper::ConvertToJSObject(ctx, ret.Get());
    }


    double Canvas2DContext::ConvertToNum(jscore::LynxValue *value) {
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