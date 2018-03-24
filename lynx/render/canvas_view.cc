//
// Created by chicheng on 2018/3/6.
//

#include "render/canvas_view.h"
#include "render/impl/render_object_impl.h"
#include "render/render_tree_host.h"

namespace lynx{
    CanvasView::CanvasView(const char *tag_name,
                               RenderObjectType type,
                               uint64_t id,
                               RenderTreeHost *host)
            : ExtendedView(tag_name,
                           type,
                           id,
                           host) {

    }

    base::ScopedPtr<jscore::LynxObject> CanvasView::getImageData(int x, int y, int w, int h) {
        if(impl_.Get() != NULL){
            return impl_->GetImagePixel(x, y, w, h);
        }
        return base::ScopedPtr<jscore::LynxObject>();
    }

}
