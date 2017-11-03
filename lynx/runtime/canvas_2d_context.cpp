//
// Created by chicheng on 2017/9/26.
//

#include <runtime/base/lynx_value.h>
#include <sstream>
#include <runtime/jsc/object_wrap.h>
#include "runtime/canvas_2d_context.h"
#include "runtime/js_context.h"
#include "runtime/runtime.h"
#include "layout/css_color.h"
#include "runtime/jsc/objects/object_template.h"

namespace  jscore{

    Canvas2DContext::Canvas2DContext(JSContext *context, lynx::RenderObject *render_object):
            render_object_(render_object), context_(context)  {
        set_class_name("Canvas2DContext");

        RegisterAccessorCallback("lineWidth", 0, &SetLineWidthCallback);
        RegisterAccessorCallback("fillStyle", 0, &SetFillStyleCallback);
        RegisterAccessorCallback("strokeStyle", 0, &SetStrokeStyleCallback);

        RegisterRawMethodCallback("transform", &RawTransformCallback);
        RegisterRawMethodCallback("save", &RawSaveCallback);
        RegisterRawMethodCallback("fill", &RawFillCallback);
        RegisterRawMethodCallback("stroke", &RawStrokeCallback);
        RegisterRawMethodCallback("render", &RawRenderCallback);
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
    }

    Canvas2DContext::~Canvas2DContext() {

    }

    void Canvas2DContext::SetLineWidthCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        std::string format_str = "lw@"+ConvertNumToIntStringBySp(value.Get());
        render_object->SetAttribute("renderCmd", format_str);
    }

    void Canvas2DContext::SetFillStyleCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        std::string format_str = "fs@" + JSCHelper::ConvertToString(value.Get());
        render_object->SetAttribute("renderCmd", format_str);
    }

    void Canvas2DContext::SetStrokeStyleCallback(LynxObjectTemplate* object,
                                               base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        std::string format_str = "fs@" + JSCHelper::ConvertToString(value.Get());
        render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "tf@"+ConvertNumToIntString(ctx, arguments[0])+","+
                                     ConvertNumToIntString(ctx, arguments[1])+","+
                                     ConvertNumToIntString(ctx, arguments[2])+","+
                                     ConvertNumToIntString(ctx, arguments[3])+","+
                                     ConvertNumToIntString(ctx, arguments[4])+","+
                                     ConvertNumToIntString(ctx, arguments[5]);
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string boolStr =  JSValueToBoolean(ctx, arguments[5]) ? "true" : "false";
            std::string format_str = "arc@"+ConvertNumToIntString(ctx, arguments[0])+","+
                                     ConvertNumToIntString(ctx, arguments[1])+","+
                                     ConvertNumToIntString(ctx, arguments[2])+","+
                                     ConvertNumToIntString(ctx, arguments[3])+","+
                                     ConvertNumToIntString(ctx, arguments[4])+","+ boolStr;
            render_object->SetAttribute("renderCmd", format_str);
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
        render_object->SetAttribute("renderCmd", "save");
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
        render_object->SetAttribute("renderCmd", "fill");
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
        render_object->SetAttribute("renderCmd", "bp");
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
        render_object->SetAttribute("renderCmd", "cp");
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
        render_object->SetAttribute("renderCmd", "stroke");
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
        render_object->SetAttribute("postRender", "render");
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
        render_object->SetAttribute("renderCmd", "restore");
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
            std::string format_str = "tl@"+ConvertNumToIntStringBySp(ctx, arguments[0])+","+
                    ConvertNumToIntStringBySp(ctx, arguments[1]);
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "mt@"+ConvertNumToIntStringBySp(ctx, arguments[0])+","+
                    ConvertNumToIntStringBySp(ctx, arguments[1]);
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "ft@"+JSCHelper::ConvertToString(ctx, arguments[0])+","+
                    ConvertNumToIntStringBySp(ctx, arguments[1])+","+
                    ConvertNumToIntStringBySp(ctx, arguments[2]);
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "lt@"+ConvertNumToIntStringBySp(ctx, arguments[0])+","+
                    ConvertNumToIntStringBySp(ctx, arguments[1]);
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "scale@"+ConvertNumToIntString(ctx, arguments[0])+","+
                                     ConvertNumToIntString(ctx, arguments[1]);
            render_object->SetAttribute("renderCmd", format_str);
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
            double x = JSValueToNumber(ctx, arguments[1], NULL) * 100;
            double y = JSValueToNumber(ctx, arguments[2], NULL) * 100;
            double width = JSValueToNumber(ctx, arguments[3], NULL) * 100;
            double height = JSValueToNumber(ctx, arguments[4], NULL) * 100;

            std::string format_str = "di@"+JSCHelper::ConvertToString(ctx, arguments[0])+","+
                                     ConvertToString(SPToPixel(x))+","+
                                     ConvertToString(SPToPixel(y)) +","+
                                     ConvertToString(SPToPixel(x+width)) +","+
                                     ConvertToString(SPToPixel(y+height));
            render_object->SetAttribute("renderCmd", format_str);
        }else if(argumentCount == 9){
            lynx::RenderObject *render_object = context->render_object();
            double x = JSValueToNumber(ctx, arguments[5], NULL) * 100;
            double y = JSValueToNumber(ctx, arguments[6], NULL) * 100;
            double width = JSValueToNumber(ctx, arguments[7], NULL) * 100;
            double height = JSValueToNumber(ctx, arguments[8], NULL) * 100;
            std::string format_str = "dis@"+JSCHelper::ConvertToString(ctx, arguments[0])+","+
                    ConvertNumToIntString(ctx, arguments[1])+","+
                    ConvertNumToIntString(ctx, arguments[2]) +","+
                    ConvertNumToIntString(ctx, arguments[3]) +","+
                    ConvertNumToIntString(ctx, arguments[4])+","+
                                     ConvertToString(SPToPixel(x))+","+
                                     ConvertToString(SPToPixel(y)) +","+
                                     ConvertToString(SPToPixel(x+width)) +","+
                                     ConvertToString(SPToPixel(y+height));
            render_object->SetAttribute("renderCmd", format_str);
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
            std::string format_str = "rotate@"+ConvertNumToIntString(ctx, arguments[0]);
            render_object->SetAttribute("renderCmd", format_str);
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
            double x = JSValueToNumber(ctx, arguments[0], NULL) * 100;
            double y = JSValueToNumber(ctx, arguments[1], NULL) * 100;
            double width = JSValueToNumber(ctx, arguments[2], NULL) * 100;
            double height = JSValueToNumber(ctx, arguments[3], NULL) * 100;

            std::string format_str = "fr@"+ ConvertToString(SPToPixel(x))+","+
                                     ConvertToString(SPToPixel(y)) +","+
                                     ConvertToString(SPToPixel(x+width)) +","+
                                     ConvertToString(SPToPixel(y+height));
            render_object->SetAttribute("renderCmd", format_str);
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
            double x = JSValueToNumber(ctx, arguments[0], NULL) * 100;
            double y = JSValueToNumber(ctx, arguments[1], NULL) * 100;
            double width = JSValueToNumber(ctx, arguments[2], NULL) * 100;
            double height = JSValueToNumber(ctx, arguments[3], NULL) * 100;

            std::string format_str = "cr@"+ ConvertToString(SPToPixel(x))+","+
                                     ConvertToString(SPToPixel(y)) +","+
                                     ConvertToString(SPToPixel(x+width)) +","+
                                     ConvertToString(SPToPixel(y+height));
            render_object->SetAttribute("renderCmd", format_str);
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
            double x = JSValueToNumber(ctx, arguments[0], NULL) * 100;
            double y = JSValueToNumber(ctx, arguments[1], NULL) * 100;
            double width = JSValueToNumber(ctx, arguments[2], NULL) * 100;
            double height = JSValueToNumber(ctx, arguments[3], NULL) * 100;

            std::string format_str = "sr@"+ ConvertToString(SPToPixel(x))+","+
                                     ConvertToString(SPToPixel(y)) +","+
                                     ConvertToString(SPToPixel(x+width)) +","+
                                     ConvertToString(SPToPixel(y+height));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return JSValueMakeNull(ctx);
    }

    std::string Canvas2DContext::ConvertNumToIntString(JSContextRef ctx, JSValueRef value) {
        double number = JSValueToNumber(ctx, value, NULL) * 100;
        return ConvertToString(number);
    }

    std::string Canvas2DContext::ConvertNumToIntStringBySp(JSContextRef ctx, JSValueRef value) {
        double number = JSValueToNumber(ctx, value, NULL) * 100;
        return ConvertToString(SPToPixel(number));
    }


    std::string Canvas2DContext::ConvertNumToIntStringBySp(jscore::LynxValue *value) {
        double number = ConvertNumToIntBySp(value);
        return ConvertToString(number);
    }

    // 传入的数字进行乘以100，那边的解析就只需要使用Int, 这里直接转成sp
    double Canvas2DContext::ConvertNumToIntBySp(jscore::LynxValue *value) {
        double result = 0;
        if (value == NULL) {
            return result;
        }
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT: {
                result = value->data_.i * 100;
            }
                break;
            case LynxValue::Type::VALUE_DOUBLE: {
                result = value->data_.d * 100;
            }
                break;
            case LynxValue::Type::VALUE_FLOAT: {
                result = value->data_.f * 100;
            }
                break;
            case LynxValue::Type::VALUE_LONG: {
                result = value->data_.l * 100;
            }
                break;
            default:
                break;
        }
        return SPToPixel(result);
    }


    std::string Canvas2DContext::ConvertToString(double value) {
        std::string result = "0";
        std::stringstream stream;
        stream << (int)value;
        result = stream.str();
        return result;
    }

}