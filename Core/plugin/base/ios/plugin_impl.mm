#include "plugin/base/plugin.h"
#include "plugin/base/ios/plugin_impl.h"
#include "plugin/base/plugin_manager.h"
#include "base/log/logging.h"
#include "base/ios/oc_helper.h"

#include <objc/message.h>

namespace plugin {
    class PluginImpl : public Plugin {
    public:
        PluginImpl(const char* name, PluginManager* manager, LynxPlugin* instance) : Plugin(manager), instance_(instance){
            manager->Register(name, this);
        }
        
        virtual void Exec(base::ScopedPtr<jscore::LynxArray> args) {
            NSArray* array = base::ios::OCHelper::ConvertToOCArray(args.Get());
            [instance_ Exec:array];
        }
        
    private:
        LynxPlugin* instance_;
    };
}

@implementation LynxPlugin {
    plugin::Plugin* delegate_;
}

-(id) initWithName:(NSString*)name pluginManager:(plugin::PluginManager*)manager {
    self = [super init];
    if(self != nil) {
        delegate_ = new plugin::PluginImpl([name UTF8String], manager, self);
    }
    return self;
}

-(void) Exec:(NSArray*)args {
    NSString* methodName = [NSString stringWithFormat:@"%@:arguments:", [args objectAtIndex:1]];
    SEL normalSelector = NSSelectorFromString(methodName);
    if ([self respondsToSelector:normalSelector]) {
        ((void (*)(id, SEL, id, id))objc_msgSend)(self, normalSelector, [args objectAtIndex:2], [args objectAtIndex:3]);
    } else {
        NSLog(@"ERROR: Method '%@' not defined in Plugin '%@'", methodName, [args objectAtIndex:0]);
    }
}

-(void) Return:(NSNumber*) methodId resultType:(plugin::Plugin::ResultType)type argments:(NSArray*)args {
     base::ScopedPtr<jscore::LynxArray> array = base::ios::OCHelper::ConvertToLynxArray(args);
     delegate_->Return([methodId intValue], type, array);
}



@end



