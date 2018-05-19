#ifndef LYNX_GL_CANVAS_LYNX_PATH_H_
#define LYNX_GL_CANVAS_LYNX_PATH_H_

#include <vector>
#include "gl/canvas/lynx_point.h"

#include "base/scoped_ptr.h"
#include "base/scoped_vector.h"

namespace canvas {

class LxPath {
 public:
  struct SubPath {
    std::vector<LxPoint> points_;
    bool closed_;

    SubPath() : points_(), closed_(false) {}
  };
  typedef base::ScopedVector<SubPath> SubPathStack;

  LxPath();
  ~LxPath();
  void MoveTo(float x, float y);
  void LineTo(float x, float y);
  void Arc(float cx,
           float cy,
           float radius,
           float start_angle,
           float end_angle,
           bool anti_clockwise);
  void ArcTo(float x1, float y1, float x2, float y2, float radius);
  void Clear();
  void Stroke();

  const SubPathStack& path_stack() const {
    return path_stack_;
  }
 private:
  void PushPoint(float x, float y);

  SubPathStack path_stack_;
  base::ScopedPtr<SubPath> current_sub_path_;
  LxPoint begin_position_;
  LxPoint current_position_;
};
}  // namespace canvas

#endif