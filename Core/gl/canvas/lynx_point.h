#ifndef LYNX_GL_CANVAS_LYNX_POINT_H_
#define LYNX_GL_CANVAS_LYNX_POINT_H_

namespace canvas {
  struct LxPoint {
    float x_;
    float y_;

    LxPoint() : x_(0), y_(0){
    }

    LxPoint(float x, float y) : x_(x), y_(y){
    }

    void Set(float x, float y) {
      x_ = x;
      y_ = y;
    }

  };
}

#endif