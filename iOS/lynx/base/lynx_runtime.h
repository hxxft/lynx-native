// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_LYNX_RUNTIME_H_
#define UI_BASE_LYNX_RUNTIME_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/render_tree_host_impl_bridge.h"
#include "base/lynx_object.h"

#include "runtime/runtime.h"

@interface LynxRuntime : NSObject {
    @private
    jscore::Runtime* runtime_;
}

@property(nonatomic, readonly) RenderTreeHostImplBridge *host;

+ (CGFloat) defaultZoomRatio;

- (void) prepare;

- (void) prepareWithZoomRatio:(CGFloat) zoomRatio;

- (RenderTreeHostImplBridge *) active;

- (void) loadUrl:(NSString *) url;

- (void) loadHtmlData:(NSString *) html;

- (void) loadHtml:(NSString *)url withSource:(NSString*)html;

- (NSString *) runScript:(NSString *)script;

- (void) addJavaScriptInterface:(LxObject *) object withName:(NSString *) name ;

- (void) destroy;
@end

#endif
