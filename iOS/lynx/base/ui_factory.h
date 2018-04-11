// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_UI_FACTORY_H_
#define UI_BASE_UI_FACTORY_H_

#import <Foundation/Foundation.h>

#include "base/lynx_render_object_impl.h"

@interface LynxUIFactory : NSObject

+ (LynxUI *) createUI:(LynxRenderObjectImpl *) impl;
@end

#endif  // UI_BASE_UI_FACTORY_H_
