// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_COLOR_H_
#define LYNX_LAYOUT_CSS_COLOR_H_

#include <string>

namespace lynx {
class CSSColor {
 public:
  CSSColor() : r_(0), g_(0), b_(0), a_(1.0f) {}
  CSSColor(unsigned char r, unsigned char g, unsigned char b, float a)
      : r_(r), g_(g), b_(b), a_(a) {}
  static bool Parse(const std::string& color_str, CSSColor& color);

  unsigned int Cast() const;

  bool operator==(const CSSColor& other) const {
    return r_ == other.r_ && g_ == other.g_ && b_ == other.b_ && a_ == other.a_;
  }

  unsigned char r_;
  unsigned char g_;
  unsigned char b_;
  float a_;
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_CSS_COLOR_H_
