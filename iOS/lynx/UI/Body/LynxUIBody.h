// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxUIView.h"
#import "IOSView.h"
#import "LynxUIAction.h"
#import "LYXTransferStation.h"

@interface LynxUIBody : LynxUIView<LYXTransferStation>

- (void) resetView:(IOSView*)view;
- (void) collectAction:(LynxUIAction *)action;
@end
