// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxRenderTreeHostImpl.h"
#import "LYXFunctionObject.h"

#include "runtime/jsc/jsc_runtime.h"

@interface LynxRuntime : NSObject {
    @private
    jscore::JSCRuntime* runtime_;
}

@property(nonatomic, readonly) LynxRenderTreeHostImpl *host;

- (LynxRenderTreeHostImpl *) activeWithSize:(CGSize)size andDensity:(CGFloat)density;

- (void) loadUrl:(NSString *) url;

- (void) loadHtmlData:(NSString *) html;

- (void) loadHtml:(NSString *)url withSource:(NSString*)html;

- (NSString *) runScript:(NSString *)script;

- (void) addJavaScriptInterface:(LYXFunctionObject *) object withName:(NSString *) name ;

- (void) destroy;
@end
