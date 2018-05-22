#include "plugin/base/ios/plugin_impl.h"

#include "plugin/base/plugin_manager.h"

@interface NetInfoPlugin : LynxPlugin

@property (atomic, readwrite) NSString* networkStatus;

+(void) createWithManager: (plugin::PluginManager*)manager;

@end
