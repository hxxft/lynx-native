// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/lynx_runtime.h"

#include "modules/module_register.h"
#include "utils/device_info_util.h"
#include "utils/screen_util.h"

#include "config/global_config_data.h"
#include "parser/render_parser.h"
#include "render/render_tree_host.h"
#include "render/ios/render_tree_host_impl_ios.h"
#include "runtime/ios/lynx_object_ios.h"
#include "runtime/jsc/jsc_context.h"

#include "base/string/platform_string_impl.h"

@implementation LynxRuntime

static CGFloat kDefaultZoomRatio = -1;

+ (CGFloat) defaultZoomRatio {
    return kDefaultZoomRatio;
}

- (id) init {
    self = [super init];
    if (self) {
        runtime_ = new jscore::Runtime(new jscore::JSCContext());
        // Default
        runtime_->InitRuntime("");
    }
    return self;
}

- (void) prepare {
    [self prepareWithZoomRatio: -1];
}

- (void) prepareWithZoomRatio:(CGFloat) zoomRatio {
    std::string deviceInfo = [[LxDeviceInfoUtil deviceInfo] UTF8String];
    config::GlobalConfigData::GetInstance()->SetScreenConfig([[LxScreenUtil shareInstance] getScreenWidth],
                                                             [[LxScreenUtil shareInstance] getScreenHeight],
                                                             [[LxScreenUtil shareInstance] getScreenDensity],
                                                             zoomRatio,
                                                             deviceInfo);
}

- (RenderTreeHostImplBridge *) active {
    // Create RenderTreeHost
    lynx::RenderTreeHost* render_tree_host = runtime_->SetupRenderHost();
//    LxModuleRegister *moduleRegister = [[LxModuleRegister alloc] initWithRuntime:self];
    [LxModuleRegister registeWithRuntime:self];
    _host = reinterpret_cast<lynx::RenderTreeHostImplIOS*>(render_tree_host->host_impl())->tree_host_bridge();
    return _host;
}

- (void)loadUrl:(NSString *)url {
    runtime_->LoadUrl(std::string([url UTF8String]));
}

- (void)loadHtml:(NSString *)url withSource:(NSString*)html{
    base::PlatformStringImpl scoped_url(url);
    base::PlatformStringImpl scoped_source(html);
    runtime_->LoadHTML(scoped_url.ToString(), scoped_source.ToString());
}

- (void)loadHtmlData:(NSString *)html {
    base::PlatformStringImpl scoped_source(html);
    runtime_->LoadHTML(scoped_source.ToString());
}

- (NSString *) runScript:(NSString *)script {
    base::ScopedPtr<base::PlatformString> scoped_source(
                                                        static_cast<base::PlatformString*>(new base::PlatformStringImpl(script)));
    runtime_->RunScript(scoped_source);
    return @"";
}

- (void) addJavaScriptInterface:(LxObject *) object
                       withName:(NSString *) name {
    runtime_->AddJavaScriptInterface([name UTF8String], new jscore::LynxObjectIOS(object));
}

- (void)destroy {
    runtime_->Destroy();
}

@end
