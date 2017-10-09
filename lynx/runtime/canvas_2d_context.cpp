//
// Created by chicheng on 2017/9/26.
//

#include <runtime/base/lynx_value.h>
#include <sstream>
#include "runtime/canvas_2d_context.h"
#include "runtime/js_context.h"
#include "runtime/runtime.h"
#include "layout/css_color.h"

namespace  jscore{

    Canvas2DContext::Canvas2DContext(JSContext *context, lynx::RenderObject *render_object):
            render_object_(render_object), context_(context)  {
        set_class_name("Canvas2DContext");

        RegisterMethodCallback("fillRect", &FillRectCallback);
        RegisterMethodCallback("strokeRect", &StrokeRectCallback);
        RegisterMethodCallback("clearRect", &ClearRectCallback);
        RegisterMethodCallback("fill", &FillCallback);
        RegisterMethodCallback("stroke", &StrokeCallback);
        RegisterMethodCallback("render", &RenderCallback);
        RegisterMethodCallback("save", &SaveCallback);
        RegisterMethodCallback("restore", &RestoreCallback);
        RegisterMethodCallback("beginPath", &BeginPathCallback);
        RegisterMethodCallback("closePath", &ClosePathCallback);
        RegisterMethodCallback("transform", &TransformCallback);
        RegisterMethodCallback("translate", &TranslateCallback);
        RegisterMethodCallback("scale", &ScaleCallback);
        RegisterMethodCallback("arc", &ArcCallback);
        RegisterMethodCallback("moveTo", &MoveToCallback);
        RegisterMethodCallback("lineTo", &LineToCallback);
        RegisterMethodCallback("drawImage", &DrawImageCallback);
        RegisterMethodCallback("fillText", &FillTextCallback);
        RegisterMethodCallback("rotate", &RotateCallback);


        RegisterAccessorCallback("lineWidth", 0, &SetLineWidthCallback);
        RegisterAccessorCallback("fillStyle", 0, &SetFillStyleCallback);
        RegisterAccessorCallback("strokeStyle", 0, &SetStrokeStyleCallback);
    }

    Canvas2DContext::~Canvas2DContext() {

    }
    base::ScopedPtr<LynxValue>
    Canvas2DContext::FillRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double left = ConvertNumToIntBySp(array->Get(0));
            double top = ConvertNumToIntBySp(array->Get(1));
            double width = ConvertNumToIntBySp(array->Get(2));
            double height = ConvertNumToIntBySp(array->Get(3));
            std::string format_str = "fr@"+ConvertToString(left)+","+
                    ConvertToString(top)+","+
                    ConvertToString(left+width)+","+
                    ConvertToString(top+height);
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::StrokeRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double left = ConvertNumToIntBySp(array->Get(0));
            double top = ConvertNumToIntBySp(array->Get(1));
            double width = ConvertNumToIntBySp(array->Get(2));
            double height = ConvertNumToIntBySp(array->Get(3));
            std::string format_str = "sr@"+ConvertToString(left)+","+
                                    ConvertToString(top)+","+
                                     ConvertToString(left+width)+","+
                                     ConvertToString(top+height);
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::ClearRectCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 3) {
            lynx::RenderObject *render_object = context->render_object();
            double left = ConvertNumToIntBySp(array->Get(0));
            double top = ConvertNumToIntBySp(array->Get(1));
            double width = ConvertNumToIntBySp(array->Get(2));
            double height = ConvertNumToIntBySp(array->Get(3));
            std::string format_str = "cr@"+ConvertToString(left)+","+
                                     ConvertToString(top)+","+
                                     ConvertToString(left+width)+","+
                                     ConvertToString(top+height);
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::StrokeCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "stroke");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::FillCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "fill");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::RenderCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("postRender", "render");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::SaveCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "save");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::RestoreCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "restore");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::BeginPathCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "bp");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::ClosePathCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        render_object->SetAttribute("renderCmd", "cp");
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::TransformCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 5) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "tf@"+ConvertNumToIntString(array->Get(0))+","+
                    ConvertNumToIntString(array->Get(1))+","+
                    ConvertNumToIntString(array->Get(2))+","+
                    ConvertNumToIntString(array->Get(3))+","+
                    ConvertNumToIntString(array->Get(4))+","+
                    ConvertNumToIntString(array->Get(5));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::TranslateCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "tl@"+ConvertToString(ConvertNumToIntBySp(array->Get(0)))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(1)));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }
    base::ScopedPtr<LynxValue>
    Canvas2DContext::ScaleCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "scale@"+ConvertNumToIntString(array->Get(0))+","+
                    ConvertNumToIntString(array->Get(1));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::ArcCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 5) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "arc@"+ConvertToString(ConvertNumToIntBySp(array->Get(0)))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(1)))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(2))) +","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(3))) +","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(4))) +","+
                                     JSCHelper::ConvertToString(array->Get(5));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::MoveToCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "mt@"+ConvertToString(ConvertNumToIntBySp(array->Get(0)))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(1)));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::LineToCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "lt@"+ConvertToString(ConvertNumToIntBySp(array->Get(0)))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(1)));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::FillTextCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 1) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "ft@"+JSCHelper::ConvertToString(array->Get(0))+","+
                    ConvertToString(ConvertNumToIntBySp(array->Get(1)))+","+ConvertToString(ConvertNumToIntBySp(array->Get(2)));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    base::ScopedPtr<LynxValue>
    Canvas2DContext::RotateCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() > 0) {
            lynx::RenderObject *render_object = context->render_object();
            std::string format_str = "rotate@"+ConvertNumToIntString(array->Get(0));
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }


    base::ScopedPtr<LynxValue>
    Canvas2DContext::DrawImageCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        if (array.Get() != NULL && array->Size() == 5) {
            lynx::RenderObject *render_object = context->render_object();
            double x = ConvertNumToIntBySp(array->Get(1));
            double y = ConvertNumToIntBySp(array->Get(2));
            double width = ConvertNumToIntBySp(array->Get(3));
            double height = ConvertNumToIntBySp(array->Get(4));

            std::string format_str = "di@"+JSCHelper::ConvertToString(array->Get(0))+","+
                                     ConvertToString(x)+","+
                                     ConvertToString(y) +","+
                                     ConvertToString(x+width) +","+
                                     ConvertToString(y+height);
            render_object->SetAttribute("renderCmd", format_str);
        }else if(array.Get() != NULL && array->Size() == 9){
            lynx::RenderObject *render_object = context->render_object();
            double x = ConvertNumToIntBySp(array->Get(5));
            double y = ConvertNumToIntBySp(array->Get(6));
            double width = ConvertNumToIntBySp(array->Get(7));
            double height = ConvertNumToIntBySp(array->Get(8));
            std::string format_str = "dis@"+JSCHelper::ConvertToString(array->Get(0))+","+
                                     ConvertNumToIntString(array->Get(1))+","+
                                     ConvertNumToIntString(array->Get(2)) +","+
                                     ConvertNumToIntString(array->Get(3)) +","+
                                     ConvertNumToIntString(array->Get(4))+","+
                                     ConvertToString(x)+","+
                                     ConvertToString(y) +","+
                                     ConvertToString(x+width) +","+
                                     ConvertToString(y+height);
            render_object->SetAttribute("renderCmd", format_str);
        }
        return base::ScopedPtr<LynxValue>(NULL);
    }

    void Canvas2DContext::SetLineWidthCallback(LynxObjectTemplate* object,
                                        base::ScopedPtr<jscore::LynxValue> value) {
        Canvas2DContext *context = static_cast<Canvas2DContext *>(object);
        lynx::RenderObject *render_object = context->render_object();
        std::string format_str = "lw@"+ConvertToString(ConvertNumToIntBySp(value.Get()));
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

    // 传入的数字进行乘以100，那边的解析就只需要使用Int
    std::string Canvas2DContext::ConvertNumToIntString(jscore::LynxValue *value) {
        std::string result = "";
        if (value == NULL) {
            return result;
        }
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT: {
                double number = value->data_.i * 100;
                std::stringstream stream;
                stream << (int)number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_DOUBLE: {
                double number = value->data_.d * 100;;
                std::stringstream stream;
                stream << (int)number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_FLOAT: {
                double number = value->data_.f * 100;;
                std::stringstream stream;
                stream << (int)number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_LONG: {
                double number = value->data_.l * 100;;
                std::stringstream stream;
                stream << (int)number;
                result = stream.str();
            }
                break;
            default:
                break;
        }
        return result;
    }

    // 传入的数字进行乘以100，那边的解析就只需要使用Int
    double Canvas2DContext::ConvertNumToIntBySp(jscore::LynxValue *value) {
        double result = 0;
        int density = (int) config::GlobalConfigData::GetInstance()->screen_density();
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
        return result;
    }


    std::string Canvas2DContext::ConvertToString(double value) {
        std::string result = "0";
        if (value == NULL) {
            return result;
        }
        std::stringstream stream;
        stream << (int)value;
        result = stream.str();
        return result;
    }
}