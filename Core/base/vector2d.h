// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_VECTOR2D_H_
#define LYNX_BASE_VECTOR2D_H_
namespace base {
class Vector2D {
 public:
    Vector2D() : x_(0), y_(0) {}
    Vector2D(int x, int y) : x_(x), y_(y) {}
    ~Vector2D() {}

    int x() { return x_; }
    int y() { return y_; }

    void Reset() {
        x_ = 0;
        y_ = 0;
    }

    void Offset(int x, int y) {
        x_ += x;
        y_ += y;
    }

 private:
    int x_;
    int y_;
};
}  // namespace base

#endif  // LYNX_BASE_VECTOR2D_H_
