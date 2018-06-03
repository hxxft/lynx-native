// Copyright 2017 The Lynx Authors. All rights reserved.

#include "plugin/base/plugin.h"
#include "plugin/plugin_server.h"

#include "plugin/base/ios/plugin_impl.h"
#include "base/log/logging.h"
#include "base/ios/oc_helper.h"

#include <objc/message.h>
#include <Foundation/Foundation.h>


@implementation LynxPlugin {
    plugin::Plugin* delegate_;
}

-(void) setDelegate:(plugin::Plugin*) delegate {
    delegate_ = delegate;
}

-(void) exec:(NSInteger)clientId argments:(NSArray*)args {
    NSString* methodName = [NSString stringWithFormat:@"%@:method:arguments:", [args objectAtIndex:1]];
    SEL normalSelector = NSSelectorFromString(methodName);
    if ([self respondsToSelector:normalSelector]) {
        ((void (*)(id, SEL, NSInteger, id, id))objc_msgSend)(self, normalSelector, clientId, [args objectAtIndex:2], [args objectAtIndex:3]);
    } else {
        NSLog(@"ERROR: Method '%@' not defined in Plugin '%@'", methodName, [args objectAtIndex:0]);
    }
}

-(void) returnBack:(NSInteger)clientId method:(NSNumber*) method successed:(Boolean)successed argments:(NSArray*)args {
     base::ScopedPtr<jscore::LynxArray> array = base::ios::OCHelper::ConvertToLynxArray(args);
     delegate_->Return(clientId, [method intValue], successed, array);
}

-(void) addEvent:(NSString*) event {
    NSLog(@"ERROR: AddEvent not implement in Plugin '%@'", _pluginName);
}

-(void) removeEvent:(NSString*) event {
     NSLog(@"ERROR: RemoveEvent not implement in Plugin '%@'", _pluginName);
}

-(void) dispatchEvent:(NSString*) event argments:(NSArray*)args {
    base::ScopedPtr<jscore::LynxArray> array = base::ios::OCHelper::ConvertToLynxArray(args);
    std::string plugin_name([_pluginName UTF8String]);
    std::string plugin_event([event UTF8String]);
    delegate_->DispatchEvent(plugin_name, plugin_event, array);
}

@end

namespace plugin {
    class PluginImpl : public Plugin {
    public:
        PluginImpl(){}
        virtual ~PluginImpl() {}
        PluginImpl(LynxPlugin* instance) : Plugin(), instance_(instance){
            [instance_ setDelegate:this];
        }
        
        virtual void Exec(long client_id, base::ScopedPtr<jscore::LynxArray> args) {
            NSArray* array = base::ios::OCHelper::ConvertToOCArray(args.Get());
            [instance_ exec:client_id argments:array];
        }
        
        virtual void AddEvent(const std::string& event) {
            [instance_ addEvent:[NSString stringWithUTF8String:event.c_str()]];
        }
        
        virtual void RemoveEvent(const std::string& event) {
            [instance_ removeEvent:[NSString stringWithUTF8String:event.c_str()]];
        }
    private:
        LynxPlugin* instance_;
    };
    
    Plugin* Plugin::Create(const std::string &name) {
        NSString* plugin_clazz_name = [NSString stringWithFormat:@"%@Plugin", [NSString stringWithUTF8String:name.c_str()]];
        Class clazz = NSClassFromString(plugin_clazz_name);
        LynxPlugin* lynx_plugin = [[clazz alloc] init];
        return new PluginImpl(lynx_plugin);
    }
}



