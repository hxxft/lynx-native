#ifndef LYNX_GL_CANVAS_LYNX_CANVAS_H_
#define LYNX_GL_CANVAS_LYNX_CANVAS_H_

#include "gl/canvas/lynx_paint.h"
#include "gl/canvas/lynx_path.h"

namespace canvas {
class LxCanvas {
 public:
  
  LxCanvas(unsigned int width, unsigned height, int format)
      : width_(width), height_(height), format_(format) {}

  //virtual void DrawPoint(float x, float y, LxColor color) = 0;
  virtual void DrawLine(float x0,
                        float y0,
                        float x1,
                        float y1,
                        const LxPaint& paint) = 0;
  virtual void DrawPath(const LxPath& path, const LxPaint& paint) = 0;

  virtual void DrawRectCoords(float left, float top, float right,
                        float bottom, const LxPaint& paint) = 0;

  virtual void DrawArc(float x,
                       float y,
                       float radius,
                       float start_angle,
                       float sweep_angle,
                       const LxPaint& paint) = 0;

      // virtual int Save() = 0;

      // virtual void Restore() = 0;

      protected : unsigned int width_;
  unsigned int height_;
  int format_;
};
}  // namespace canvas

#endif