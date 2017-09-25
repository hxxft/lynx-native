// Copyright 2017 The Lynx Authors. All rights reserved.

#import <UIKit/UIKit.h>
#import "LynxFrameRateController.h"
#import "LynxUIView.h"
#import "IOSView.h"
#import "LynxRuntime.h"
#import "LynxRenderTreeHostImpl.h"

#include "runtime/runtime.h"
#include "render/render_object.h"
#include "render/ios/render_tree_host_impl_ios.h"

@interface LynxView : IOSView

@property(nonatomic, readwrite) LynxRuntime* runtime;
@property(nonatomic, readwrite) LynxRenderTreeHostImpl* renderTreeHostImpl;
@property(nonatomic, readwrite) LynxFrameRateController* controller;

-(id) init;

-(id) initWithFrame:(CGRect)frame;

-(void) loadScriptData:(NSString*)source;

-(void) loadScriptFile:(NSString*)scriptFile;

-(void) loadHTMLFile:(NSString*)htmlFile;

-(void) loadUrl:(NSString*)url;

-(void) layoutSubviews;

-(void) destroy;

@end
