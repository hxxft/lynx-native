// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIView.h"
#import "IOSView.h"
#import "LynxUIAction.h"

@interface LynxUIBody : LynxUIView

- (void) resetView:(IOSView*)view;
- (void) collectAction:(LynxUIAction *)action;
@end