// Copyright 2014 The Lynx Authors. All rights reserved.

#ifndef CONVERT_H_
#define CONVERT_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"

namespace base {
    class Size;
    class Position;
}

namespace lynx {
    class CSSStyle;
}


namespace base {

    class Convert {
    public:

        static Size SizeConvert(jobject size);
        static android::ScopedLocalJavaRef<jobject> SizeConvert(const Size& size);
        static android::ScopedLocalJavaRef<jobject> PositionConvert(const Position& position);
        static android::ScopedLocalJavaRef<jobject> StyleConvert(const lynx::CSSStyle& style);

        static void BindingJavaClass(JNIEnv* env);
    private:

        static jclass size_class_;
        static jmethodID size_constructor_;
        static jfieldID size_width_field_;
        static jfieldID size_height_field_;

        static jclass position_class_;
        static jmethodID position_constructor_;
        static jfieldID position_left_field_;
        static jfieldID position_top_field_;
        static jfieldID position_right_field_;
        static jfieldID position_bottom_field_;

        static jclass style_class_;
        static jmethodID style_constructor_;
        static jfieldID background_color_field_;
        static jfieldID border_width_field_;
        static jfieldID border_color_field_;
        static jfieldID border_radius_field_;
        static jfieldID opacity_field_;
        static jfieldID font_color_field_;
        static jfieldID font_size_field_;
        static jfieldID font_weight_field_;
        static jfieldID text_overflow_field_;
        static jfieldID white_space_field_;
        static jfieldID text_align_field_;
        static jfieldID flex_direction_field_;
        static jfieldID display_type_field_;
        static jfieldID visible_field_;
        static jfieldID width_field_;
        static jfieldID height_field_;
        static jfieldID zindex_field_;
        static jfieldID line_height_field_;
        static jfieldID padding_right_field_;
        static jfieldID padding_left_field_;
        static jfieldID padding_top_field_;
        static jfieldID padding_bottom_field_;
        static jfieldID position_type_field_;
        static jfieldID object_fit_field_;
        static jfieldID pointer_events_field_;
        static jfieldID background_image_field_;
        static jfieldID background_repeat_field_;
        static jfieldID background_width_field_;
        static jfieldID background_height_field_;
        static jfieldID background_position_x_field_;
        static jfieldID background_position_y_field_;

    };
}
#endif //CONVERT_H_
