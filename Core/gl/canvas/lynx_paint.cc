#include "gl/canvas/lynx_paint.h"

namespace canvas {
LxPaint::LxPaint() {
  //fTextSize = SkPaintDefaults_TextSize;
  //fTextScaleX = SK_Scalar1;
  //fTextSkewX = 0;
  color_ = Lx_ColorBLACK;
  width_ = 0;
  //fMiterLimit = SkPaintDefaults_MiterLimit;

  // Zero all bitfields, then set some non-zero defaults.
  bitfields_uint_ = 0;
  //fBitfields.fFlags = SkPaintDefaults_Flags;
  bitfields_.cap_type_ = kDefault_Cap;
  bitfields_.join_type_ = kDefault_Join;
  //fBitfields.fTextAlign = kLeft_Align;
  //fBitfields.fStyle = kFill_Style;
  //fBitfields.fTextEncoding = kUTF8_TextEncoding;
  //fBitfields.fHinting = SkPaintDefaults_Hinting;
}

LxPaint::~LxPaint() {

}

 void LxPaint::SetStrokeWidth(float width) {
   width_ = width;
 }

void LxPaint::SetColor(LxColor color) {
  color_ = color;
}

void LxPaint::SetStrokeCap(Cap cap) {
  if((unsigned)cap < kCapCount) {
    bitfields_.cap_type_ = ((uint8_t)(cap));
  }
}

void LxPaint::SetStrokeJoin(Join join) {
  if((unsigned)join < kJoinCount) {
    bitfields_.join_type_ = ((uint8_t)(join));
  }
}
}