#ifndef LYNX_GL_CANVAS_LYNX_GL_CANVAS_H_
#define LYNX_GL_CANVAS_LYNX_GL_CANVAS_H_ 

#include "gl/canvas/lynx_canvas.h"
#include "gl/canvas/lynx_canvas_renderer.h"
#include "gl/canvas/lynx_paint.h"


namespace canvas {
class LxGLCanvas : public LxCanvas {
 public:
  LxGLCanvas(int width, int height);
  virtual ~LxGLCanvas();
  void DrawLine(float x0,
                        float y0,
                        float x1,
                        float y1,
                        const LxPaint& paint) override;
  void DrawRectCoords(float left,
                      float top,
                      float right,
                      float bottom,
                      const LxPaint& paint) override;
  void DrawArc(float x,
               float y,
               float radius,
               float start_angle,
               float sweep_angle,
               const LxPaint& paint) override;

  void DrawPath(const LxPath& path, const LxPaint& paint) override;

  void Flush();

 private:
  void DrawLineJoin(LxPoint p1, LxPoint p2, LxPoint p3, const LxPaint& paint);
  float ToGLX(float x);
  float ToGLY(float y);
  LxCanvasRenderer* renderer_;
};
}  // namespace canvas

#endif