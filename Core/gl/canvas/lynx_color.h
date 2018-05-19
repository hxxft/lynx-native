#ifndef LYNX_GL_CANVAS_LYNX_COLOR_H_
#define LYNX_GL_CANVAS_LYNX_COLOR_H_

#include <stdint.h>

namespace canvas {

typedef uint32_t LxColor;

static inline LxColor LxColorSetARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

#define LxColorSetRGB(r, g, b)  LxColorSetARGB(0xFF, r, g, b)

#define LxColorGetA(color)      (((color) >> 24) & 0xFF)

#define LxColorGetR(color)      (((color) >> 16) & 0xFF)

#define LxColorGetG(color)      (((color) >>  8) & 0xFF)

#define LxColorGetB(color)      (((color) >>  0) & 0xFF)

#define LxColorGetAf(color) (1.0f * LxColorGetA(color) / 255.0f)

#define LxColorGetRf(color) (1.0f * LxColorGetR(color) / 255.0f)

#define LxColorGetGf(color) (1.0f * LxColorGetG(color) / 255.0f)

#define LxColorGetBf(color) (1.0f * LxColorGetB(color) / 255.0f)

static inline LxColor LxColorSetA(LxColor c, uint8_t a) {
    return (c & 0x00FFFFFF) | (a << 24);
}

// common colors

#define Lx_AlphaTRANSPARENT 0x00        //!< transparent SkAlpha value
#define Lx_AlphaOPAQUE      0xFF        //!< opaque SkAlpha value

#define Lx_ColorTRANSPARENT 0x00000000  //!< transparent SkColor value

#define Lx_ColorBLACK       0xFF000000  //!< black SkColor value
#define Lx_ColorDKGRAY      0xFF444444  //!< dark gray SkColor value
#define Lx_ColorGRAY        0xFF888888  //!< gray SkColor value
#define Lx_ColorLTGRAY      0xFFCCCCCC  //!< light gray SkColor value
#define Lx_ColorWHITE       0xFFFFFFFF  //!< white SkColor value

#define Lx_ColorRED         0xFFFF0000  //!< red SkColor value
#define Lx_ColorGREEN       0xFF00FF00  //!< green SkColor value
#define Lx_ColorBLUE        0xFF0000FF  //!< blue SkColor value
#define Lx_ColorYELLOW      0xFFFFFF00  //!< yellow SkColor value
#define Lx_ColorCYAN        0xFF00FFFF  //!< cyan SkColor value
#define Lx_ColorMAGENTA     0xFFFF00FF  //!< magenta SkColor value

}

#endif 