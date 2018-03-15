//
// Created by chicheng on 2018/3/6.
//

#ifndef LYNX_CANVAS_VIEW_H
#define LYNX_CANVAS_VIEW_H

#include "render/extended_view.h"

namespace lynx {
    class CanvasView : public ExtendedView {
    public:
        CanvasView(const char *tag_name,
                     RenderObjectType type,
                     uint64_t id,
                     RenderTreeHost *host);
        virtual ~CanvasView() {}
        base::ScopedPtr<jscore::LynxObject> getImageData(int x, int y, int w, int h);
        double line_width_;
        std::string fill_style_;
        std::string stroke_style_;
        std::string line_cap_;
        std::string text_align_;
        std::string line_join_;
        std::string font_;
        std::string global_composite_operation_;
    };

};


#endif //LYNX_CANVAS_VIEW_H
