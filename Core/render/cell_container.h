// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_CELLS_CONTAINER_H_
#define LYNX_RENDER_CELLS_CONTAINER_H_

#include <string>

#include "base/scoped_ptr_map.h"
#include "render/render_object.h"

namespace lynx {
    class CellsContainer {
    public:
        CellsContainer() {

        }

        ~CellsContainer() {

        }

        void Add(RenderObject* render_object) {
            cells_map_.add(reinterpret_cast<intptr_t>(render_object), render_object);
        }

        void Remove(RenderObject* render_object) {
            CellsMap::iterator iter = cells_map_.find(reinterpret_cast<intptr_t>(render_object));
            if(iter != cells_map_.end()) {
                cells_map_.erase(iter);
            }
        }
    private:
        typedef base::ScopedPtrMap<intptr_t, RenderObject> CellsMap;
        CellsMap cells_map_;
    };
}

#endif  // LYNX_RENDER_CELLS_CONTAINER_H_
