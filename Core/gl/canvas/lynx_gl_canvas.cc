#include <math.h>
#include "gl/canvas/lynx_gl_canvas.h"

#include "base/print.h"

namespace canvas {

#define MakeBindingVertex(x, y, tx, ty, c) \
       GeometryBindingVertex(ToGLX(x), ToGLY(y), ToGLX(tx), ToGLY(ty), c) 


LxGLCanvas::LxGLCanvas(int width, int height)
    : LxCanvas(width, height, GL_RGBA),
      renderer_(new LxCanvasRenderer(width, height, GL_RGBA)) {}

LxGLCanvas::~LxGLCanvas() {}

float LxGLCanvas::ToGLX(float x) {
  return -1.0f + 2.0f * (x / width_);
}

float LxGLCanvas::ToGLY(float y) {
  return 1.0f - 2.0f * (y / height_);
}

void LxGLCanvas::DrawRectCoords(float left,
                                float top,
                                float right,
                                float bottom,
                                const LxPaint& paint) {

  GeometryBindingVertex vetices[6];
  vetices[0] = MakeBindingVertex(left, top, 0, 0, paint.GetColor());  
  vetices[1] = MakeBindingVertex(right, top, 0, 0, paint.GetColor());  
  vetices[2] = MakeBindingVertex(left, bottom, 0, 0, paint.GetColor());  
  vetices[3] = MakeBindingVertex(right, top, 0, 0, paint.GetColor());  
  vetices[4] = MakeBindingVertex(left, bottom, 0, 0, paint.GetColor());  
  vetices[5] = MakeBindingVertex(right, bottom, 0, 0, paint.GetColor());  
  renderer_->DrawVertices(&paint, vetices, 6);
}

void LxGLCanvas::DrawLine(float x0, float y0, float x1, float y1, const LxPaint& paint) {
  float line_width = paint.GetStrokeWidth();
  float delta_x = x1- x0;
  float delta_y = y1 - y0;
  float len = sqrtf(delta_x * delta_x + delta_y * delta_y);
  float t = delta_x;
  delta_x = delta_y * line_width / len;
  delta_y = t * line_width / len;

  GeometryBindingVertex vetices[6];
  vetices[0] = MakeBindingVertex(x0 - delta_x, y0 + delta_y, 0, 0, paint.GetColor());  
  vetices[1] = MakeBindingVertex(x0 + delta_x, y0 - delta_y, 0, 0, paint.GetColor());  
  vetices[2] = MakeBindingVertex(x1 - delta_x, y1 + delta_y, 0, 0, paint.GetColor());  
  vetices[3] = MakeBindingVertex(x0 + delta_x, y0 - delta_y, 0, 0, paint.GetColor());  
  vetices[4] = MakeBindingVertex(x1 - delta_x, y1 + delta_y, 0, 0, paint.GetColor());  
  vetices[5] = MakeBindingVertex(x1 + delta_x, y1 - delta_y, 0, 0, paint.GetColor());  
  renderer_->DrawVertices(&paint, vetices, 6); 
}

void LxGLCanvas::DrawLineJoin(LxPoint p1, LxPoint p2, LxPoint p3, const LxPaint& paint) {
  float lineWidth = paint.GetStrokeWidth(); 
  float nx = p3.x_ - p2.x_;
  float ny = p3.y_ - p2.y_;
  float nl = sqrtf(nx * nx + ny * ny);
  float nt = nx;
  nx = ny * lineWidth / nl;
  ny = nt * lineWidth / nl;

  LxPoint n11, n12;
  n11 = LxPoint(p2.x_ - nx, p2.y_ + ny);
  n12 = LxPoint(p2.x_ + nx, p2.y_ - ny);

  float nextAngle = atan2(p3.x_ - p2.x_, p3.y_ - p2.y_);
  float curAngle = atan2(p1.x_ - p2.x_, p1.y_ - p2.y_);

  float angleDiff = nextAngle - curAngle;
  if (angleDiff < 0) {
    angleDiff += M_PI * 2;
  }

  LxPoint joinP1, joinP2;
  if (angleDiff > M_PI) {
    joinP1 = n12;
    //joinP2 = miter22;
  } else {
    //joinP1 = miter21;
    joinP2 = n11;
  }
  
  GeometryBindingVertex vetices[3];
  vetices[0] = MakeBindingVertex(p2.x_, p2.y_, 0, 0, paint.GetColor());  
  vetices[1] = MakeBindingVertex(joinP1.x_, joinP1.y_, 0, 0, paint.GetColor());  
  vetices[2] = MakeBindingVertex(joinP2.x_, joinP2.y_, 0, 0, paint.GetColor());  
  renderer_->DrawVertices(&paint, vetices, 3); 
}

void LxGLCanvas::DrawPath(const LxPath& path, const LxPaint& paint) {
  //path.Stroke();
  for(int i = 0; i < path.path_stack().size(); ++i) {
    LxPath::SubPath* sub_path = path.path_stack().at(i); 
    LxPoint p1 = sub_path->points_[0]; 
    for(int j = 1; j < sub_path->points_.size(); ++j) {
      LxPoint p2 = sub_path->points_[j]; 
      DrawLine(p1.x_, p1.y_, p2.x_, p2.y_, paint);
      // if(j+1 < sub_path->points_.size()) {
      //   LxPoint p3 = sub_path->points_[j+1];
      //   DrawLineJoin(p1, p2, p3, paint);
      // }
      p1 = p2;
    }
  }
}

void LxGLCanvas::DrawArc(float x,
                         float y,
                         float radius,
                         float start_angle,
                         float sweep_angle,
                         const LxPaint& paint) {
  LxPath path;
  path.Arc(x, y, radius, start_angle, start_angle+sweep_angle, false);
  path.Stroke();
  DrawPath(path, paint);

//   int step_count = 100;
//   //step_count *= (sweep_angle / (2.0f * M_PI));
//   if (step_count < 1) {
//     step_count = 1;
//   }
//   float delta_angle = sweep_angle / step_count;

//   float dx = cosf(delta_angle);
//   float dy = sinf(delta_angle);

//   float offset_x = radius * cosf(start_angle);
//   float offset_y = radius * sinf(start_angle);

// //LOGE("lynx", "step_count: %d, %d", step_count, path.path_stack().at(0)->points_.size());
//   for (int step = 0; step <= step_count; ++step) {

//     float old_x = offset_x, old_y = offset_y;
//     offset_x = old_x * dx - old_y * dy;
//     offset_y = old_x * dy + old_y * dx;
//     //path.path_stack().at(0).points_[step]
//     DrawLine(x + old_x, y + old_y, x + offset_x, y + offset_y, paint);
//   }
}

void LxGLCanvas::Flush() {
  renderer_->Flush();
}
}  // namespace canvas
