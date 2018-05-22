#ifndef LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_
#define LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_

#include "plugin/base/plugin.h"
#include "plugin/base/plugin_manager.h"

@protocol PluginProtocol <NSObject>

@required
-(void) Exec:(NSArray*)args;

@end


@interface LynxPlugin : NSObject<PluginProtocol>
-(id) initWithName:(NSString*)name pluginManager:(plugin::PluginManager*)manager;
-(void) Return:(NSNumber*) methodId resultType:(plugin::Plugin::ResultType)type argments:(NSArray*)args;
@end

#endif  // LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_
