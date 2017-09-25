// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_SIZE_H_
#define LYNX_BASE_SIZE_H_

namespace base {

class Size {
 public:
    class Descriptor {
     public:
        static int Make(int size, int mode) {
            return (size & ~MODE_MASK) | (mode & MODE_MASK);
        }

        static int GetMode(int descriptor) {
            return (descriptor & MODE_MASK);
        }

        static int GetSize(int descriptor) {
            return (descriptor & ~MODE_MASK);
        }

     private:
        static const int MODE_SHIFT = 30;
        static const int MODE_MASK  = 0x3 << MODE_SHIFT;
     public:
        static const int EXACTLY     = 1 << MODE_SHIFT;
        static const int AT_MOST     = 2 << MODE_SHIFT;
    };

    Size() : width_(0), height_(0) {}
    Size(int width, int height) : width_(width), height_(height) {}

    inline bool IsEqual(Size& size) {
        return width_ == size.width_ && height_ == size.height_;
    }

    inline bool Update(int width, int height) {
        if (width == width_ && height == height_) {
            return false;
        }

        width_ = width;
        height_ = height;
        return true;
    }

    inline bool IsEmpty() {
        return !width_ && !height_;
    }

    int width_;
    int height_;
};
}  // namespace base

#endif  // LYNX_BASE_SIZE_H_
