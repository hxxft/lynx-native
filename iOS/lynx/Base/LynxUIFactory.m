// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIFactory.h"
#import "LynxUIBody.h"
#import "LynxUILabel.h"
#import "LynxUIView.h"
#import "LynxUIListView.h"
#import "LynxUIImage.h"
#import "LynxUIScrollView.h"
#import "LynxRenderObjectImpl.h"

@implementation LynxUIFactory

+ (LynxUI *) createUI:(LynxRenderObjectImpl *) impl {
    LynxUI* ui = NULL;
    switch (impl->type_) {
        case lynx::LYNX_BODY:
            ui = [[LynxUIBody alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_VIEW:
        case lynx::LYNX_CELLVIEW:
            ui = [[LynxUIView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_LABEL:
            ui = [[LynxUILabel alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_LISTVIEW:
            ui = [[LynxUIListView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_IMAGEVIEW:
            ui = [[LynxUIImage alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_SCROLLVIEW:
            ui = [[LynxUIScrollView alloc] initWithRenderObjectImpl:impl];
            break;
        case lynx::LYNX_SWIPER:
            break;
        default:
            break;
    }
    return ui;
}
@end
