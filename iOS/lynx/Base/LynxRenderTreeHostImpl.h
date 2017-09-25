// Copyright 2017 The Lynx Authors. All rights reserved.

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "LynxUIAction.h"
#import "LynxRenderObjectImpl.h"
#import "LynxUIActionCollector.h"

namespace lynx {
    class RenderTreeHostImplIOS;
}

@interface LynxRenderTreeHostImpl : NSObject {
    @private
    lynx::RenderTreeHostImplIOS* render_tree_host_impl_;
}

@property(nonatomic, readonly) LynxRenderObjectImpl *rootRenderObjectImpl;
@property(nonatomic, readwrite) LynxUIActionCollector *collector;
@property(nonatomic, readwrite) LynxUIActionCollector *lastCollector;

- (id) initWithRenderTreeHost:(lynx::RenderTreeHostImplIOS *) renderTreeHostImpl
          andRenderObjectImpl:(lynx::RenderObjectImplIOS *) renderObjectImpl;

- (void) onVSync;

- (void) beginFrame;

- (void) prepareCommit;

- (void) updateViewport:(CGSize)size;

- (void) collectAction:(LynxUIAction *)action;
@end
