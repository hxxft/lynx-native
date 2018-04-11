// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef UI_BASE_LYNX_RUNTIME_H_
#define UI_BASE_LYNX_RUNTIME_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "base/lynx_render_tree_host_impl.h"
#include "base/lynx_function_object.h"

#include "runtime/runtime.h"

@interface LynxRuntime : NSObject {
    @private
    jscore::Runtime* runtime_;
}

@property(nonatomic, readonly) LynxRenderTreeHostImpl *host;

+ (CGFloat) defaultZoomRatio;

- (void) prepare;

- (void) prepareWithZoomRatio:(CGFloat) zoomRatio;

- (LynxRenderTreeHostImpl *) active;

- (void) loadUrl:(NSString *) url;

- (void) loadHtmlData:(NSString *) html;

- (void) loadHtml:(NSString *)url withSource:(NSString*)html;

- (NSString *) runScript:(NSString *)script;

- (void) addJavaScriptInterface:(LYXFunctionObject *) object withName:(NSString *) name ;

- (void) destroy;
@end

#endif
