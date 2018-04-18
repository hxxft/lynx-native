// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/ui_factory.h"

#include "widget/body/lynx_ui_body.h"
#include "widget/label/lynx_ui_label.h"
#include "widget/view/lynx_ui_view.h"
#include "widget/image/lynx_ui_image.h"
#include "widget/listview/lynx_ui_listview.h"
#include "widget/scrollview/lynx_ui_scrollview.h"


@implementation LynxUIFactory

+ (LynxUI *) createUI:(RenderObjectImplBridge *) impl {
    LynxUI* ui = NULL;
    switch (impl->type_) {
        case lynx::LYNX_BODY:
            ui = [[LxUIBody alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_VIEW:
        case lynx::LYNX_CELLVIEW:
            ui = [[LxUIView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_LABEL:
            ui = [[LxUILabel alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_LISTVIEW:
            ui = [[LxUIListView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_IMAGEVIEW:
            ui = [[LxUIImage alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_SCROLLVIEW:
            ui = [[LxUIScrollView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_SWIPER:
            break;
        default:
            break;
    }
    return ui;
}
@end
