//
// Created by chicheng on 2017/9/26.
//

#ifndef LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
#define LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_

#include "runtime/element.h"
#include "JavaScriptCore/JavaScript.h"

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
        static void SetLineWidthCallback(LynxObjectTemplate* object,
                                          base::ScopedPtr<jscore::LynxValue> value);

        static base::ScopedPtr<LynxValue> GetLineWidthCallback(LynxObjectTemplate* object);

        static void SetFillStyleCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);

        static base::ScopedPtr<LynxValue> GetFillStyleCallback(LynxObjectTemplate* object);

        static void SetStrokeStyleCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);

        static base::ScopedPtr<LynxValue> GetStrokeStyleCallback(LynxObjectTemplate* object);

        static void SetLineCapCallback(LynxObjectTemplate* object,
                                           base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetLineCapCallback(LynxObjectTemplate* object);

        static void SetTextAlignCallback(LynxObjectTemplate* object,
                                       base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetTextAlignCallback(LynxObjectTemplate* object);

        static void SetLineJoinCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetLineJoinCallback(LynxObjectTemplate* object);

        static void SetGlobalCompositeOperationCallback(LynxObjectTemplate* object,
                                         base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetGlobalCompositeOperationCallback(LynxObjectTemplate* object);

        static void SetFontCallback(LynxObjectTemplate* object,
                                                        base::ScopedPtr<jscore::LynxValue> value);
        static base::ScopedPtr<LynxValue> GetFontCallback(LynxObjectTemplate* object);

        static double ConvertToNum(jscore::LynxValue* value);
        static std::string ConvertToString(double value);

        static JSValueRef RawTransformCallback(JSContextRef ctx,
                                                           JSObjectRef function,
                                                           JSObjectRef thisObject,
                                                           size_t argumentCount,
                                                           const JSValueRef arguments[],
                                                           JSValueRef* exception);
        static JSValueRef RawSaveCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawRestoreCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawTranslateCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawScaleCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawDrawImageCallback(JSContextRef ctx,
                                           JSObjectRef function,
                                           JSObjectRef thisObject,
                                           size_t argumentCount,
                                           const JSValueRef arguments[],
                                           JSValueRef* exception);
        static JSValueRef RawRotateCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawFillRectCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawStrokeRectCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawClearRectCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawFillCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawStrokeCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawRenderCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawUpdateCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawBeginPathCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawClosePathCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawMoveToCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawLineToCallback(JSContextRef ctx,
                                               JSObjectRef function,
                                               JSObjectRef thisObject,
                                               size_t argumentCount,
                                               const JSValueRef arguments[],
                                               JSValueRef* exception);
        static JSValueRef RawArcCallback(JSContextRef ctx,
                                            JSObjectRef function,
                                            JSObjectRef thisObject,
                                            size_t argumentCount,
                                            const JSValueRef arguments[],
                                            JSValueRef* exception);
        static JSValueRef RawFillTextCallback(JSContextRef ctx,
                                         JSObjectRef function,
                                         JSObjectRef thisObject,
                                         size_t argumentCount,
                                         const JSValueRef arguments[],
                                         JSValueRef* exception);
        static JSValueRef RawStrokeTextCallback(JSContextRef ctx,
                                              JSObjectRef function,
                                              JSObjectRef thisObject,
                                              size_t argumentCount,
                                              const JSValueRef arguments[],
                                              JSValueRef* exception);
        static JSValueRef RawQuadraticCurveToCallback(JSContextRef ctx,
                                                JSObjectRef function,
                                                JSObjectRef thisObject,
                                                size_t argumentCount,
                                                const JSValueRef arguments[],
                                                JSValueRef* exception);
        static JSValueRef RawBezierCurveToCallback(JSContextRef ctx,
                                                      JSObjectRef function,
                                                      JSObjectRef thisObject,
                                                      size_t argumentCount,
                                                      const JSValueRef arguments[],
                                                      JSValueRef* exception);

        static JSValueRef RawGetImageDataCallback(JSContextRef ctx,
                                                   JSObjectRef function,
                                                   JSObjectRef thisObject,
                                                   size_t argumentCount,
                                                   const JSValueRef arguments[],
                                                   JSValueRef* exception);




        lynx::RenderObject *render_object_;
        JSContext* context_;

        DISALLOW_COPY_AND_ASSIGN(Canvas2DContext);


    };
}



#endif //LYNX_RUNTIME_CANVAS_2D_CONTEXT_H_
