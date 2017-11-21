// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxView.h"
#import "LynxUIBody.h"
#import "LynxRenderObjectImpl.h"
#import "LynxFilePathUtility.h"

#include "runtime/jsc/jsc_runtime.h"
#include "layout/css_style.h"
#include "render/ios/render_object_impl_ios.h"
#include "config/global_config_data.h"
#include "parser/render_parser.h"

using namespace lynx;

@implementation LynxView

-(id) init {
    CGRect screenRect = [UIScreen mainScreen].bounds;
    self = [self initWithFrame: CGRectMake(0, 0, screenRect.size.width, screenRect.size.height)];
    return self;
}

-(id) initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        _runtime = [[LynxRuntime alloc] init];
        _renderTreeHostImpl = [_runtime activeWithSize:frame.size andDensity:1];
        [_renderTreeHostImpl updateViewport:frame.size];
        
        // Connect RenderTreeHost and BodyView
        LynxUIBody* lynxUIBody = (LynxUIBody*)[_renderTreeHostImpl.rootRenderObjectImpl createLynxUI];
        [lynxUIBody resetView:self];
        
        // Create FrameRateController
        _controller = [[LynxFrameRateController alloc] initWithVSyncListener:_renderTreeHostImpl];
        
        self.clipsToBounds = YES;
        [self setBackgroundColor:[UIColor whiteColor]];
    }
    return self;
}

-(void) loadScriptData:(NSString*)source {
    [_runtime runScript:source];
}

-(void) loadHTMLData:(NSString*)source {
    [_runtime loadHtmlData:source];
}

-(void) loadScriptFile:(NSString*)scriptFile
{
    if (nil != scriptFile) {
        NSString* file = [[NSBundle mainBundle] pathForResource:scriptFile ofType:@"js"];
        NSData* fileData = [NSData dataWithContentsOfFile:file];
        if (nil != fileData && fileData.length > 0)
        {
            NSString* scriptString = [[NSString alloc] initWithData:fileData
                                                           encoding:NSUTF8StringEncoding];
            if (nil != scriptString && scriptString.length > 0)
            {
                [self loadScriptData:scriptString];
            }
        }
    }
}

-(void) loadPage:(NSString*)pagePath
{
    if (nil != pagePath) {
        NSString* pageLayoutPath = [pagePath stringByAppendingString:@"index.html"];

        NSString* file = [LynxFilePathUtility toFilePath:pageLayoutPath];
        NSData* fileData = [NSData dataWithContentsOfFile:file];
        if (nil != fileData && fileData.length > 0)
        {
            NSString* htmlString = [[NSString alloc] initWithData:fileData
                                                           encoding:NSUTF8StringEncoding];
            if (nil != htmlString && htmlString.length > 0)
            {
                [_runtime loadHtml:pagePath withSource:htmlString];
            }
        }
    }
}

-(void) loadUrl:(NSString*)url {
    [_runtime loadUrl:url];
}

- (void)layoutSubviews {
    [super layoutSubviews];
}

-(void)didMoveToWindow {
    [super didMoveToWindow];
    if (self.window) {
        [_controller start];
    } else {
        [_controller stop]; 
    }
}

- (void)destroy {
//    [_runtime destroy];
}

@end
