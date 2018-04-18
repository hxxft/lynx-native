// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>
#include "base/framerate_controller.h"
#include "widget/view/lynx_ui_view.h"
#include "widget/view/view_wrapper.h"
#import "base/lynx_runtime.h"
#import "base/render_tree_host_impl_bridge.h"

#include "runtime/runtime.h"
#include "render/render_object.h"
#include "render/ios/render_tree_host_impl_ios.h"

@interface LxContentView : ViewWrapper

@property(nonatomic, readwrite) LynxRuntime* runtime;
@property(nonatomic, readwrite) RenderTreeHostImplBridge* renderTreeHostImpl;
@property(nonatomic, readwrite) LynxFrameRateController* controller;

-(id) init;

-(id) initWithFrame:(CGRect)frame;

-(void) loadScriptData:(NSString*)source;

-(void) loadScriptFile:(NSString*)scriptFile;

-(void) loadPage:(NSString*)htmlFile;

-(void) loadUrl:(NSString*)url;

-(void) layoutSubviews;

-(void) destroy;

@end
