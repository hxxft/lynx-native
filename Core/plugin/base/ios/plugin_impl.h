// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_
#define LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_

#include <Foundation/Foundation.h>

@interface LynxPlugin : NSObject

@property (atomic, readwrite)NSString* pluginName;
-(void) exec:(NSInteger)clientId argments:(NSArray*)args;
-(void) returnBack:(NSInteger)clientId method:(NSNumber*)methodId successed:(Boolean)successed argments:(NSArray*)args;
-(void) addEvent:(NSString*) event;
-(void) removeEvent:(NSString*) event;
-(void) dispatchEvent:(NSString*) event argments:(NSArray*)args;

@end

#endif  // LYNX_PLUGIN_BASE_IOS_PLUGIN_MANAGER_H_
