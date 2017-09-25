// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_RENDER_FACTORY_H_
#define LYNX_RENDER_RENDER_FACTORY_H_

#include <stdint.h>
#include <string>
#include <base/print.h>

#include "render/body.h"
#include "render/image_view.h"
#include "render/label.h"
#include "render/view_stub.h"
#include "render/list_shadow.h"
#include "render/list_view.h"
#include "render/scroll_view.h"
#include "render/view.h"
#include "render/input.h"
#include "render/swiper_view.h"
#include "render/extended_view.h"
#include "render/text_area.h"
#include "runtime/thread_manager.h"
#include "render/render_tree_host.h"
namespace lynx {
class RenderFactory {
 public:
    static RenderObject* CreateRenderObject(
        jscore::ThreadManager* manager,
        const std::string& tag,
        RenderTreeHost* host) {
        static uint64_t id = 10;
        if (tag.compare("body") == 0) {
            RenderObject*renderer = lynx_new ListView("listview", ++id, host);
            host->render_root()->AppendChild(renderer);
            return renderer;
        } else if (tag.compare("view") == 0) {
            return lynx_new View("view", ++id, NULL, host);
        } else if (tag.compare("label") == 0) {
            return lynx_new Label(manager, "label", ++id, host);
        } else if (tag.compare("text") == 0) {
            return lynx_new TextNode(manager, "text", ++id, host);
        } else if (tag.compare("listview") == 0) {
            return lynx_new ListView("listview", ++id, host);
        } else if (tag.compare("listview-shadow") == 0) {
            return lynx_new ListShadow("listview-shadow", ++id, host);
        } else if (tag.compare("viewstub") == 0) {
            return lynx_new ViewStub(manager, "layoutview", ++id, host);
        } else if (tag.compare("img") == 0) {
            return lynx_new ImageView("img", ++id, host);
        } else if (tag.compare("scrollview") == 0) {
            return lynx_new ScrollView("scrollview", ++id, host);
        } else if (tag.compare("input") == 0) {
            return lynx_new Input("input", ++id, host);
        } else if (tag.compare("swiper") == 0) {
            return lynx_new SwiperView(manager, "swiper", ++id, host);
        } else if (tag.compare("textarea") == 0) {
            return lynx_new TextArea("textarea", ++id, host);
        } else if (tag.compare("switch") == 0) {
            return lynx_new ExtendedView("switch", LYNX_SWITCH, ++id, host);
        } else if (tag.compare("slider") == 0) {
            return lynx_new ExtendedView("slider", LYNX_SLIDER, ++id, host);
        } else if (tag.compare("button") == 0) {
            return lynx_new ExtendedView("button", LYNX_BUTTON, ++id, host);
        } else if (tag.compare("checkbox") == 0) {
            return lynx_new ExtendedView("checkbox", LYNX_CHECKBOX, ++id, host);
        } else if (tag.compare("radio") == 0) {
            return lynx_new ExtendedView("radio", LYNX_RADIO, ++id, host);
        } else if (tag.compare("radio-group") == 0) {
            return lynx_new View("radio-group",
                                 LYNX_RADIO_GROUP,
                                 ++id,
                                 RenderObjectImpl::Create(manager, LYNX_RADIO_GROUP),
                                 host);
        } else {
            LOGE("[LYNX ERROR]", "Element Tag(%s) is Not Support", tag.c_str());
        }
        return NULL;
    }
};
}  // namespace lynx


#endif  // LYNX_RENDER_RENDER_FACTORY_H_
