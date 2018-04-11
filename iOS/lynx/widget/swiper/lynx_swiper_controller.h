// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_WIDGET_SWIPER_LYNX_SWIPER_CONTROLLER_H_
#define UI_WIDGET_SWIPER_LYNX_SWIPER_CONTROLLER_H_

#import <UIKit/UIKit.h>
#include "widget/swiper/lynx_swiper_view.h"
#include "widget/lynx_ui.h"

@interface LxSwiperController : NSObject<LxSwiperViewDataSource, LxSwiperViewDelegate>

@property(nonatomic, readwrite) NSMutableArray *children;
@property(nonatomic, readonly, weak) LxSwiperView *swiperView;
@property(nonatomic, readonly, weak) LynxUI *ui;

- (id) init:(LxSwiperView *) swiperView withUI:(LynxUI *)ui;
- (void) reloadData;

@end

#endif  // UI_WIDGET_SWIPER_LYNX_SWIPER_CONTROLLER_H_
