//
//  plugin_register.m
//  lynx
//
//  Created by dli on 2018/5/20.
//  Copyright © 2018年 lynx. All rights reserved.
//


#include "plugin/base/plugin_manager.h"
#include "plugin/base/plugin_builder.h"

#import "plugin/impl/net_info/ios/net_info_plugin.h"

namespace plugin {
    void PluginBuilder::build(PluginManager* manager) {
        [NetInfoPlugin createWithManager:manager];
    }
}
