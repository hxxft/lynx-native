// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/convert.h"

#include "base/android/android_jni.h"
#include "base/position.h"
#include "base/size.h"
#include "layout/css_style.h"

namespace base {
#define SIZE_CLASS_PATH "com/lynx/base/Size"
#define SIZE_CONSTRUCTOR_METHOD "<init>", "()V"
#define SIZE_WIDTH_FIELD "mWidth", "D"
#define SIZE_HEIGHT_FIELD "mHeight", "D"
jclass Convert::size_class_ = nullptr;
jmethodID Convert::size_constructor_ = 0;
jfieldID Convert::size_width_field_ = 0;
jfieldID Convert::size_height_field_ = 0;

Size Convert::SizeConvert(jobject size_obj) {
    Size size;

    JNIEnv* env = android::AttachCurrentThread();

    if (size_class_ == nullptr) {
        BindingJavaClass(env);
    }
    if (size_obj == 0) {
        size.width_ = 0;
        size.height_ = 0;
    } else {
        size.width_ = env->GetDoubleField(size_obj, size_width_field_);
        size.height_ = env->GetDoubleField(size_obj, size_height_field_);
        base::android::CheckException(env);
    }
    return size;
}

android::ScopedLocalJavaRef<jobject>  Convert::SizeConvert(const Size& size) {
    JNIEnv* env = android::AttachCurrentThread();

    if (size_class_ == nullptr) {
        BindingJavaClass(env);
    }

    jobject size_obj = env->NewObject(size_class_, size_constructor_);
    env->SetDoubleField(size_obj, size_width_field_, size.width_);
    env->SetDoubleField(size_obj, size_height_field_, size.height_);
    base::android::CheckException(env);
    return android::ScopedLocalJavaRef<jobject>(env, size_obj);
}

#define POSITION_CLASS_PATH "com/lynx/base/Position"
#define POSITION_CONSTRUCTOR_METHOD "<init>", "()V"
#define POSITION_LEFT_FIELD "mLeft", "I"
#define POSITION_TOP_FIELD "mTop", "I"
#define POSITION_RIGHT_FIELD "mRight", "I"
#define POSITION_BOTTOM_FIELD "mBottom", "I"
jclass Convert::position_class_ = nullptr;
jmethodID Convert::position_constructor_ = 0;
jfieldID Convert::position_left_field_ = 0;
jfieldID Convert::position_top_field_ = 0;
jfieldID Convert::position_right_field_ = 0;
jfieldID Convert::position_bottom_field_ = 0;

android::ScopedLocalJavaRef<jobject>  Convert::PositionConvert(
    const Position& position) {
    JNIEnv* env = android::AttachCurrentThread();

    if (position_class_ == nullptr) {
        BindingJavaClass(env);
    }

    jobject position_obj =
        env->NewObject(position_class_, position_constructor_);
    env->SetIntField(position_obj, position_left_field_, position.left_);
    env->SetIntField(position_obj, position_top_field_, position.top_);
    env->SetIntField(position_obj, position_right_field_, position.right_);
    env->SetIntField(position_obj, position_bottom_field_, position.bottom_);
    base::android::CheckException(env);
    return android::ScopedLocalJavaRef<jobject>(env, position_obj);
}


#define STYLE_CLASS_PATH "com/lynx/base/Style"
#define STYLE_CONSTRUCTOR_METHOD "<init>", "()V"
#define STYLE_BACKGROUBD_COLOR_FIELD "mBackgroundColor", "I"
#define STYLE_BORDER_WIDTH_FIELD "mBorderWidth", "D"
#define STYLE_BORDER_COLOR_FIELD "mBorderColor", "I"
#define STYLE_BORDER_RADIUS_FIELD "mBorderRadius", "D"
#define STYLE_OPACITY_FIELD "mOpacity", "D"
#define STYLE_FONT_COLOR_FIELD "mFontColor", "I"
#define STYLE_FONT_SIZE_FIELD "mFontSize", "I"
#define STYLE_FONT_WEIGHT_FIELD "mFontWeight", "I"
#define STYLE_TEXT_OVERFLOW_FIELD "mTextOverflow", "I"
#define STYLE_WHITE_SPACE_FIELD "mWhiteSpace", "I"
#define STYLE_TEXT_ALIGN_FIELD "mTextAlign", "I"
#define STYLE_FLEX_DIRECTION_FIELD "mFlexDirection", "I"
#define STYLE_DISPLAY_TYPE_FIELD "mDisplayType", "I"
#define STYLE_VISIBLE_FIELD "mVisible", "I"
#define WIDTH_FIELD "mWidth", "I"
#define HEIGHT_FIELD "mHeight", "I"
#define ZINDEX_FIELD "mZIndex", "I"
#define LINE_HEIGHT_FIELD "mLineHeight", "I"
#define PADDING_LEFT_FIELD "mPaddingLeft", "I"
#define PADDING_RIGHT_FIELD "mPaddingRight", "I"
#define PADDING_TOP_FIELD "mPaddingTop", "I"
#define PADDING_BOTTOM_FIELD "mPaddingBottom", "I"
#define POSITION_TYPE_FIELD "mPositionType", "I"
#define OBJECT_FIT_TYPE_FIELD "mObjectFit", "I"

jclass Convert::style_class_ = nullptr;
jmethodID Convert::style_constructor_ = 0;
jfieldID Convert::background_color_field_ = 0;
jfieldID Convert::border_width_field_ = 0;
jfieldID Convert::border_color_field_ = 0;
jfieldID Convert::border_radius_field_ = 0;
jfieldID Convert::opacity_field_ = 0;
jfieldID Convert::font_color_field_ = 0;
jfieldID Convert::font_size_field_ = 0;
jfieldID Convert::font_weight_field_ = 0;
jfieldID Convert::text_overflow_field_ = 0;
jfieldID Convert::white_space_field_ = 0;
jfieldID Convert::text_align_field_ = 0;
jfieldID Convert::flex_direction_field_ = 0;
jfieldID Convert::display_type_field_ = 0;
jfieldID Convert::visible_field_ = 0;
jfieldID Convert::width_field_ = 0;
jfieldID Convert::height_field_ = 0;
jfieldID Convert::zindex_field_ = 0;
jfieldID Convert::line_height_field_ = 0;
jfieldID Convert::padding_right_field_ = 0;
jfieldID Convert::padding_left_field_ = 0;
jfieldID Convert::padding_top_field_ = 0;
jfieldID Convert::padding_bottom_field_ = 0;
jfieldID Convert::position_type_field_ = 0;
jfieldID Convert::object_fit_field_ = 0;

android::ScopedLocalJavaRef<jobject>  Convert::StyleConvert(
    const lynx::CSSStyle& style) {

    JNIEnv* env = android::AttachCurrentThread();

    if (style_class_ == nullptr) {
        BindingJavaClass(env);
    }

    jobject style_obj = env->NewObject(style_class_, style_constructor_);

    env->SetIntField(style_obj, background_color_field_, style.background_color_.Cast());
    env->SetDoubleField(style_obj, border_width_field_, style.border_width_);
    env->SetIntField(style_obj, border_color_field_, style.border_color_.Cast());
    env->SetDoubleField(style_obj, border_radius_field_, style.border_radius_);
    env->SetDoubleField(style_obj, opacity_field_, style.opacity_);
    env->SetIntField(style_obj, font_color_field_, style.font_color_.Cast());
    env->SetIntField(style_obj, font_size_field_, style.font_size_);
    env->SetIntField(style_obj, font_weight_field_, style.font_weight_);
    env->SetIntField(style_obj, text_overflow_field_, style.text_overflow_);
    env->SetIntField(style_obj, white_space_field_, style.text_white_space_);
    env->SetIntField(style_obj, text_align_field_, style.text_align_);
    env->SetIntField(style_obj, flex_direction_field_, style.flex_direction_);
    env->SetIntField(style_obj, display_type_field_, style.css_display_type_);
    env->SetIntField(style_obj, visible_field_, style.visible_);
    env->SetIntField(style_obj, width_field_, style.width_);
    env->SetIntField(style_obj, height_field_, style.height_);
    env->SetIntField(style_obj, zindex_field_, style.zindex_);
    env->SetIntField(style_obj, line_height_field_, style.line_height_);
    env->SetIntField(style_obj, padding_right_field_, style.padding_right_);
    env->SetIntField(style_obj, padding_left_field_, style.padding_left_);
    env->SetIntField(style_obj, padding_top_field_, style.padding_top_);
    env->SetIntField(style_obj, padding_bottom_field_, style.padding_bottom_);
    env->SetIntField(style_obj, position_type_field_, style.css_position_type_);
    env->SetIntField(style_obj, object_fit_field_, style.css_object_fit_);
    base::android::CheckException(env);

    return android::ScopedLocalJavaRef<jobject>(env, style_obj);
}

void Convert::BindingJavaClass(JNIEnv* env) {
    if (size_class_ == nullptr) {
        size_class_ = (jclass)env->NewGlobalRef(env->FindClass(SIZE_CLASS_PATH));
        size_constructor_ = env->GetMethodID(size_class_, SIZE_CONSTRUCTOR_METHOD);
        size_width_field_ = env->GetFieldID(size_class_, SIZE_WIDTH_FIELD);
        size_height_field_ = env->GetFieldID(size_class_, SIZE_HEIGHT_FIELD);
    }

    if (position_class_ == nullptr) {
        position_class_ = (jclass)env->NewGlobalRef(env->FindClass(POSITION_CLASS_PATH));
        position_constructor_ = env->GetMethodID(position_class_, POSITION_CONSTRUCTOR_METHOD);
        position_left_field_ = env->GetFieldID(position_class_, POSITION_LEFT_FIELD);
        position_top_field_ = env->GetFieldID(position_class_, POSITION_TOP_FIELD);
        position_right_field_ = env->GetFieldID(position_class_, POSITION_RIGHT_FIELD);
        position_bottom_field_ = env->GetFieldID(position_class_, POSITION_BOTTOM_FIELD);
    }

    if (style_class_ == nullptr) {
        style_class_ = (jclass)env->NewGlobalRef(env->FindClass(STYLE_CLASS_PATH));
        style_constructor_ = env->GetMethodID(style_class_, STYLE_CONSTRUCTOR_METHOD);
        background_color_field_ = env->GetFieldID(style_class_, STYLE_BACKGROUBD_COLOR_FIELD);
        border_width_field_ = env->GetFieldID(style_class_, STYLE_BORDER_WIDTH_FIELD);
        border_color_field_ = env->GetFieldID(style_class_, STYLE_BORDER_COLOR_FIELD);
        border_radius_field_ = env->GetFieldID(style_class_, STYLE_BORDER_RADIUS_FIELD);
        opacity_field_ = env->GetFieldID(style_class_, STYLE_OPACITY_FIELD);
        font_color_field_ = env->GetFieldID(style_class_, STYLE_FONT_COLOR_FIELD);
        font_size_field_ = env->GetFieldID(style_class_, STYLE_FONT_SIZE_FIELD);
        font_weight_field_ = env->GetFieldID(style_class_, STYLE_FONT_WEIGHT_FIELD);
        text_overflow_field_ = env->GetFieldID(style_class_, STYLE_TEXT_OVERFLOW_FIELD);
        white_space_field_ = env->GetFieldID(style_class_, STYLE_WHITE_SPACE_FIELD);
        text_align_field_ = env->GetFieldID(style_class_, STYLE_TEXT_ALIGN_FIELD);
        flex_direction_field_ = env->GetFieldID(style_class_, STYLE_FLEX_DIRECTION_FIELD);
        display_type_field_ = env->GetFieldID(style_class_, STYLE_DISPLAY_TYPE_FIELD);
        visible_field_ = env->GetFieldID(style_class_, STYLE_VISIBLE_FIELD);
        width_field_ = env->GetFieldID(style_class_, WIDTH_FIELD);
        height_field_ = env->GetFieldID(style_class_, HEIGHT_FIELD);
        zindex_field_ = env->GetFieldID(style_class_, ZINDEX_FIELD);
        line_height_field_ = env->GetFieldID(style_class_, LINE_HEIGHT_FIELD);
        padding_left_field_ = env->GetFieldID(style_class_, PADDING_LEFT_FIELD);
        padding_right_field_ = env->GetFieldID(style_class_, PADDING_RIGHT_FIELD);
        padding_top_field_ = env->GetFieldID(style_class_, PADDING_TOP_FIELD);
        padding_bottom_field_ = env->GetFieldID(style_class_, PADDING_BOTTOM_FIELD);
        position_type_field_ = env->GetFieldID(style_class_, POSITION_TYPE_FIELD);
        object_fit_field_ = env->GetFieldID(style_class_, OBJECT_FIT_TYPE_FIELD);
    }
    base::android::CheckException(env);
}
}
