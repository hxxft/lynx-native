#ifndef GL_GL_CANVAS_LYNX_PAINT_H_
#define GL_GL_CANVAS_LYNX_PAINT_H_

#include "gl/canvas/lynx_color.h"

namespace canvas {
  
  class LxPaint {
    public:
    LxPaint();
    ~LxPaint();
     float GetStrokeWidth() const { return width_; }

     void SetStrokeWidth(float width);

     LxColor GetColor() const { return color_; }

      void SetColor(LxColor color);

    enum Cap {
        kButt_Cap,      //!< begin/end contours with no extension
        kRound_Cap,     //!< begin/end contours with a semi-circle extension
        kSquare_Cap,    //!< begin/end contours with a half square extension

        kCapCount,
        kDefault_Cap = kButt_Cap
    };

    enum Join {
        kMiter_Join,    //!< connect path segments with a sharp join
        kRound_Join,    //!< connect path segments with a round join
        kBevel_Join,    //!< connect path segments with a flat bevel join

        kJoinCount,
        kDefault_Join = kMiter_Join
    };

    Cap GetStrokeCap() const { return (Cap)bitfields_.cap_type_; }

    void SetStrokeCap(Cap cap);

    Join GetStrokeJoin() const { return (Join)bitfields_.join_type_; }

    void SetStrokeJoin(Join join);

    private:
     float width_;
     LxColor color_;

     union {
        struct {
            // all of these bitfields should add up to 32
            unsigned        flags_ : 16;
            unsigned        text_align_ : 2;
            unsigned        cap_type_ : 2;
            unsigned        join_type_ : 2;
            unsigned        style_ : 2;
            unsigned        text_encoding_ : 2;  // 3 values
            unsigned        hinting_ : 2;
            unsigned        filter_quality_ : 2;
            //unsigned      fFreeBits : 2;
        } bitfields_;
        uint32_t bitfields_uint_;
    };
     
  };
}

#endif