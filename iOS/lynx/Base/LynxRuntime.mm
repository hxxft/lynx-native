// Copyright 2017 The Lynx Authors. All rights reserved.

#import "LynxRuntime.h"

#include "config/global_config_data.h"
#include "parser/render_parser.h"
#include "render/render_tree_host.h"
#include "render/ios/render_tree_host_impl_ios.h"

@implementation LynxRuntime

- (id) init {
    self = [super init];
    if (self) {
        runtime_ = new jscore::JSCRuntime();
        // Default
        runtime_->InitRuntime("");
    }
    return self;
}

- (LynxRenderTreeHostImpl *) activeWithSize:(CGSize)size andDensity:(CGFloat)density {
    config::GlobalConfigData::GetInstance()->SetScreenConfig(size.width, size.height, density);
    // Create RenderTreeHost
    lynx::RenderTreeHost* render_tree_host = runtime_->SetupRenderHost();
    return reinterpret_cast<lynx::RenderTreeHostImplIOS*>(render_tree_host->host_impl())->ios();
}

- (void)loadUrl:(NSString *)url {
    runtime_->LoadUrl(std::string([url UTF8String]));
}

- (void)loadHtml:(NSString *)url withSource:(NSString*)html{
    base::PlatformString scoped_url(url);
    base::PlatformString scoped_source(html);
    runtime_->LoadHTML(scoped_url.ToString(), scoped_source.ToString());
}

- (void)loadHtmlData:(NSString *)html {
    base::PlatformString scoped_source(html);
    runtime_->LoadHTML(scoped_source.ToString());
}

- (NSString *) runScript:(NSString *)script {
    base::PlatformString scoped_source(script);
    runtime_->RunScript(scoped_source);
    return @"";
}

- (void)destroy {
    runtime_->Destroy();
}

@end
