// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_LAYOUT_H_
#define LYNX_LAYOUT_CSS_LAYOUT_H_

#include <vector>

#include "base/size.h"

namespace lynx {

class LayoutObject;
class CSSStaticLayout {
 public:
    static base::Size Measure(LayoutObject* renderer, int width, int height);
    static void Layout(LayoutObject* renderer, int width, int height);

 private:
    static base::Size measureInner(LayoutObject* renderer,
                                    int width, int height);
    static bool measureSpecially(LayoutObject* renderer, int width, int height);
    static base::Size measureRowOneLine(LayoutObject* renderer, int width,
                                        int height, int start, int end);
    static base::Size measureRowWrap(LayoutObject* renderer,
                                    int width, int height);
    static base::Size measureRow(LayoutObject* renderer,
                                int width, int height);
    static base::Size measureColumnOneLine(LayoutObject* renderer,
                        int width, int height, int start, int end);
    static base::Size measureColumnWrap(LayoutObject* renderer,
                                        int width, int height);
    static base::Size measureColumn(LayoutObject* renderer,
                                    int width, int height);


    static void layoutRowWrap(LayoutObject* renderer, int width, int height);
    static void layoutRow(LayoutObject* renderer, int width, int height);
    static void layoutColumnWrap(LayoutObject* renderer, int width, int height);
    static void layoutColumn(LayoutObject* renderer, int width, int height);
    static void measureAbsolute(LayoutObject* renderer, int width, int height);
    static void measureFixed(LayoutObject* renderer);
    static void layoutAbsolute(LayoutObject* parent,
                            LayoutObject* renderer,  int width, int height);
    static void layoutFixed(LayoutObject* parent, LayoutObject* renderer);
    static void layoutFixedOrAbsolute(LayoutObject* parent,
                                    LayoutObject* renderer,
                                    double width, double height);
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_CSS_LAYOUT_H_
