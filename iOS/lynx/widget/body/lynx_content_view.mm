// Copyright 2017 The Lynx Authors. All rights reserved.

#include "widget/body/lynx_content_view.h"
#include "widget/body/lynx_ui_body.h"
#include "base/lynx_render_object_impl.h"
#include "base/file_path_utility.h"
#include "utils/screen_util.h"

#include "layout/css_style.h"
#include "render/ios/render_object_impl_ios.h"
#include "config/global_config_data.h"
#include "parser/render_parser.h"

using namespace lynx;

@implementation LxContentView

-(id) init {
    CGRect screenRect = [UIScreen mainScreen].bounds;
    self = [self initWithFrame: CGRectMake(0, 0, screenRect.size.width, screenRect.size.height)];
    return self;
}

-(id) initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        
        // Init Runtime
        _runtime = [[LynxRuntime alloc] init];
        [_runtime prepare];
        _renderTreeHostImpl = [_runtime active];
        
        // Update viewport
        [_renderTreeHostImpl updateViewport:frame];
        
        // Connect RenderTreeHost and BodyView
        LynxUI* ui = [[_renderTreeHostImpl rootRenderObjectImpl] createLynxUI];
        LxUIBody* LxUIBody = ui;
        [LxUIBody resetView:self];
        
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
