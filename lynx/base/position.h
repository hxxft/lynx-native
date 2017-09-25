// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_POSITION_H_
#define LYNX_BASE_POSITION_H_

#include "base/vector2d.h"

namespace base {

class Position {
 public:
    Position():
        left_(0), top_(0), right_(0), bottom_(0) {}

    Position(int l, int t, int r, int b) :
        left_(l),
        top_(t),
        right_(r),
        bottom_(b) {
        }

    Position(Position& pos):
        left_(pos.left_),
        top_(pos.top_),
        right_(pos.right_),
        bottom_(pos.bottom_) {
        }

    ~Position() {}

    void Update(Vector2D& vec) {
        left_ += vec.x();
        top_ += vec.y();
        right_ += vec.x();
        bottom_ += vec.y();
    }

    Vector2D Offset() {
        return Vector2D(left_, top_);
    }

    bool Update(Position& position) {
        if (left_ == position.left_ &&
                top_ == position.top_ &&
                right_ == position.right_ &&
                bottom_ == position.bottom_) {
            return false;
        }

        left_ = position.left_;
        top_ = position.top_;
        right_ = position.right_;
        bottom_ = position.bottom_;
        return true;
    }

    bool Update(int left, int top, int right, int bottom) {
        if (left_ == left &&
                top_ == top &&
                right_ == right &&
                bottom_ == bottom) {
            return false;
        }

        left_ = left;
        top_ = top;
        right_ = right;
        bottom_ = bottom;
        return true;
    }

    inline int GetWidth() const {
        return right_ - left_;
    }

    inline int GetHeight() const {
        return bottom_ - top_;
    }

    inline bool IsEmpty() const {
        return !(left_ || top_ || right_ || bottom_);
    }


 public:
    int left_;
    int top_;
    int right_;
    int bottom_;
};

}  // namespace base

#endif  // LYNX_BASE_POSITION_H_
