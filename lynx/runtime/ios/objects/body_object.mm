// Copyright 2017 The Lynx Authors. All rights reserved.

#include "body_object.h"
#include "render_factory.h"
#include "config/global_config_data.h"
using namespace std;
using namespace lynx;

@implementation BodyObject

@synthesize clientWidth, clientHeight;

-(id) initWithHost:(lynx::RenderTreeHost*)host {
    self = [self init];
    if (self) {
        render_object_.Reset(host->render_root());
        render_object_->SetJSRef((__bridge void *)(self));
    }
    return self;
}

-(CGFloat) clientWidth {
    return (CGFloat)config::GlobalConfigData::GetInstance()->screen_width();
}

-(CGFloat) clientHeight {
    return (CGFloat)config::GlobalConfigData::GetInstance()->screen_height();
}


@end
