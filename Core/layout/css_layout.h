// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LAYOUT_CSS_LAYOUT_H_
#define LYNX_LAYOUT_CSS_LAYOUT_H_

#include <vector>

#include "base/size.h"

namespace lynx {

class LayoutObject;
class CSSStaticLayout {
 public:
    static base::Size Measure(LayoutObject* renderer, int width_descriptor, int height_descriptor);
    static void Layout(LayoutObject* renderer, int width, int height);

 private:
    static LayoutObject* GetRoot(LayoutObject* renderer);
    static base::Size MeasureInner(LayoutObject* renderer, int width, int width_mode,
                                   int height, int height_mode);
    static bool MeasureSpecially(LayoutObject* renderer, int width, int width_mode,
                                 int height, int height_mode);
    static base::Size MeasureRowOneLine(LayoutObject *renderer, int width, int width_mode,
                                        int height, int height_mode, int start, int end);
    static base::Size MeasureRowWrap(LayoutObject* renderer, int width, int width_mode,
                                     int height, int height_mode);
    static base::Size MeasureRow(LayoutObject* renderer, int width, int width_mode,
                                 int height, int height_mode);
    static base::Size MeasureColumnOneLine(LayoutObject* renderer, int width, int width_mode,
                                           int height, int height_mode, int start, int end);
    static base::Size MeasureColumnWrap(LayoutObject* renderer, int width, int width_mode,
                                        int height, int height_mode);
    static base::Size MeasureColumn(LayoutObject* renderer, int width, int width_mode,
                                    int height, int height_mode);
    static void MeasureAbsolute(LayoutObject* renderer, int width, int width_mode,
                                int height, int height_mode);
    static void MeasureFixed(LayoutObject* renderer);

    static void LayoutWhenDisplayNone(LayoutObject* renderer);
    static void LayoutRowWrap(LayoutObject* renderer, int width, int height);
    static void LayoutRow(LayoutObject* renderer, int width, int height);
    static void LayoutColumnWrap(LayoutObject* renderer, int width, int height);
    static void LayoutColumn(LayoutObject* renderer, int width, int height);
    static void LayoutAbsolute(LayoutObject* parentNode, LayoutObject* renderer,
                               int width, int height);
    static void LayoutFixed(LayoutObject* parentNode, LayoutObject* renderer);
    static void LayoutFixedOrAbsolute(LayoutObject* parent, LayoutObject* child,
                                      int width, int height);

    static int CalculateOffsetWithFlexContainerStyle(LayoutObject* parent, LayoutObject* child,
                                                     int width, int height);
};
}  // namespace lynx

#endif  // LYNX_LAYOUT_CSS_LAYOUT_H_
