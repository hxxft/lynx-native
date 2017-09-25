// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>
#import "IOSSwiperView.h"
#import "LynxUI.h"

@interface LynxSwiperController : NSObject<IOSSwiperViewDataSource, IOSSwiperViewDelegate>

@property(nonatomic, readwrite) NSMutableArray *children;
@property(nonatomic, readonly, weak) IOSSwiperView *swiperView;
@property(nonatomic, readonly, weak) LynxUI *ui;

- (id) init:(IOSSwiperView *) swiperView withUI:(LynxUI *)ui;
- (void) reloadData;

@end
