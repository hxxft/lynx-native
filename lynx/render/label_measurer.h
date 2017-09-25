// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_LABEL_MEASURER_H_
#define LYNX_RENDER_LABEL_MEASURER_H_

#include <string>

#if OS_ANDROID
#include "jni.h"
#endif

namespace base {
class Size;
}

namespace lynx {
class CSSStyle;
class RenderObject;
class LabelMeasurer {
 public:
    static base::Size MeasureLabelSize(
        RenderObject* render_object,
        const base::Size& size,
        const std::string& text);
    
    
    static base::Size MeasureLabelSizeAndSetTextLayout(
                        RenderObject* render_object,
                        const base::Size& size,
                        const std::string& text);

#if OS_ANDROID

    static bool RegisterJNIUtils(JNIEnv* env);
#endif
};

}  // namespace lynx

#endif  // LYNX_RENDER_LABEL_MEASURER_H_
