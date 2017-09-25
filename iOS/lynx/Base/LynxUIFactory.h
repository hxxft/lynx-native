// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import "LynxRenderObjectImpl.h"

@interface LynxUIFactory : NSObject

+ (LynxUI *) createUI:(LynxRenderObjectImpl *) impl;
@end
